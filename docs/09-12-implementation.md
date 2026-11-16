# Chapters 09–12 — Gameplay, 3D Production, Animation and Engine Integration

## 09. Implement gameplay first as a working state transition

Use the approved technical design to implement one player action whose outcome changes the world. In C++ or the equivalent engine language, keep authoritative data separate from visuals. A pickup is collected exactly once. An interaction checks target, distance, line of sight and eligibility. A hold-to-confirm donation cancels harmlessly if released early or the target is lost; successful commit applies only once. A bridge, fountain or checkpoint derives visible/collision state from that commit.

Before adding a new component, locate and reuse the existing ability, input, interaction, save, actor and UI APIs. Implement targeted native tests and integration evidence. Never claim that a C++ test proves the player can see or reach an object.

## 10. Produce and validate 3D assets

Choose the cheapest tool producing **usable geometry**, not the greatest number of attractive renders. Editor Modeling Mode can build modular walls; Blender can create or fix topology, UVs, pivots, rigs and exports; Tripo/Meshy can generate candidate meshes needing inspection.

Required source-asset checks:

| Check | Reason |
| --- | --- |
| Bounds and units | Avoid 100× scale changes across Blender and Unreal |
| Vertex/triangle counts and geometry | Avoid degenerate/floating pieces and surprise performance costs |
| Normals, tangents, UVs and textures | Preserve shading and correct PBR interpretation |
| Material slots | Retain stable state/material assignments |
| Pivot, naming and coordinate axes | Allow modular assembly and reliable placements |
| Skeleton / bind pose | Prevent collapse, limb distortion or invalid retarget |
| Rights and provenance | Keep redistribution and credit legal |
| Representative reimport | Verify the actual FBX/GLB pipeline rather than only export success |

For native Unreal static meshes, a verified Editor FBX export/reimport is a meaningful transfer test, but it does not replace Tripo generation, Blender revision or packaged runtime validation.

## 11. Character and motion production

Start from a consistent multiview character reference and a neutral riggable pose. Check bone parents, local axes, bind/rest pose and deformation. Reusing **bone names** across four characters does not guarantee they share identical rest pose or one interchangeable skeleton.

Produce only the animations needed by real gameplay: idle, move, interact, response, recovery and finale as required. Verify animation-only FBX scale separately from mesh scale, test retarget at several sampled frames, check foot contact and loop seams, and confirm a valid blending/animation graph. If an MCP integration cannot author AnimGraph nodes, document the limitation and use an actually supported authoring route; do not pretend the graph exists.

## 12. Integrate in the Engine

1. Assign one active Unreal Editor/MCP owner; inspect loaded map, dirty assets, file ownership and existing Blueprints before edits.
2. Import source/export assets into coherent project content paths via supported Editor APIs.
3. Preserve existing Actor identity, triggers, persistent state IDs and collisions.
4. Build a small material-parent library and parameterized instances.
5. Wire state-driven meshes, animations, audio and VFX to already committed game events.
6. Apply permanent world state immediately on load/stream-in; play transition presentation only after a fresh action.
7. Test Blender export/import integrity, Unreal asset validation, Blueprint and material compilation and level placement.
8. Handoff to a human for actual PIE and the release owner for cooked standalone checks.

**Asset states:** Concept → Source Created → Export Verified → Imported → Integrated → Editor Checked → Runtime Tested → Human Validated. Report the highest *actually evidenced* stage.

References: [Unreal Engine pipeline](../pipelines/unreal-engine/README.md) · [Blender pipeline](../pipelines/blender/README.md) · [feature execution](06-production-planning/feature-execution.md).