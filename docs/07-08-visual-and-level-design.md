# Chapters 07–08 — Visual Development and Level Design

**Core principle:** a concept, a level diagram, an Unreal blockout, and an imported game-ready asset are different deliverables.

**Practical reference and asset-brief tutorial:** [T10 — Visual References, Style Bibles and Generation-Ready Art Briefs](../tutorials/visual-references-generation-ready-briefs.md) provides one coherent art grammar, a rights-cleared multi-view reference package and [worked two-state landmark brief](../examples/visual-art-brief/README.md).

## 07. Visual-first development

1. Approve character silhouette and its behavior at **actual gameplay camera size**, not just in a portrait concept.
2. Establish a world-wide vocabulary: architecture, materials, recurring shape motifs, color and lighting, before/after states.
3. Make reference boards for *all essential landmarks* so that late locations are not visually invented under delivery pressure.
4. Separately produce **clean single-asset references** for generators and three-/four-view sheets for characters. Text overlays, scenic backgrounds and multiple touching objects may confuse 3D generation.
5. Record the human-selected concept and rejected variants; only the selected version becomes production authority.

The Unmade's visual direction distinguished **an intentionally incomplete city** from arbitrary ruins. Gothic forms, missing plates, restrained inner cyan light and later warm life connected the protagonist to architecture. This art choice also created clear functional requirements: a restored bridge must actually become walkable.

## 08. Spatial design and blockout

Create a route diagram and a scale-aware spatial reference before authoring final geometry. Record landmark order, elevations, sightlines, entrances, exits, gate conditions, recovery routes, one-way changes, checkpoints and camera occlusion.

Build blockout with real collision and traversable stairs/ramps. Test with the final movement capsule, camera and input, not just with Editor viewport navigation. Avoid big invisible blocking volumes that contradict the visible geometry; distinguish physical collision from nonblocking interaction triggers. Preserve the ability to complete the route after any approved sacrifice.

**Real blockout is not throwaway by definition.** Modeling Mode/Geometry Script can produce final modular Static Meshes for architectural surfaces. Use reusable elements with coherent pivots, scale, collision and material slots. Preserve functional Actor identity when replacing rough geometry with final art.

### World Partition rules

Keep critical progression controllers logically stable; prevent streamed props from independently deciding persistent progress. Applying a restored state on actor registration must be idempotent. Revisited cells must not respawn consumed pickups or replay completed cinematics.

### Human acceptance checklist

- The complete critical route is visible and traversable.
- Stairs, terraces and bridges work with normal CharacterMovement.
- The intended camera never hides critical interactions for unacceptable periods.
- Before a gate is earned, its collision actually blocks travel; after restoration it does not.
- Required post-sacrifice routes do **not** require the donated ability.
- Interaction points are reachable without clipping or invisible walls.
- Improvements to vertical drama do not compromise readability or navigation.

**Exit evidence:** approved whole-world concepts, a spatial diagram, actual Unreal blockout and recorded human traversal observations.