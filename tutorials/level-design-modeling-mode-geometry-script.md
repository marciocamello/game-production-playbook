# T14 — Level Design, Modeling Mode and Geometry Script: From Traversable Blockout to Authored Level

**Publication state:** An end-to-end Unreal Engine 5.9 level-design procedure plus [original guarded Editor Python blockout source](../examples/level-blockout/README.md). **No actual map, Static Mesh, Geometry Script Blueprint, PIE route or package has been created/tested by this documentation publication.** Tests begin NOT RUN.

**Objective:** build a playable sequence of a plaza, choke point, crossing and recovery area, check camera/capsule traversal, integrate one **real existing progression gate**, and convert only justified geometry to final authored Static Meshes using Modeling Mode and (optionally) Geometry Script.

## 1. Turn the approved critical route into real spatial constraints

A landscape reference is not a level. Start with the approved GDD, [T10 visual design](visual-references-generation-ready-briefs.md), [T06 interaction](contextual-interactions-pickups-sacrifices.md), [T07 state](savegame-reload-world-partition.md), [T08 camera](camera-rigs-occlusion-enhanced-input.md) and actual Character capsule dimensions.

Record entrances/exits, all mandatory interaction points, donation cost, recovery path, which paths are allowed/blocked before progress, and where the player regains a needed ability. A sacrifice must never strand the player in an area that requires the sacrificed ability to escape.

Plan a dependency graph:

```text
Entry plaza
    -> find/inspect required part
    -> reach actual restoration actor
    -> voluntarily confirm donation
    -> authoritative gate commits OPEN
    -> physically traversable crossing
    -> separate recovery fragment
    -> next landmark / safe checkpoint
```

Draw route topology and elevation before detail modeling. No art polish can repair a softlock caused by mandatory wrong-way progression.

## 2. Author a real minimal spatial shell in Unreal

Option A: **manual Unreal Modeling Mode**. Option B: the [guarded Editor Python script](../examples/level-blockout/scripts/author_walkable_blockout.py) that creates seven real Engine-cube architectural actors on the *already approved* loaded level. Both produce actual level content only after execution.

The example geometry (centimetres):

| Real structural element | Approximate dimensions | Purpose |
| --- | --- | --- |
| Plaza floor | 800 × 600 × 20 | Start and approach space |
| Crossing floor | 600 × 220 × 20 | Narrow readable traversal |
| Recovery platform | 400 × 500 × 20 | Room to collect distinct recovery fragment |
| Two entry wall segments | Each 60 × 200 × 240 | Narrow focal opening, with gap suitable for testing |
| Two walkway side walls | Each 600 × 40 × 120 | Guide crossing without invisible barriers |

These dimensions are **educational defaults**; compare against the real Character capsule and camera. The script uses an actual Engine Cube mesh, creates seven `AStaticMeshActor` instances, validates the loaded map and avoids duplicate labels. It intentionally does **not** spawn a fake gate, pickup or quest controller; those must be existing implemented gameplay Actors or authorized real new features.

## 3. Modeling Mode: from structural primitives to authored meshes

Epic's Modeling Mode can create and edit real in-engine geometry. In UE 5.9:

1. Enable **Modeling Tools Editor Mode** if required, then open **Select Mode → Modeling** (or Shift+5 where available).
2. In the **Create** category, make real Box primitives for platform, wall and support components, using **actual numeric dimensions**.
3. In the Modeling Mode Quick Settings, confirm the output asset destination is an **approved Content namespace**, not a temporary or unrelated folder.
4. Use Group/PolyGroup tools where needed and **PolyEdit** / Extrude / Inset to create a real recess or edge detail. Preserve structural grid alignment.
5. Use Transform/pivot controls and the grid to ensure modules meet at consistent 200 cm increments where that is the selected design.
6. UV/edit only where surfaces actually require texture treatment. Assign a real existing Material or Material Instance rather than generating a new material per wall.
7. Configure collision through Static Mesh Editor or Modeling tools, verifying that the player's physical path remains correct.
8. **Accept** each finished Modeling operation and explicitly save the mesh and level packages. An unaccepted Modeling tool preview is not an authored saved mesh.
9. Reload the map; compare asset dependencies and Actor transform/collision. Decorative changes must not delete the existing restoration Actor or stable IDs.

Manual Modeling can produce **final assets** where geometry quality and gameplay justify it. A geometry primitive is not automatically a disposable placeholder.

## 4. Geometry Script: optional real procedural authoring

Epic documents Geometry Script as a **Beta** suite built on `UDynamicMesh` and Geometry Script Blueprint/Python functions. It is useful when repeated stairs, wall trims, curved paths or repairable architecture really benefit from parameterized generation.

To implement a genuine authoring tool in the Editor:

1. Enable the **Geometry Script** plugin and verify its 5.9 support.
2. Create a **real Blueprint subclass of GeneratedDynamicMeshActor** in the approved Content folder.
3. In its graph, implement **Event On Rebuild Generated Mesh** and access the actual Target Dynamic Mesh.
4. Use **Append Box** / **Append Linear Stairs** or other *discovered real* Geometry Script nodes to build the approved dimensions. Configure transform, Polygroups/material IDs and valid bounds as required.
5. Place the actor in an owned editor level; modify a numeric parameter and verify it **rebuilds real geometry**. No stub function or empty event graph qualifies.
6. If static production geometry is intended, use the supported Editor **Create New Static Mesh Asset From Mesh** conversion and save the resulting real Static Mesh package; do not assume runtime Dynamic Mesh supports every Nanite/LOD workflow.
7. Re-test collision/material slots/UV and the final Character route. Ensure the generator does not overwrite an existing mesh and the gameplay progress remains owned outside geometry.
8. If Geometry Script does not improve a genuine asset, skip it. T14 does not require it for a simple, playable 7-piece blockout.

An Editor-authored Generator Blueprint is a **tool**. It is not evidence that a functional game level, stateful gate or full World Partition route has been integrated.

## 5. Occlusion, walkability and verticality

Test at the **actual possessed T08 camera** and CharacterMovement capsule:

- Corridor width and turning radius; no need to squeeze through visible stone.
- Floor connection and step height; a small seam must not be a fatal gap.
- Actual camera boom collision `ECC_Camera` near pillars and walls; distinguish intended occlusion from accidental retraction.
- Material value grouping near the interactable; the player must see the cost and next route.
- Stairs/ramp climb parameters; a level that only works in fly-around Editor camera is **not** playable.
- Bridge/crossing should have real collidable surfaces; visible animation does not grant crossing collision.
- Recovery fragment placed on a path accessible while the donated ability is unavailable.
- A safe checkpoint/spawn reachable after a fail/restart.

Test more than one camera zoom and movement speed; sprint can reveal collisions not seen when walking.

## 6. Real progression gate and persistent world projection

An actual feature needs a persistent gate/bridge/fountain controller built through the relevant gameplay SDD, not created as a fake geometric object in this tutorial. Its behavior:

1. Reject interaction when physical prerequisites are missing.
2. Cancel a partial hold with **no progress mutation**.
3. Commit a unique restoration transaction to the real authoritative service **once**.
4. Remove/deny the donated personal ability via GAS where required.
5. Update **actual bridge/gate collision** so the route becomes traversable.
6. Apply visible lights/mesh variants as a projection of committed state, not as the state owner.
7. On World Partition reload, apply the committed state idempotently; do not repeat one-time FX.
8. Ensure recovery exists on a reachable path, then permit continued gameplay.

The playbook's T05/T06/T07 examples are **educational and not yet integrated** into one tested game. Level authoring cannot silently mark their combined stateful route PASS.

## 7. Preserve World Partition and Actor identity

For a single continuous world, separate **always-needed progression controllers** from scenery and pickups that can stream if their logical state persists. Do not convert or replace a meaningful Actor with a purely decorative StaticMeshActor during level polish.

Use stable IDs, authored actor placement and explicit conversion ownership. Before a major geometry replacement, record current Actor paths/IDs, collision responses, level/data-layer membership and dirty packages. Save only authorized content with one active Unreal Editor/MCP operator.

Modeling Mode asset creation, Geometry Script output and Blender imports each have distinct source ownership. A new mesh is not permission to overwrite the level.

## 8. Actual acceptance matrix

| ID | Real check | Evidence |
| --- | --- | --- |
| LVL-01 | Approved route/dependency plan exists | Human GDD/feature approval |
| LVL-02 | Actual map/owner/dirty packages inspected | Editor preflight |
| LVL-03 | Real floor/wall assets authored and saved | Map/mesh packages |
| LVL-04 | All seven scripted blockout Actors present (if using script) | Editor Actor inventory |
| LVL-05 | Grid, units and pivot correct | Transform/bounds |
| LVL-06 | Floor is physically walkable | PIE Character |
| LVL-07 | Wall actually blocks and camera remains readable | PIE collision/camera |
| LVL-08 | Corridor is traversable without clipping | Human traversal |
| LVL-09 | Recovery platform reachable from crossing | PIE |
| LVL-10 | Generated Geometry Script Blueprint rebuilds real mesh (only if adopted) | Rebuild evidence |
| LVL-11 | Modeling Mode changes are accepted and saved | Reloaded assets |
| LVL-12 | Geometry Script to Static Mesh conversion works if selected | Saved asset |
| LVL-13 | Existing gate controller/unique ID preserved | Actor comparisons |
| LVL-14 | Unauthorized early crossing remains blocked when gate exists | PIE |
| LVL-15 | Voluntary donation properly opens **physical** route | Gameplay state + collision |
| LVL-16 | Recovery requires no donated ability | Anti-softlock PIE |
| LVL-17 | Cancelled/repeated donation has no extra effect | Invariant checks |
| LVL-18 | World Partition stream-out/in preserves gate/fragments | Streaming test |
| LVL-19 | Save/load returns to safe checkpoint | Fresh-process test |
| LVL-20 | Target readable from intended camera and zoom | PIE screenshot/video |
| LVL-21 | Contrast/material states distinguish progress | Visual review |
| LVL-22 | Asset provenance and permission verified | Manifest |
| LVL-23 | Reimport/model refinements preserve source and Actor IDs | Editor diff |
| LVL-24 | Win64 package cooks map and geometry | UAT logs |
| LVL-25 | Fresh Windows build plays mandatory critical route | Human acceptance |

**T14 publication only authored this guide and the script. None of the listed Engine/gameplay tests ran here.** Conditional Geometry Script cases are **NOT APPLICABLE** only if that technique was not part of the approved scope; a necessary progress gate cannot be omitted from a real game's acceptance merely because it was missing from this educational blockout.

## 9. Failure recovery and performance

| Failure | Correction |
| --- | --- |
| Player walks in Editor but not PIE | Inspect Pawn collision, floor gap and CharacterMovement instead of editor fly cam |
| Gap/route invisible after sacrifice | Restore actual collision state from progress owner |
| Camera retracts at small stone | Inspect ECC_Camera and Spring Arm from T08 |
| Geometry Script actor grows every rebuild | Clear/reinitialize its own DynamicMesh before appending geometry, inspect actual Blueprint graph |
| Actor accidentally deleted by mesh replacement | Restore owned Actor identity and separate visual mesh from gameplay controller |
| Modeling tool preview looks right but reloaded mesh wrong | Accept, save actual asset and re-open |
| Import produces correct appearance but wrong scale | Compare Blender metres to Unreal centimetres and measured bounding boxes |
| DynamicMesh runtime performance is poor | Bake approved static output and test real target platform |
| Other operator's map already dirty | Stop writes, preserve state and request handoff |

## 10. Next

Follow [T15 — Niagara, Chaos and deterministic reconstruction](README.md) for **presentation that follows state**, not substitutes for a real restored route.

**Official sources:** [Epic Modeling Mode Overview](https://dev.epicgames.com/documentation/unreal-engine/modeling-mode-in-unreal-engine), [Epic Modeling Mode Quick Start](https://dev.epicgames.com/documentation/en-us/unreal-engine/modeling-mode-quick-start-in-unreal-engine), [Epic Geometry Script Guide](https://dev.epicgames.com/documentation/unreal-engine/geometry-scripting-users-guide-in-unreal-engine), [Epic 5.9 Geometry Scripting Reference](https://dev.epicgames.com/documentation/unreal-engine/geometry-scripting-reference-in-unreal-engine).
