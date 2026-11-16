# T11 — Blender Modeling, Retopology, UV and PBR: Build and Export a Real Modular Kit

**Status:** Published technical walkthrough plus an [original executable Blender Python generator](../examples/blender-modular-kit/README.md) for actual geometry, UV unwrap and FBX export. **No Blender generation, Blender→Unreal import, PIE or packaged-game check has been performed by this repository.** Run and report each stage independently.

**Target:** Blender with the documented 4.5+ Python API and Unreal Engine 5.9. Verify both installed versions before adapting the scripts. The worked module is a *new, original educational stone kit*, not a copy of any game asset.

## 1. Use approved gameplay and visual contracts

Read [T10 style/brief](visual-references-generation-ready-briefs.md) and the project's current source/asset inventory. Inspect existing mesh names, Blender file ownership, UV/material source, grid scale, importer settings and Unreal actor collision. If a working modular wall already exists, **repair/reuse it** rather than replacing it with new sample geometry.

Agree on one 2 m grid and a collision/camera requirement. Distinguish a 3D reference image, editable source mesh, correct FBX export, engine import and playable actor.

## 2. Model three pieces with actual Blender geometry

The [source script](../examples/blender-modular-kit/scripts/build_modular_kit.py) creates a real `GPP_Modular_Stone_Original.blend` and three distinct FBX files **only after execution**.

| Piece | Source dimensions (metres) | Actual purpose |
| --- | --- | --- |
| Floor slab | 2 × 2 × 0.2 | A reusable tile with top surface and base pivot |
| Wall | 2 × 0.3 × 2.4 | A blocker whose camera response must be inspected |
| Pillar | 0.35 × 0.35 × 2.8 | A repeatable architectural landmark or structural accent |

The script creates cubes, applies physical dimensions, translates vertices to keep the object **origin at base center**, applies a real bevel modifier, unwraps actual mesh faces and assigns one real shader material. It writes each mesh as FBX from selected objects only; a manifest captures measured triangles, dimensions, UV counts and SHA-256s.

A source file with three primitives is a **functional modular geometry exercise**, not a claim of high-end finished environment art. A production feature still needs the game's approved silhouette, collision and material quality.

## 3. Run in an isolated process and preserve authored files

Do not execute a bulk reset/cleanup in an artist's active Blender scene. The script requires `bpy.app.background` and only resets the new background Blender process. It refuses existing output directories, so no previous artist's source or export can be silently replaced.

```powershell
# $env:GPP_NEW_ASSET_OUTPUT must be set to a NEW path whose parent exists.
blender --background --factory-startup `
  --python ./examples/blender-modular-kit/scripts/build_modular_kit.py `
  -- --out "$env:GPP_NEW_ASSET_OUTPUT"
```

The output contains actual `.blend` and FBX files **only after successful execution**. Inspect the terminal log and generated `manifest.json`; if Blender reports an error or a file is missing, the operation **FAILED** even if a partial directory was created. Preserve partial output and avoid blind retries into the same path.

## 4. Retopology and mesh repair for real incoming assets

A raw generator mesh may have open boundaries, disconnected shells, severe triangle density, inverted normals, excessive thin parts, self-intersections or embedded meshes. **Retopology is conditionally necessary**, not automatic.

For an actual incoming source:

1. Back up/branch ownership of that *specific* source file. Open the real mesh and inspect dimensions/units and modifiers.
2. Find disconnected islands/non-manifold edges, degenerates, overlapping duplicates, inward normals and unneeded geometry.
3. Decide whether the mesh is **structural modular** (rebuild with clean quad/tri surfaces and stable borders) or organic (manual retopo / Shrinkwrap / targeted remesh).
4. Preserve silhouette and hard structural edges; compare normal directions and shading under a representative raking light.
5. Avoid destructive decimation of skeletal deforming meshes or structural collision surfaces. Measure face reduction and check deformation/traversal afterward.
6. Reunwrap, inspect seam distortion and assign stable material slots. Do a representative export/reimport test before processing the whole asset family.

If the source is already clean and performant, **skip retopology** and record why. Do not claim the T11 generator itself performs retopology; it creates new clean modular objects.

## 5. UVs and PBR: color-space discipline

The generator uses Blender's actual **Smart UV Project** operator. For authored hero assets, seam-based unwrapping and consistent texel density are often preferable. Inspect stretching, flipped islands, overlap policy and padding with a UV grid in the real source file.

A physically based material can use numeric inputs or maps:

| Data | Usual interpretation | Verification |
| --- | --- | --- |
| Base Color | Color (sRGB for ordinary color textures) | Compare visual value under controlled light |
| Roughness | Non-color/linear | Smooth/rough surfaces respond coherently |
| Metallic | Non-color/linear | Metallic areas reflect the environment appropriately |
| Normal | Non-color with correct tangent-space convention | No inverted bumps, harsh seams or channel errors |
| AO | Non-color/linear | Does not double-darken excessive baked shadow |
| Emission | Color plus intensity | Gameplay state determines activation |

**Actual script output:** a Principled BSDF with numeric Base Color, Metallic=0 and Roughness=0.82. The script does **not** bake/claim normal maps or image textures. In Unreal, implement equivalent material parameters in real Material/Material Instance assets; FBX material conversion is not automatically equivalent to a authored PBR graph.

## 6. Scale, pivots and FBX round trip

Blender metric dimensions are in **metres**; Unreal level geometry uses centimetres. A 2 m slab should measure **200 Unreal units** after the chosen importer/FBX transformation path. Check real imported asset bounds, not a presumed export conversion.

- Inspect applied object transforms and base-centered pivot.
- Export one Static Mesh per FBX; preserve stable mesh identity.
- The script uses `axis_forward='-Y'`, `axis_up='Z'`, `apply_unit_scale=True`, `global_scale=1.0`.
- Verify forward rotation and world units in **the actual installed UE 5.9 importer**. Blender-to-FBX conversion and importer settings may differ; do not multiply everything by 100 by habit.
- Import first into a dedicated owned content area via Unreal Editor; verify mesh bounds, UV and material slots before batch import.
- Use authored simple collision that matches real traversability; avoid huge invisible bounding boxes around decorative trim.

FBX export is not Blender source, and an `.fbx` file existing does not prove it imports without warning.

## 7. Modeling quality gates

| ID | Real operator check | Expected evidence |
| --- | --- | --- |
| MOD-01 | Run script in isolated Blender process | Actual exit result, source created without overwrites |
| MOD-02 | Open .blend | Three real named mesh objects, one PBR material |
| MOD-03 | Inspect dimensions | Floor 2×2×0.2 m; wall 2×0.3×2.4 m; pillar 0.35×0.35×2.8 m |
| MOD-04 | Inspect pivots | Base-center origin and correct modular joint positions |
| MOD-05 | Inspect UV editor | Real nonempty UV layer for each mesh, sensible islands |
| MOD-06 | Inspect geometry | Valid normals, no obvious non-manifold/degenerate faces |
| MOD-07 | Inspect PBR nodes | Principled BSDF parameter values actually set |
| MOD-08 | Inspect FBX/manifest | All three exports exist, nonzero bytes and matching checksums |
| MOD-09 | Import in installed Unreal | Real Static Meshes exist, inspect actual unit conversion |
| MOD-10 | Verify Material Instances | Actual shading/roughness under target map lighting |
| MOD-11 | Place snapped modules | No unintended gaps/overhang/rotation offsets |
| MOD-12 | Test collision and camera in PIE | Player cannot pass through wall and can walk on floor |
| MOD-13 | Reimport corrected single source | Changes propagate without replacing unrelated placed actors |
| MOD-14 | Record rights/provenance | Original generated code/outputs, Blender version and author |
| MOD-15 | Package a representative scene | No missing cooked mesh/material; visible in fresh launch |

All tests are **NOT RUN** in this publication; the source code is present, but no generated artifact is claimed.

## 8. Failure recovery

- 100× scale mismatch: inspect source metre bounds, export scale and importer conversion **as measured**, then reexport one representative piece.
- Invalid shading: inspect normals, UVs and imported material; do not “fix” structural geometry solely through Roughness.
- Missing UV: re-open source; verify unwrap operator completed, **not** only FBX filename.
- Source/generated asset collision: keep outputs in distinct paths and one operator per file.
- Unlicensed source images or meshes: block import/redistribution, record rights, use original geometry.
- Huge polycounts: inspect triangle count, prioritize silhouette-preserving retopo/LOD and target platform measurements.
- Unreal import PASS but gameplay blocked: correct collision and modular assembly in the authored level, not the FBX export log.

## 9. Handoff and next

The Blender owner delivers **the real source file, FBX and manifest** with output tool/version and QA. The Unreal Editor owner imports it and records exact package IDs. Human PIE checks camera and collision. No agent overwrites a private mesh or declares asset validation without a real run.

Next: [T12](README.md) audits skeletons, animation scale and IK retarget. [T13](README.md) covers static-mesh import/material/actor integration. [T14](README.md) combines source assets and Modeling Mode into a playable real level.

**Official references:** [Blender 4.5 FBX operator](https://docs.blender.org/api/4.5/bpy.ops.export_scene.html), [Blender UV tools](https://docs.blender.org/manual/en/5.0/modeling/meshes/editing/uv.html), [Blender Bevel](https://docs.blender.org/manual/en/5.1/modeling/modifiers/generate/bevel.html), [Epic FBX pipeline](https://dev.epicgames.com/documentation/unreal-engine/fbx-static-mesh-pipeline-in-unreal-engine).
