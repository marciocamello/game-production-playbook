# Practical Technical Tutorials — Publication Roadmap

This is the **implementation-oriented companion** to the [197-step production roadmap](../docs/production-roadmap.md). Each published tutorial includes applicable versions, required project state, a reproducible real-world exercise, tool ownership, exact inspection points, expected outputs, failure recovery, verification and authoritative references. A tutorial being published does **not** claim that its exercise was run in a reader's project.

**Publication language:** English for every repository document, prompt, example, report and filename.

**Status definition:** **Published** means a complete instructional document is available; **Not yet authored** means no tutorial page exists. Neither status is a claim about a game feature's implementation. This list is prioritized for editorial production, not a requirement that game teams execute every technology in this exact order.

| ID | Tutorial | Concrete learning outcome | Documentation status |
| --- | --- | --- | --- |
| T01 | [Unreal Engine 5.9 MCP: connect, discover, author, verify](unreal-mcp-5-9.md) | Safely connect to the correct Editor and complete a real, saved lighting edit with explicit evidence | **Published** |
| T02 | [Unreal Engine 5.9 project bootstrap and local build pipeline](unreal-project-bootstrap-local-build.md) | Configure a real C++/Blueprint project, compile an Editor target, package a Win64 executable and verify separate launch using [local scripts](../examples/unreal-local-build/README.md) | **Published** |
| T03 | From game concept to GDD, technical design and executable SDD | Produce approved designs and one independently verifiable gameplay feature contract | Not yet authored |
| T04 | Agent orchestration, file ownership and Editor handoffs | Execute multiple nonconflicting source, art and Editor workstreams without losing asset changes | Not yet authored |
| T05 | Gameplay C++: state, Gameplay Tags and GAS | Implement a real input-driven ability and a durable state transition | Not yet authored |
| T06 | Contextual interaction, pickups and irreversible gameplay choices | Make a world object respond exactly once to a valid player action and reject invalid inputs | Not yet authored |
| T07 | SaveGame, reload and World Partition | Persist gameplay state and reapply it idempotently after process restart or streaming | Not yet authored |
| T08 | Camera rigs, occlusion and Enhanced Input | Build a playable stable elevated camera, context-aware control and verified zoom/sprint | Not yet authored |
| T09 | UMG/MVVM: HUD, menus, settings and loading | Build real widgets backed by authoritative state, input routing and persisted options | Not yet authored |
| T10 | Visual references and generation-ready art briefs | Translate approved art direction into coherent, legally trackable asset production inputs | Not yet authored |
| T11 | Blender modeling, retopology, UV and PBR | Repair actual meshes, create consistent modular pieces and round-trip exports | Not yet authored |
| T12 | Character rigging, animation and Unreal retargeting | Validate bind pose, skeleton compatibility and animation-only FBX scales in a real import | Not yet authored |
| T13 | Unreal import and actor/material integration | Convert sourced meshes into stable, optimized, gameplay-aware engine assets | Not yet authored |
| T14 | Level design, Modeling Mode and Geometry Script | Produce collision-correct authored architecture with camera-readable traversal | Not yet authored |
| T15 | Niagara, Chaos and deterministic restoration | Build presentation effects that do not corrupt permanent world state | Not yet authored |
| T16 | Sequencer, gameplay transitions and cinematic return | Run an interruptible story beat and restore correct player control | Not yet authored |
| T17 | Reactive music, spatial sound and audio rights | Drive audible gameplay feedback from committed events and verify licensing | Not yet authored |
| T18 | Native tests, Editor checks, human PIE and profiling | Run distinct validation layers and log failed or unrun checks accurately | Not yet authored |
| T19 | Windows packaging, real gameplay capture and release | Produce, launch, test and publish a downloadable standalone build | Not yet authored |
| T20 | Glass Box, provenance and evidence-based postmortem | Publish a privacy-safe record of decisions, AI use, assets, failures and scope cuts | Not yet authored |

## How to use the series

Choose the tutorial relevant to the next **actual dependency** in your game. A manually authored Blender character does not need T01; an AI agent that must operate a UE Editor does. AI-assisted and AI-driven teams follow the same acceptance gates as manual teams. The [feature execution contract](../docs/06-production-planning/feature-execution.md) governs real game feature work.

## Tutorial editorial acceptance

A tutorial is ready for publication only when it:

1. Cites applicable, version-specific official tool documentation.
2. Gives concrete steps a reader can perform in real software, not imagined or stubbed tooling.
3. Warns clearly when an operation can overwrite game content or expose credentials.
4. Identifies how to prove that the artifact **exists, is saved and works**.
5. States which execution was **not performed by this repository's maintainers**, instead of inventing test output.
6. Uses no absolute personal machine paths, private code or redistributed third-party assets.
7. Does not introduce GitHub Actions or hosted automation.

The tutorials are **documentation**. Executing them requires a real engine/project/tool installation under the reader's control.
