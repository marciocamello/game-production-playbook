"""Author seven real, collision-bearing structural blockout actors in one approved Unreal Editor level.

Requires real UE Editor Python execution as the SOLE level writer.
Reads GPP_BLOCKOUT_MAP and GPP_BLOCKOUT_PREFIX; never saves or deletes level assets.
Refuses to run in an unexpected map or when any owned labels already exist.
"""
import os
import re
import unreal


def main():
    expected_map = os.environ.get("GPP_BLOCKOUT_MAP", "").strip()
    prefix = os.environ.get("GPP_BLOCKOUT_PREFIX", "").strip()
    if not re.fullmatch(r"[A-Za-z][A-Za-z0-9_]*", expected_map):
        raise RuntimeError("Specify the actual current map's asset name in GPP_BLOCKOUT_MAP.")
    if not re.fullmatch(r"GPP_[A-Za-z][A-Za-z0-9_]*_", prefix):
        raise RuntimeError("Use an owned label prefix, e.g. GPP_Training_.")

    # EditorLevelLibrary is deprecated; use the installed Editor subsystem.
    world = unreal.get_editor_subsystem(unreal.UnrealEditorSubsystem).get_editor_world()
    if not world or world.get_name() != expected_map:
        raise RuntimeError("Wrong Unreal Editor world. Refusing level changes.")

    actors = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
    present = list(actors.get_all_level_actors())
    if any(a.get_actor_label().startswith(prefix) for a in present):
        raise RuntimeError("Found existing owned blockout actors; refusing duplicate edits.")

    cube = unreal.EditorAssetLibrary.load_asset("/Engine/BasicShapes/Cube.Cube")
    if not cube or not isinstance(cube, unreal.StaticMesh):
        raise RuntimeError("The installed Engine Cube mesh is missing.")

    # All dimensions are centimetres. Each Actor uses a real 100 cm Engine cube.
    # Floor top Z=0; a 200+ cm wide corridor stays open until the game's
    # actual progression gate Actor is authored and integrated by its owner.
    structures = [
        ("PlazaFloor",       (0, 0, -10),       (8.0, 6.0, 0.2)),
        ("WalkwayFloor",     (700, 0, -10),     (6.0, 2.2, 0.2)),
        ("RecoveryFloor",    (1200, 0, -10),    (4.0, 5.0, 0.2)),
        ("EntryWallNorth",   (370, 210, 120),  (0.6, 2.0, 2.4)),
        ("EntryWallSouth",   (370, -210, 120), (0.6, 2.0, 2.4)),
        ("WalkwayWallNorth",(700, 130, 60),   (6.0, 0.4, 1.2)),
        ("WalkwayWallSouth",(700, -130, 60),  (6.0, 0.4, 1.2)),
    ]

    created = []
    for name, location, scale in structures:
        label = prefix + name
        actor = actors.spawn_actor_from_class(
            unreal.StaticMeshActor,
            unreal.Vector(*location),
            unreal.Rotator(0.0, 0.0, 0.0),
        )
        if not actor:
            raise RuntimeError(
                f"Spawn failed at {label}. Map may contain partially created "
                "actors; preserve and inspect, do not auto-clean.")
        actor.set_actor_label(label)
        component = actor.get_component_by_class(unreal.StaticMeshComponent)
        if not component:
            raise RuntimeError(f"StaticMeshComponent missing in {label}.")
        component.set_static_mesh(cube)
        actor.set_actor_scale3d(unreal.Vector(*scale))
        created.append(label)

    print("AUTHORED real level actors:", created)
    print("UNSAVED map changes: the sole Editor operator must inspect and save only owned packages.")
    print("NOT DONE: gate progression, interactive fragments, collision QA, human PIE, packaging.")


if __name__ == "__main__":
    main()
