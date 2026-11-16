# Practical Technical Tutorials — Publication Roadmap

This is the **implementation-oriented companion** to the [197-step production roadmap](../docs/production-roadmap.md). Each published tutorial includes applicable versions, required project state, a reproducible real-world exercise, tool ownership, exact inspection points, expected outputs, failure recovery, verification and authoritative references. A tutorial being published does **not** claim that its exercise was run in a reader's project.

**Publication language:** English for every repository document, prompt, example, report and filename.

**Status definition:** **Published** means a complete instructional document is available; **Not yet authored** means no tutorial page exists. Neither status is a claim about a game feature's implementation. This list is prioritized for editorial production, not a requirement that game teams execute every technology in this exact order.

| ID | Tutorial | Concrete learning outcome | Documentation status |
| --- | --- | --- | --- |
| T01 | [Unreal Engine 5.9 MCP: connect, discover, author, verify](unreal-mcp-5-9.md) | Safely connect to the correct Editor and complete a real, saved lighting edit with explicit evidence | **Published** |
| T02 | [Unreal Engine 5.9 project bootstrap and local build pipeline](unreal-project-bootstrap-local-build.md) | Configure a real C++/Blueprint project, compile an Editor target, package a Win64 executable and verify separate launch using [local scripts](../examples/unreal-local-build/README.md) | **Published** |
| T03 | [From game concept to GDD, technical design and executable SDD](concept-to-gdd-tdd-executable-sdd.md) | Produce an approved project charter, GDD, TDD/ADRs and one execution-ready end-to-end feature contract, with [reusable templates](../templates/README.md) | **Published** |
| T04 | [Multi-agent production, file ownership and Unreal Editor handoffs](multi-agent-production-handoffs.md) | Execute bounded parallel source/art tasks with an [ownership matrix](../templates/artifact-ownership-matrix.md), exclusive Editor sessions and [read-only Git preflight](../examples/multi-agent-handoff/README.md) | **Published** |
| T05 | [Gameplay C++, Gameplay Tags and GAS: a real Light ability](gameplay-gas-tags-light.md) | Integrate a native [eight-file C++ example](../examples/gas-light-demo/README.md) with GAS activation, derived Gameplay Tags, visible lights, in-session donation/recovery and explicit PIE verification | **Published** |
| T06 | [Contextual interaction, fragments and voluntary sacrifice](contextual-interactions-pickups-sacrifices.md) | Extend T05 with focus, LOS, unique pickup IDs, voluntary hold-to-donate, distinct recovery, softlock checks and a 24-case manual acceptance plan | **Published** |
| T07 | [SaveGame, Reload and World Partition](savegame-reload-world-partition.md) | Persist Light, unique fragments, powered landmarks and checkpoints with [original versioned A/B C++ source and read-only slot inspection](../examples/savegame-ab/README.md); verify fresh-process and streaming restoration | **Published** |
| T08 | [Camera rigs, occlusion and Enhanced Input](camera-rigs-occlusion-enhanced-input.md) | Extend the existing [T05 Character](../examples/gas-light-demo/README.md) with smooth bounded zoom, sprint/reset, collision telemetry and a real Editor Gameplay Cameras alternative; verify 25 individual cases | **Published** |
| T09 | [UMG/MVVM: functional HUD, menus, settings and loading boundaries](umg-mvvm-hud-menus-settings-loading.md) | Reuse T07 state with a [four-file C++ ViewModel/Controller example](../examples/umg-mvvm-demo/README.md), real UMG Widget bindings and settings; verify 27 cases without claiming unfinished Continue/loading | **Published** |
| T10 | [Visual references, style bibles and generation-ready art briefs](visual-references-generation-ready-briefs.md) | Create one source-of-truth art grammar, cleared references and an approved asset-production contract using a [worked example](../examples/visual-art-brief/README.md) | **Published** |
| T11 | [Blender modeling, retopology, UV and PBR](blender-modeling-uv-pbr-export.md) | Produce a [real original three-mesh modular kit](../examples/blender-modular-kit/README.md) with UV, Principled BSDF, FBX exports, manifest and non-destructive Blender CLI instructions | **Published** |
| T12 | [Character rigging, animation and Unreal retargeting](character-rigging-animation-retarget.md) | Use a [read-only real Blender rig audit](../examples/blender-rig-audit/README.md), compare rest poses/hierarchy, import separately scaled animations and validate IK Rig/Retargeter | **Published** |
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
