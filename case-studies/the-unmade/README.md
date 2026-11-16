# Case Study — The Unmade (Epic AI MegaJam 2026)

**Historical case, not a redistributable game source mirror.** This chapter examines a single-player Gothic puzzle/exploration game built in an Unreal Engine 5.9 C++/Blueprint production workflow under the official 2026 theme **DIRECTOR'S CUT**.

## The central idea

A small incomplete living reliquary awakens in a dead city. The creature finds missing components and gives away parts of its capabilities to restore a fountain, a mechanical crossing and eventually the city's shared heart. The final sacrifice appears fatal; the restored city returns life to the protagonist.

The intended gameplay contract was **explore → discover a missing part → deliberately sacrifice a capability → restore functional infrastructure → advance or recover**, using a stable elevated camera, permanent world transformations, environmental narration and a brief epilogue. The approved core did not require combat or an LLM running inside the game.

## Why document this project?

The case illustrates both strong practices and failure modes:

- Approving story/GDD/TDD before scaling implementation; still protecting an early real vertical slice.
- Separating visual concept approval, spatial design, Unreal blockout, 3D source generation, and final asset integration.
- Dividing source code, Blender/3D, Unreal Editor/MCP, docs and human PIE between distinct owners.
- Real C++/GAS, World Partition, SaveGame, gameplay cameras, UMG/MVVM, Blueprint, Geometry Script, Niagara, Sequencer and candidate Chaos work.
- Diagnosing asset scale/rest-pose mismatches, invalidated camera requirements, Editor dirty-state ownership and packaging gaps.
- Recording scope cuts and the difference between source/Editor tests and human acceptance.

## Reading order

1. [Historical timeline](timeline.md)
2. [Design and technical decisions](decisions.md)
3. [Feature F001–F014 inventory](features/README.md)
4. [Real integration failures and fixes](failures-and-fixes.md)
5. [Original scope and documented cuts](scope-cuts.md)
6. [Evidence policy and source inventory](evidence-index.md)
7. [Lessons to reuse](lessons-learned.md)

## Accuracy and privacy boundary

The underlying development repository is **private** and is not linked or copied here. This public retelling was prepared from the game's recorded GDD/TDD, feature specifications, agent execution reports and supplied design discussions. **An agent report is attributed as such; the public guide does not claim to have independently run the original game.** Some historical specs still say Planned after later reports describe Editor work; these must be interpreted chronologically. No player submission, award, final packaged gameplay or juror acceptance is asserted without separate public evidence.

This case is a source of reusable methods, **not a template for copying proprietary assets, Engine source, project binaries or private developer paths**. See [evidence index](evidence-index.md) and [third-party policy](../../THIRD_PARTY.md).