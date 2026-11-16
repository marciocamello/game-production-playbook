"""Import one EXISTING FBX Static Mesh into an approved NEW Unreal asset path.

Run only inside the real Unreal Editor Python environment as the sole Editor owner.
Reads GPP_FBX_SOURCE, GPP_UNREAL_DEST, GPP_ASSET_NAME from environment.
Never replaces assets, changes a map, or silently deletes imported data.
"""
import os
from pathlib import Path
import re

import unreal


def required_env(key):
    value = os.environ.get(key, "").strip()
    if not value:
        raise RuntimeError(f"Required environment variable missing: {key}")
    return value


def main():
    source = Path(required_env("GPP_FBX_SOURCE")).expanduser().resolve()
    destination = required_env("GPP_UNREAL_DEST").rstrip("/")
    asset_name = required_env("GPP_ASSET_NAME")

    if not source.is_file() or source.suffix.lower() != ".fbx" or source.stat().st_size == 0:
        raise RuntimeError("An existing nonempty FBX source file is required.")
    if not destination.startswith("/Game/") or ".." in destination:
        raise RuntimeError("Destination must be an approved /Game/ content folder.")
    if not re.fullmatch(r"[A-Za-z][A-Za-z0-9_]*", asset_name):
        raise RuntimeError("Asset name must be a valid explicit Unreal identifier.")

    target = destination + "/" + asset_name
    if unreal.EditorAssetLibrary.does_asset_exist(target):
        raise FileExistsError("Refusing to replace existing Unreal package: " + target)
    if not unreal.EditorAssetLibrary.does_directory_exist(destination):
        raise RuntimeError("Create/approve the intended destination Content folder in Editor first.")

    if not hasattr(unreal, "FbxImportUI"):
        raise RuntimeError("Installed Editor lacks the expected FBX import API; inspect Interchange/manual workflow.")

    options = unreal.FbxImportUI()
    options.set_editor_property("automated_import_should_detect_type", False)
    options.set_editor_property("mesh_type_to_import", unreal.FBXImportType.FBXIT_STATIC_MESH)
    options.set_editor_property("import_as_skeletal", False)
    options.set_editor_property("import_materials", False)
    options.set_editor_property("import_textures", False)
    options.set_editor_property("import_animations", False)

    task = unreal.AssetImportTask()
    task.set_editor_property("filename", str(source))
    task.set_editor_property("destination_path", destination)
    task.set_editor_property("destination_name", asset_name)
    task.set_editor_property("automated", True)
    task.set_editor_property("replace_existing", False)
    task.set_editor_property("save", True)
    task.set_editor_property("options", options)

    tools = unreal.AssetToolsHelpers.get_asset_tools()
    tools.import_asset_tasks([task])
    created_paths = list(task.get_editor_property("imported_object_paths"))

    if len(created_paths) != 1:
        raise RuntimeError(
            f"Expected one imported Static Mesh, found {created_paths!r}. "
            "Inspect newly created assets; do NOT delete blindly.")

    asset = unreal.EditorAssetLibrary.load_asset(target)
    if not asset or not isinstance(asset, unreal.StaticMesh):
        raise RuntimeError(f"Imported target does not resolve to a StaticMesh: {target}")

    if not unreal.EditorAssetLibrary.save_loaded_asset(asset, only_if_is_dirty=False):
        raise RuntimeError("The new StaticMesh could not be saved.")

    print("IMPORTED AND SAVED:", target)
    print("SOURCE FBX:", source.name)
    print("NOT VERIFIED: dimensions, UV, materials, collision, level placement, PIE, packaging")


if __name__ == "__main__":
    main()
