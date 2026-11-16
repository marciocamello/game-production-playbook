# T11 Example — Real Blender Modular Stone Geometry

This example contains a **nonstub, destructive-safe Blender Python generator** under [scripts/build_modular_kit.py](scripts/build_modular_kit.py). When actually executed in an **isolated background Blender process**, it creates:

- A native Blender `.blend` master containing three actual bevelled Stone modules.
- A 2 m floor slab, a 2 m wall, and a 0.35 m pillar with **base-centered pivots**.
- Real UV unwraps using Blender's `smart_project` and a Principled BSDF material with assigned Base Color/Metallic/Roughness values.
- Three individual FBX exports plus a JSON manifest with measured triangle counts, bounds, material slots and SHA-256 file digests.

**Publication evidence boundary:** The generator's Python source is present and was inspected; this repository has **not executed Blender**, inspected produced meshes in Blender, imported the FBX files into Unreal or played a game. No output `.blend`, FBX, manifest or other asset is claimed to exist at publication. These files are created only when the user runs the script.

## Execute with an actual Blender installation

Choose an empty/new output directory under your **own** writable art workspace. The script refuses any output directory that already exists, including an empty one; it never overwrites source art.

```powershell
# From the playbook repository checkout, after selecting a new absolute path:
# Replace $env:GPP_NEW_ASSET_OUTPUT with your real, unused output folder.
blender --background --factory-startup `
  --python ./examples/blender-modular-kit/scripts/build_modular_kit.py `
  -- --out "$env:GPP_NEW_ASSET_OUTPUT"
```

The Blender executable must be installed and available in PATH. The parent directory of the requested destination must already exist. Do not run this script inside an active Editor/Blender session: it deliberately resets **its own newly created background Blender process** to an empty scene.

## Where the implementation stops

- **Source file created** means the .blend exists and opens.
- **Export verified** means the FBX files exist and manifest dimensions/UVs/materials are inspected; no validation of Unreal importer compatibility is implied.
- **Imported** requires a real Unreal Editor import and measured dimensions in centimetres.
- **Integrated** requires actual level placements, collision and material assignments.
- **Runtime tested** requires actual PIE and the game's capsule/camera.
- **Human validated** requires an authorized gameplay review.

This exercise does **not** create normal maps, authored albedo texture images, collision hulls, LODs or a final photogrammetric stone asset. The Principled BSDF **numeric material inputs** constitute actual physically based parameters, not baked PBR texture maps.

See [T11 — Blender Modeling, UV, PBR and Export](../../tutorials/blender-modeling-uv-pbr-export.md), [T13 Unreal Import](../../tutorials/README.md) and [third-party rights](../../THIRD_PARTY.md).
