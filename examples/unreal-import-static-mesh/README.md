# T13 Example — Guarded Real Static Mesh Import into Unreal Editor

This [Python script](scripts/import_one_static_mesh.py) uses Unreal's **actual Editor asset-import APIs**, not a fabricated import function. It takes one **existing** nonempty FBX file and creates **one new** Static Mesh in an explicitly approved Unreal Content folder.

**Status:** Original code is published, but it has **not been executed in Unreal Engine 5.9**. The script may need adjustments for the **actual installed Editor importer**, which can use legacy FBX or an Interchange pipeline depending on version/settings. No Unreal `.uasset`, material, Blueprint or level has been created by this documentation repository.

## Use with an authorized Editor owner

1. Run [T11](../../tutorials/blender-modeling-uv-pbr-export.md) and verify a **real** FBX exists, or provide another licensed FBX source.
2. In the actual Unreal Editor, inspect unsaved packages and conflicts; ensure you are the sole operator.
3. Create/approve the destination folder in the Editor (under your project's Content).
4. Configure three environment variables **before launching Unreal Editor**:

   - `GPP_FBX_SOURCE`: full path to an existing, nonempty FBX.
   - `GPP_UNREAL_DEST`: the **existing** Unreal folder path, for example `/Game/Core/Architecture/Modules` in a project whose owner uses that namespace.
   - `GPP_ASSET_NAME`: a unique expected asset name, such as `SM_GPP_Floor_200`.

5. In the real Editor, enable the built-in Python Editor Script Plugin when necessary. Execute the script through the Editor Python console or the supported Python Script menu. Verify the actual 5.9 API schema; never fabricate tool names.
6. Inspect **Output Log**, the imported object path and whether the asset is a saved **Static Mesh**. The script fails if the target package already exists and refuses to delete/replace it.
7. Open the Static Mesh Editor and manually verify measured bounds (e.g. 200 × 200 × 20 cm for the T11 floor), UV channel(s), shading, material slots and intentional simple/complex collision.
8. Assign an **existing** Material Instance or author a real Material in the Editor. Place the mesh into the intended level only under its actual map owner, and test Character capsule collision and camera.
9. Save the **owned** map/material packages; run PIE, cook/package and fresh-process gameplay at the required feature gate.

**Important:** `imported_object_paths` is an import result, not proof of a good mesh. The script enforces one imported path and never removes surprise extra assets automatically. A successful `save_loaded_asset` does not prove collision or gameplay.

## Limitations

This script intentionally **does not** implement reimport/overwrite, material graph creation, collision repair, actor replacement, Nanite/LOD policy or scene placement. Those are actual Editor-owned steps described in [T13](../../tutorials/unreal-static-mesh-import-integration.md), not empty stubs or claims of completion. Use a supported Editor UI workflow if the installed import plugin does not expose the expected Python types.

It is original educational source licensed under [MIT](../../LICENSE-CODE.md). Imported external FBX files retain their creators' terms.
