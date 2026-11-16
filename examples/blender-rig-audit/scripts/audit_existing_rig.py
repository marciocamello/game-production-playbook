"""Read-only rig/rest-pose/animation audit for an EXISTING Blender file.

Invocation:
 blender --background real-rig.blend --python audit_existing_rig.py -- \
   --armature ActualRigName --mesh ActualMeshName --report /new/location/rig-audit.json

Does not write or save the .blend. Writes a NEW JSON report only, never overwrites.
"""
import argparse
import json
from math import isfinite
from pathlib import Path
import sys

import bpy


def parse():
    raw = sys.argv[sys.argv.index("--") + 1:] if "--" in sys.argv else []
    parser = argparse.ArgumentParser()
    parser.add_argument("--armature", required=True)
    parser.add_argument("--mesh", required=True)
    parser.add_argument("--compare-armature")
    parser.add_argument("--required-actions", nargs="*", default=[])
    parser.add_argument("--report", required=True)
    return parser.parse_args(raw)


def matrix_numbers(matrix):
    result = [round(float(v), 7) for row in matrix for v in row]
    if not all(isfinite(v) for v in result):
        raise ValueError("Non-finite bone matrix detected")
    return result


def rig_bones(rig):
    return {
        bone.name: {
            "parent": bone.parent.name if bone.parent else None,
            "length_m": round(float(bone.length), 7),
            "rest_matrix_armature_space": matrix_numbers(bone.matrix_local),
            "deform": bool(bone.use_deform),
        }
        for bone in rig.data.bones
    }


def comparison(a, b, tolerance=0.0001):
    names_a, names_b = set(a), set(b)
    shared = sorted(names_a & names_b)
    parent_mismatches = [n for n in shared if a[n]["parent"] != b[n]["parent"]]
    rest_mismatches = []
    for name in shared:
        delta = max(abs(x - y) for x, y in zip(
            a[name]["rest_matrix_armature_space"],
            b[name]["rest_matrix_armature_space"],
        ))
        if delta > tolerance:
            rest_mismatches.append({"bone": name, "max_matrix_delta": round(delta, 7)})
    return {
        "only_in_source": sorted(names_a - names_b),
        "only_in_target": sorted(names_b - names_a),
        "parent_mismatches": parent_mismatches,
        "rest_pose_mismatches": rest_mismatches,
        "exact_equivalence_within_tolerance":
            not (names_a ^ names_b or parent_mismatches or rest_mismatches),
        "notice": "Different rest poses may be retargetable; they are NOT the same Skeleton asset.",
    }


def mesh_weights(mesh, bones):
    names = set(bones)
    group_names = {v.index: v.name for v in mesh.vertex_groups}
    uncovered = 0
    for vertex in mesh.data.vertices:
        total = sum(assignment.weight for assignment in vertex.groups
                    if group_names.get(assignment.group) in names)
        if total < 0.001:
            uncovered += 1
    armature_targets = [m.object.name for m in mesh.modifiers
                        if m.type == "ARMATURE" and m.object]
    return {
        "vertex_count": len(mesh.data.vertices),
        "vertices_without_bone_weight": uncovered,
        "armature_modifier_targets": armature_targets,
        "object_scale": [round(float(v), 7) for v in mesh.scale],
        "dimensions_m": [round(float(v), 6) for v in mesh.dimensions],
    }


def sample_active_pose(rig):
    animation = rig.animation_data
    if animation is None or animation.action is None:
        return {"status": "NO_ACTIVE_ACTION", "samples": []}

    action = animation.action
    start, finish = [float(v) for v in action.frame_range]
    frames = sorted(set([start, (start + finish) / 2.0, finish]))
    original_frame = bpy.context.scene.frame_current_final
    roots = [bone for bone in rig.pose.bones if bone.parent is None]
    samples = []
    try:
        for frame in frames:
            integer_frame = int(frame)
            bpy.context.scene.frame_set(integer_frame, subframe=frame - integer_frame)
            bpy.context.view_layer.update()
            samples.append({
                "frame": frame,
                "root_positions_world_m": {
                    bone.name: [round(float(v), 6) for v in
                                (rig.matrix_world @ bone.matrix).translation]
                    for bone in roots
                },
            })
    finally:
        integer_frame = int(original_frame)
        bpy.context.scene.frame_set(
            integer_frame, subframe=original_frame - integer_frame)
    return {"status": "SAMPLED", "action": action.name, "samples": samples}


def main():
    if not bpy.app.background:
        raise RuntimeError("Use an isolated --background Blender process.")
    args = parse()
    if not bpy.data.filepath:
        raise RuntimeError("Open an EXISTING rigged .blend when launching Blender.")

    rig = bpy.data.objects.get(args.armature)
    mesh = bpy.data.objects.get(args.mesh)
    if not rig or rig.type != "ARMATURE" or not rig.data.bones:
        raise RuntimeError("Requested armature with real bones is missing.")
    if not mesh or mesh.type != "MESH" or not mesh.data.vertices:
        raise RuntimeError("Requested rigged mesh with real vertices is missing.")

    missing_actions = [name for name in args.required_actions
                       if bpy.data.actions.get(name) is None]
    if missing_actions:
        raise RuntimeError("Required real Blender actions absent: " + ", ".join(missing_actions))

    bones = rig_bones(rig)
    report = {
        "source_blend_name": Path(bpy.data.filepath).name,
        "blender_version": bpy.app.version_string,
        "armature": rig.name,
        "bone_count": len(bones),
        "bones": bones,
        "armature_scale": list(rig.scale),
        "mesh": {"name": mesh.name, **mesh_weights(mesh, bones)},
        "required_action_names_found": args.required_actions,
        "active_action_pose_samples": sample_active_pose(rig),
        "unreal_import": "NOT RUN",
        "unreal_ik_retarget": "NOT RUN",
        "animation_only_fbx_scale_check": "NOT RUN",
        "human_deformation_review": "NOT RUN",
    }
    if args.compare_armature:
        other = bpy.data.objects.get(args.compare_armature)
        if not other or other.type != "ARMATURE":
            raise RuntimeError("Comparison Armature was not found.")
        report["rest_pose_comparison"] = comparison(bones, rig_bones(other))

    destination = Path(args.report).expanduser().resolve()
    if not destination.parent.is_dir() or destination.exists():
        raise FileExistsError("Report parent must exist and report path must be new.")
    with destination.open("x", encoding="utf-8") as stream:
        json.dump(report, stream, indent=2, allow_nan=False)
    print("Read-only rig audit created:", destination)


if __name__ == "__main__":
    main()
