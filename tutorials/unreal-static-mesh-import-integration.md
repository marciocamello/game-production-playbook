# T13 — Unreal Engine 5.9 Static Mesh Import, Materials, Collision and Actor Integration

**Status:** Published end-to-end technical procedure with an original [guarded Unreal Editor Python import script](../examples/unreal-import-static-mesh/README.md). **No Engine import, material asset, Actor placement, PIE or packaged-game check was run by this repository**. A valid import must be verified on the installed Unreal Engine 5.9 build.

**Goal:** take an actual authorized mesh export from T11, produce a real saved Static Mesh in the correct Content namespace, assign materials and collision, integrate it as a level Actor without overwriting existing gameplay identity, and prove camera/traversal correctness.

## 1. Preflight: inspect actual Editor content

Before importing, record:

- Installed Engine build, current level and plugin/importer availability.
- Existing source meshes and imported assets with matching IDs, including local untracked/ignored content.
- Dirty `.uasset`/`.umap` packages and their owners; one exclusive Editor/MCP operator.
- Source file/hash, license, Blender/exporter version, dimensions, pivot, UV and material slot expectations.
- Existing Material parents/instances, mesh variants and whether a Blueprint currently holds gameplay logic/collision.
- Required camera distance, Character capsule, landmarks, World Partition layout and target platform.

If the requested mesh already exists and meets requirements, **reimport/repair under authorization** instead of creating a duplicate with a suffix.

## 2. Use the FBX importer deliberately

Epic's documented static-mesh FBX pipeline supports multiple UV sets, materials, vertex colors, LODs and custom collision. It also warns that not every source material property is translated into a finished Unreal material graph. A successful FBX import may still omit correct roughness, metallic, AO and shader logic.

Manual procedure in Content Browser:

1. Select an **approved existing destination** Content folder and click **Import**.
2. Select an existing `.fbx` with known rights. Check the importer chose **Static Mesh**, not Skeletal Mesh.
3. Review import scale, axis conversion, normals/tangents, UV/lightmap options, smoothing, LODs and material import mode. Prefer referencing/reusing existing project materials over generating redundant per-mesh graphs.
4. If the project has custom `UCX_` collision geometry, verify that it matches the source mesh name exactly and remains convex as required.
5. For the T11 modules, import **one mesh per FBX** to maintain stable asset boundaries.
6. Save the imported Static Mesh, then reopen it and inspect actual bounds, geometry, UV, material slots and collision visualization.

**Representative bounds check:** T11's floor is modeled as **2 × 2 × 0.2 metres**. The imported mesh should measure approximately **200 × 200 × 20 Unreal centimetres**, using the verified exporter/importer convention. Do not assume Blender scale multiplication without inspecting real bounds.

## 3. Alternative: guarded Python import of one new mesh

The original [Editor Python import](../examples/unreal-import-static-mesh/scripts/import_one_static_mesh.py) uses:

- `unreal.AssetImportTask` plus `AssetToolsHelpers.import_asset_tasks`;
- `unreal.FbxImportUI` configured as Static Mesh with disabled automatic material/texture creation;
- `EditorAssetLibrary.does_asset_exist`, asset type checks and saving the new package;
- Refusal to overwrite any existing target and refusal to import into a nonexistent destination.

It reads the real FBX path/destination/name from environment variables already supplied to the Editor process. The script **does not** change the level, create guessed assets, delete existing objects or declare runtime PASS. If the actual UE 5.9 installation exposes a different import path (e.g. Interchange), inspect those APIs and adapt the import method rather than claiming a legacy import succeeded.

## 4. Geometry/UV/material quality in Static Mesh Editor

Open the imported asset and verify:

| Inspection | Expected observation |
| --- | --- |
| Dimensions | Match measured source and game grid |
| Pivot | Base/attachment origin supports intended placement |
| UV0 | No severe stretching, seams/tiling appropriate |
| Optional lightmap channel | Valid layout if target lighting/baking requires it |
| Normals/tangents | No inverted faces or unacceptable seam artifacts |
| Material slots | Real, stable slot indices matched by existing materials |
| Vertex colors | Imported only where authored and required |
| LOD/Nanite | Selected by performance/platform needs, not blindly enabled |
| Collision | Simple hull/collision shape fits the walkable game geometry |
| Import report | Explicit warnings and real source path recorded without leaking private paths publicly |

The T11 Blender material is a **numeric Principled BSDF**. It is not automatically a game-ready Unreal PBR Material. Create or reuse a Material parent with actual Base Color, Roughness, Metallic and optional normal/emission inputs; configure and assign a real Material Instance in Editor.

For a restored landmark, the authoritative progression state decides whether the lights/emissive/mesh variant and collision are active. A Material Instance parameter cannot independently donate abilities or unlock progression.

## 5. Collision design is gameplay, not a cosmetic importer option

Use the smallest **real collision** that satisfies gameplay. For a 2 m wall and floor:

- Floor should support normal CharacterMovement. Walkable top surface requires collision.
- Wall should block the player only where its physical body exists.
- Decorative projecting pieces can be nonblocking, but camera occlusion is a separate channel decision.
- Do not create a huge invisible box blocking an entire doorway or traversable arch.
- Verify collision complexity, normal walkability, stairs/slopes and camera boom retraction in PIE.
- If a bridge/corridor opens after restoration, **authoritative game state** changes route collision, not an animated mesh alone.

If importing custom UCX geometry, use proper matching names and **convex** pieces. Avoid “Use Complex as Simple” for simulated/dynamic meshes by default; inspect Engine limitations and your actual Actor mobility.

## 6. Integrate a real Actor without destroying gameplay identity

A static mesh is **not** the whole gameplay Actor. If the real level already has a fountain/gate/pickup controller Actor with a stable progression ID, keep that Actor intact and replace or attach only the authorized **visual component**.

Editor integration sequence:

1. Identify the existing Actor/Blueprint and its persistent ID, world transform, collision and references.
2. Locate the authorized asset's native package; drag into the scene **only if** an existing gameplay Actor does not already own that object.
3. Set transform/pivot/scale so the mesh snaps cleanly to adjacent modules at the approved grid.
4. Apply the correct Material Instance to each slot and inspect light/reflection behavior.
5. Configure simple collision, overlap triggers and camera response separately.
6. On a stateful Actor, wire the **real progression owner** to update visual/collision state idempotently on initialization/load/stream-in.
7. Keep presentation (Niagara/Sequencer/audio) distinct from committed state: skipping visuals must not undo a restored route.
8. Save **only owned** assets/map packages and reopen/reload to verify persistence.
9. Test a real player route in PIE and a fresh packaged process.

**Never edit `.uasset`/`.umap` binary bytes directly.** Use Unreal Editor or verified editor-native scripting/MCP.

## 7. Reimport and provenance

When the Blender owner corrects geometry, keep the same stable mesh identity where possible. Confirm the existing importer source path and material slots, and decide whether **reimporting this owned mesh** would overwrite human-authored collision/paint changes. Take the authorized path, preserve the old revision when necessary, then run affected tests again.

Record: original source/license, Blender `.blend`, FBX/hash, source dimensions, UE asset package, importer version, material dependencies, responsible owner, collision strategy, tests, outcome and any human acceptance.

A concept picture, a Blender `.blend`, an FBX, a saved Static Mesh and a working placed Actor are **five different levels of evidence**.

## 8. Acceptance matrix

| ID | Observable result | Executor |
| --- | --- | --- |
| IMP-01 | Source FBX exists and provenance is valid | Art operator |
| IMP-02 | Authorized Editor target/map/owner identified | Editor operator |
| IMP-03 | No target asset overwritten, ownership respected | Editor operator |
| IMP-04 | Import task or manual importer succeeds | Editor operator |
| IMP-05 | Correct Static Mesh package saved/reopened | Editor operator |
| IMP-06 | Correct bounds/pivot/forward orientation | Technical artist |
| IMP-07 | UVs and normals correct in mesh preview | Technical artist |
| IMP-08 | Real material parent/instance assigned | Material owner |
| IMP-09 | PBR roughness/metal/normal/light verified | Material owner |
| IMP-10 | Correct simple/UCX collision | Level artist |
| IMP-11 | Actor placed or visual component integrated | Editor operator |
| IMP-12 | Existing gameplay Actor/ID preserved | Gameplay owner |
| IMP-13 | State-driven variant and collision update | Gameplay/Editor |
| IMP-14 | Saved map reload retains mesh and materials | Editor operator |
| IMP-15 | Character walks on floor and stops at wall | Human PIE |
| IMP-16 | Camera remains readable near modules | Human PIE |
| IMP-17 | Streaming/revisit reprojects state without duplication | QA |
| IMP-18 | Reimport correction does not break unrelated assets | Editor operator |
| IMP-19 | Cook/package includes materials, collisions and meshes | Release owner |
| IMP-20 | Fresh standalone route plays correctly | Human tester |

All tests remain **NOT RUN** for the repository's tutorial publication.

## 9. Failure containment

| Issue | Corrective action |
| --- | --- |
| FBX imports at 100× size | Compare source dimension, exporter axis/scale and UE bounds numerically |
| Missing roughness/normal after import | Reuse/create real Unreal Material graph; inspect texture color space |
| Doorway is blocked by invisible mesh | Adjust authored simple collision and test capsule movement |
| Camera contracts against decorative stone | Inspect Camera collision response and Spring Arm probe from T08 |
| Imported mesh exists but Actor remains old placeholder | Integrate the actual owned visual component; do not duplicate gameplay Actors |
| Reimport overwrites a human-edited material slot | Stop and assign reconciliation to asset owner |
| Source file is unlicensed or private | Block redistribution, trace provenance and obtain permitted replacement |
| Import operation writes multiple assets unexpectedly | Stop; inventory actual created paths; no blind delete/reset |
| Package works in Editor but fails standalone | Verify cooked references/materials and report real runtime defects |

Next: [T14 — Playable Level Design, Modeling Mode and Geometry Script](README.md).

**Official references:** [Epic FBX Static Mesh Pipeline](https://dev.epicgames.com/documentation/unreal-engine/fbx-static-mesh-pipeline-in-unreal-engine), [FBX import options](https://dev.epicgames.com/documentation/unreal-engine/fbx-import-options-reference-in-unreal-engine), [Unreal 5.9 asset migration import](https://dev.epicgames.com/documentation/unreal-engine/migrating-assets-from-unity-to-unreal-engine), [Unreal Python API](https://dev.epicgames.com/documentation/en-us/unreal-engine/python-api/class/FbxImportUI?application_version=5.6).
