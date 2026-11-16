"""Generate three real, UV-unwrapped PBR modular stone meshes and export FBX.

Run only in an isolated background Blender process:
    blender --background --factory-startup --python build_modular_kit.py -- --out <new-output-directory>

Requires Blender with FBX exporter. Refuses existing output directories.
Does not modify any supplied .blend project or other checkout.
"""
import argparse
import hashlib
import json
from pathlib import Path
import sys

import bpy
from mathutils import Matrix


def output_path():
    values = sys.argv[sys.argv.index("--") + 1:] if "--" in sys.argv else []
    parser = argparse.ArgumentParser()
    parser.add_argument("--out", required=True, help="A NEW directory; never an existing path")
    args = parser.parse_args(values)
    path = Path(args.out).expanduser().resolve()
    if path.exists():
        raise FileExistsError(f"Refusing to overwrite or share existing output: {path}")
    if not path.parent.is_dir():
        raise FileNotFoundError(f"Output parent must exist: {path.parent}")
    return path


def stone_material():
    material = bpy.data.materials.new("M_GPP_OriginalStone")
    material.use_nodes = True
    shader = material.node_tree.nodes.get("Principled BSDF")
    if shader is None:
        raise RuntimeError("Blender Principled BSDF node not present")
    shader.inputs["Base Color"].default_value = (0.17, 0.19, 0.21, 1.0)
    shader.inputs["Metallic"].default_value = 0.0
    shader.inputs["Roughness"].default_value = 0.82
    return material


def make_module(name, width, depth, height, bevel_width, material):
    bpy.ops.mesh.primitive_cube_add(size=1.0, calc_uvs=True)
    obj = bpy.context.object
    obj.name = name
    obj.data.name = name + "_Geometry"
    obj.scale = (width, depth, height)
    bpy.ops.object.transform_apply(location=False, rotation=False, scale=True)

    # Transform vertices instead of the object: each origin stays at the base center.
    obj.data.transform(Matrix.Translation((0.0, 0.0, height / 2.0)))
    obj.data.materials.append(material)

    modifier = obj.modifiers.new("ActualBevelGeometry", "BEVEL")
    modifier.width = bevel_width
    modifier.segments = 2
    modifier.limit_method = "ANGLE"
    result = bpy.ops.object.modifier_apply(modifier=modifier.name)
    if "FINISHED" not in result:
        raise RuntimeError(f"Failed to apply bevel to {name}")

    bpy.ops.object.mode_set(mode="EDIT")
    bpy.ops.mesh.select_all(action="SELECT")
    result = bpy.ops.uv.smart_project(island_margin=0.02)
    if "FINISHED" not in result:
        raise RuntimeError(f"UV unwrap failed for {name}")
    bpy.ops.object.mode_set(mode="OBJECT")

    bpy.context.view_layer.update()
    if len(obj.data.uv_layers) < 1 or len(obj.data.polygons) < 1:
        raise RuntimeError(f"Missing geometry/UVs in {name}")

    # Actual mesh triangles, not an estimated vertex budget.
    obj.data.calc_loop_triangles()
    triangle_count = len(obj.data.loop_triangles)
    if not triangle_count:
        raise RuntimeError(f"No triangles in {name}")

    assert abs(obj.dimensions.x - width) < 0.01
    assert abs(obj.dimensions.y - depth) < 0.01
    assert abs(obj.dimensions.z - height) < 0.01
    assert abs(obj.location.x) < 1e-6 and abs(obj.location.y) < 1e-6
    assert abs(obj.location.z) < 1e-6

    z_min = min(vertex.co.z for vertex in obj.data.vertices)
    if abs(z_min) > 0.01:
        raise RuntimeError(f"Pivot at {name} is not aligned to the base")

    return obj, triangle_count


def file_sha256(path):
    sha = hashlib.sha256()
    with open(path, "rb") as stream:
        for piece in iter(lambda: stream.read(1024 * 1024), b""):
            sha.update(piece)
    return sha.hexdigest()


def main():
    if not bpy.app.background:
        raise RuntimeError("Refusing to reset a live Blender session. Use --background.")
    folder = output_path()
    # Reset only the new background process to avoid changing unrelated open projects.
    bpy.ops.wm.read_factory_settings(use_empty=True)
    folder.mkdir(parents=False)
    bpy.context.scene.unit_settings.system = "METRIC"
    bpy.context.scene.unit_settings.scale_length = 1.0

    material = stone_material()
    recipes = [
        ("SM_GPP_Floor_200", 2.0, 2.0, 0.2, 0.025),
        ("SM_GPP_Wall_200", 2.0, 0.3, 2.4, 0.03),
        ("SM_GPP_Pillar_035", 0.35, 0.35, 2.8, 0.018),
    ]
    models = []
    for name, width, depth, height, bevel in recipes:
        mesh, triangles = make_module(name, width, depth, height, bevel, material)
        models.append((mesh, triangles, (width, depth, height)))

    master = folder / "GPP_Modular_Stone_Original.blend"
    result = bpy.ops.wm.save_as_mainfile(filepath=str(master), check_existing=True)
    if "FINISHED" not in result or not master.is_file():
        raise RuntimeError("Blender master file was not saved")

    report = {
        "blender_version": bpy.app.version_string,
        "source_file": master.name,
        "source_sha256": file_sha256(master),
        "units": "metres",
        "fbx_axis_forward": "-Y",
        "fbx_axis_up": "Z",
        "geometry": [],
        "unreal_import_status": "NOT RUN",
        "human_playtest_status": "NOT RUN",
    }
    for mesh, triangles, sizes in models:
        bpy.ops.object.select_all(action="DESELECT")
        mesh.select_set(True)
        bpy.context.view_layer.objects.active = mesh
        export = folder / (mesh.name + ".fbx")
        status = bpy.ops.export_scene.fbx(
            filepath=str(export), check_existing=True,
            use_selection=True, object_types={"MESH"},
            global_scale=1.0, apply_unit_scale=True,
            axis_forward="-Y", axis_up="Z",
            bake_anim=False, add_leaf_bones=False,
            use_mesh_modifiers=True, embed_textures=False,
        )
        if "FINISHED" not in status or not export.is_file() or export.stat().st_size == 0:
            raise RuntimeError(f"FBX export failed: {export.name}")
        report["geometry"].append({
            "name": mesh.name,
            "dimensions_m": list(sizes),
            "triangles": triangles,
            "uv_layer_count": len(mesh.data.uv_layers),
            "material_slots": [slot.material.name for slot in mesh.material_slots],
            "export": export.name,
            "sha256": file_sha256(export),
        })

    with open(folder / "manifest.json", "x", encoding="utf-8") as stream:
        json.dump(report, stream, indent=2)
    print("CREATED source + 3 real FBX meshes; Engine import/runtime checks NOT RUN")
    print(json.dumps(report, indent=2))


if __name__ == "__main__":
    main()
