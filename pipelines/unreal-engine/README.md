# Unreal Engine Production Pipeline

**Reference case:** The Unmade was developed against an installed Unreal Engine 5.9 environment. Version-sensitive toolsets, plugin APIs and preview behavior **must be inspected in the user's installed build**; this guide is not a guarantee that every operation exists in all versions.

## 01. Project foundation

Select a real project type (C++ + Blueprints or Blueprint-only) and configure input, map selection and target packaging platform. Inspect existing module dependencies and enabled plugins before adding copies of gameplay frameworks. Do not use a source Engine fork as the shipping build merely because source is available for API research.

Keep project-authored content in a coherent namespace inside the project's own Content folder. Only tools compatible with Unreal should create, rename or move `.uasset` and `.umap`.

## 02. Capability discovery: Editor, MCP, Canvas and Python

1. Identify the actual installed Engine version/build, project files and enabled plugins.
2. Confirm connection and capability of the **running Editor**, not only the `.uproject` plugin list.
3. Discover available MCP toolsets, descriptions and method schemas; do not invent a `Chaos`, `Fracture`, `AnimGraph` or other endpoint.
4. Use supported Editor asset/Blueprint/Sequencer/Modeling tools; fall back to verified Unreal Python, Editor Utility tools, C++ or human Editor operation when a toolset lacks a needed operation.
5. Use Unreal Canvas as an optional visual collaboration/annotation workflow if installed and demonstrably useful. A documented/plugin-enabled Canvas feature does not automatically mean it was used to author runtime assets.
6. Record API inputs, actual outputs and any missing operations.

Never operate Editor Python by directly writing binary packages. Avoid calling the full Engine source recursively as generic context; search targeted declarations where source access is lawful and available.

## 03. Gameplay authority

Use C++/Blueprint according to actual project needs. For a character with two abilities, GAS and Gameplay Tags can express availability and effects; a persistent subsystem or another explicit owner controls long-lived progression. Enhanced Input handles remappable actions. MVVM projects state into UMG, without allowing widgets to mutate the source-of-truth fields directly.

For streaming worlds, give progression-critical Actors stable IDs, commit state once and reapply it idempotently when cells or actors load. Separate `ApplyState` from `PlayTransition`; do not replay one-time visuals on Load.

**Hands-on GAS example:** [T05 — Gameplay C++, Gameplay Tags and GAS](../../tutorials/gameplay-gas-tags-light.md) includes original [C++ source for a Light ability, native Gameplay Tag, Character and powered beacon](../../examples/gas-light-demo/README.md), with an in-Editor integration guide and an explicit untested-runtime boundary. Reuse existing ASC/progression authority rather than duplicating it.

**T13 verified import and gameplay-aware Actor integration:** [Unreal Static Mesh Import, Materials, Collision and Actors](../../tutorials/unreal-static-mesh-import-integration.md) includes a [guarded one-FBX Editor Python import](../../examples/unreal-import-static-mesh/README.md). Imported `.uasset` existence never substitutes for verified material, collision, Actor wiring or PIE.

**T14 authored level and Geometry Script:** [Level Design, Modeling Mode and Geometry Script](../../tutorials/level-design-modeling-mode-geometry-script.md) with an optional [seven-Actor collision-bearing Unreal Editor blockout](../../examples/level-blockout/README.md). This creates authored structural geometry only when executed; the actual gameplay gate and persistence remain independent authorities.

## 04. Spatial and art integration

Approve visual concepts and spatial blockout reference before large-scale scene authoring when the style/route matters. Build actual paths with Modeling Mode, static meshes, collisions and checkpoints. Preserve gameplay Actors while refining scenery. Use Geometry Script and reusable materials for controlled geometric detail; PCG, Water, Nanite and Mesh Terrain are optional engineering choices subject to benefit, compatibility and release safety.

## 05. Cameras and UI

Test cameras against actual occluding walls, stairs and the real CharacterMovement capsule. Avoid masking a boom/retraction defect with arbitrary interpolation limits. Gameplay Cameras can support multiple rigs, but author and verify the real Camera Asset and input mapping; a C++ component alone does not provide the finished Editor asset.

Create UMG widgets bound to real native ViewModels, command APIs and input settings. Test menus and buttons after being imported, compiled and navigated; an MVVM class existing in source is insufficient.

**T08 camera and input tutorial:** [Camera Rigs, Occlusion & Enhanced Input](../../tutorials/camera-rigs-occlusion-enhanced-input.md) extends the **existing** [T05 Character](../../examples/gas-light-demo/README.md) with optional Zoom/Sprint Input Actions, frame-rate-independent boom interpolation, `ECC_Camera` collision telemetry and camera-return checks. It also documents the experimental Gameplay Cameras Camera Asset route as an alternative, without claiming the Editor asset exists until it is authored and tested.

**T09 functional HUD and menus:** [UMG/MVVM — HUD, Menus, Settings and Loading Boundaries](../../tutorials/umg-mvvm-hud-menus-settings-loading.md) provides a [four-file native MVVM and PlayerController example](../../examples/umg-mvvm-demo/README.md) reusing T07 progress/save events. Actual UMG assets, audio routing and Continue world restoration require explicit Editor integration and tests; do not claim success based on source alone.

## 06. Animation, audio, VFX and cinematic integration

Import tested FBX/GLB assets; verify skeleton rest poses and root units. Retarget and author animation graphs using supported Editor tools. Drive Niagara, sound, Sequencer and Chaos-assisted visuals from committed gameplay state, not as progression authorities. Keep bridge/fountain collisions deterministic and safe after skipped effects.

## 07. Local verification

Run scoped C++ build, native tests, Editor asset compile/validation, human PIE, Windows cook/package and **fresh packaged launch** as separate checks. Check test-case results, not only process exit status. Re-run progress/save/streaming tests after changing Actor logic, UI or cinematic returns. Local script automation is permitted; **no GitHub Actions workflows** are created.

**Hands-on tutorials:** [T01 — Unreal Engine 5.9 MCP](../../tutorials/unreal-mcp-5-9.md) covers tool discovery, Editor identity, client integration and a verified lighting edit. [T02 — Project Bootstrap & Local Build](../../tutorials/unreal-project-bootstrap-local-build.md) covers project targets, module dependencies, Game Default Map, actual [PowerShell UBT/UAT wrappers](../../examples/unreal-local-build/README.md), Windows packaging and human standalone verification.

**T06 practical progression:** [Contextual Interactions, Fragments and Voluntary Sacrifice](../../tutorials/contextual-interactions-pickups-sacrifices.md) extends the real T05 GAS baseline into target selection, LOS checks, stable pickup IDs, hold confirmation and a distinct recovery route. It does **not** claim the code has already been integrated or played.

**T07 persistence implementation:** [SaveGame, Reload and World Partition](../../tutorials/savegame-reload-world-partition.md) adds an [original C++ A/B slot example](../../examples/savegame-ab/README.md), stable identities, logical-world reapplication, streaming checkpoints and fresh-process acceptance. Sample source is not claimed to have been compiled or integrated in a live game.

See [Feature Execution Workflow](../../docs/06-production-planning/feature-execution.md), [Blender pipeline](../blender/README.md), and [The Unmade case study](../../case-studies/the-unmade/README.md).