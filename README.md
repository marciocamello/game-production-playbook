# Game Production Playbook

**A practical, evidence-driven guide to making games from idea to release — manually, with AI assistance, or through AI-driven production.**

This repository teaches an end-to-end production system rather than a collection of prompts. It grew out of the planning, execution, mistakes, corrections, and scope decisions of *The Unmade*, a project developed for Epic AI MegaJam 2026 using Unreal Engine 5.9, C++, Blueprints, Blender, and multiple AI-assisted workflows.

## Choose your production model

| Model | Who produces the work? | Who approves it? |
| --- | --- | --- |
| **Manual** | Developers, designers and artists | Human product owner / team |
| **AI-assisted** | People using AI for selected tasks | Human product owner / team |
| **AI-driven** | Assigned agents operate real tools and generate artifacts | Human director; independent verification when appropriate |

All three use the **same gates**: approved intent → design → executable specification → real implementation → integration → testing → human acceptance → release. A model response, generated asset, passing compile, and playable packaged game are **different evidence levels**.

## Start here

1. [How to use this playbook](docs/00-getting-started/README.md)
2. [Full production roadmap: 197 steps](docs/production-roadmap.md)
3. [Feature execution and evidence model](docs/06-production-planning/feature-execution.md)
4. [Working with human and AI contributors](pipelines/ai-driven-development/README.md)
5. [Unreal Engine workflow](pipelines/unreal-engine/README.md)
6. [Blender and 3D asset workflow](pipelines/blender/README.md)
7. [The Unmade: historical case study](case-studies/the-unmade/README.md)
8. [Practical tutorials: 20 hands-on topics](tutorials/README.md)
9. [T01 — Unreal Engine 5.9 MCP tutorial](tutorials/unreal-mcp-5-9.md)
10. [T02 — Unreal project bootstrap and local Windows build](tutorials/unreal-project-bootstrap-local-build.md)
11. [T03 — From concept to GDD, TDD and executable SDD](tutorials/concept-to-gdd-tdd-executable-sdd.md)
12. [T04 — Multi-agent ownership and Unreal Editor handoffs](tutorials/multi-agent-production-handoffs.md)
13. [T05 — Gameplay C++, Gameplay Tags and GAS](tutorials/gameplay-gas-tags-light.md)
14. [T06 — Contextual interactions, fragments and voluntary sacrifice](tutorials/contextual-interactions-pickups-sacrifices.md)
15. [T07 — SaveGame, Reload and World Partition](tutorials/savegame-reload-world-partition.md)
16. [T08 — Camera rigs, occlusion and Enhanced Input](tutorials/camera-rigs-occlusion-enhanced-input.md)
17. [T09 — UMG/MVVM HUD, menus, settings and loading boundaries](tutorials/umg-mvvm-hud-menus-settings-loading.md)
18. [T10 — Visual references and generation-ready asset briefs](tutorials/visual-references-generation-ready-briefs.md)
19. [T11 — Blender modeling, retopology, UV/PBR and real FBX export](tutorials/blender-modeling-uv-pbr-export.md)
26. [Blender modular stone kit Python generator](examples/blender-modular-kit/README.md)
27. [Original UMG/MVVM native HUD and menu example](examples/umg-mvvm-demo/README.md)
20. [Original versioned SaveGame A/B C++ and inspection tool](examples/savegame-ab/README.md)
21. [Light ability and T08 camera/sprint C++ example](examples/gas-light-demo/README.md)
22. [Original worked art-production brief](examples/visual-art-brief/README.md)
23. [Real local build/packaging PowerShell examples](examples/unreal-local-build/README.md)
24. [Read-only Git preflight and multi-agent example](examples/multi-agent-handoff/README.md)
25. [Templates and reusable checklists](templates/README.md)

## Production chapters

| Stage | Chapter |
| --- | --- |
| 00 | [Getting started](docs/00-getting-started/README.md) |
| 01–03 | [Discovery, research and preproduction](docs/01-03-discovery-and-preproduction.md) |
| 04–06 | [Game design, architecture and SDD](docs/04-06-design-and-planning.md) |
| 07–08 | [Visual and spatial design](docs/07-08-visual-and-level-design.md) |
| 09–12 | [Programming, art, animation and integration](docs/09-12-implementation.md) |
| 13–14 | [UX, audio, VFX and cinematics](docs/13-14-presentation.md) |
| 15–16 | [QA, packaging and release](docs/15-16-quality-and-release.md) |
| 17–18 | [Postmortem and maintenance](docs/17-18-postmortem-and-maintenance.md) |

## What makes this playbook different

- **Implementation over readiness.** A feature specification is a commitment to a working, integrated behavior, not a collection of stub methods or an intention to implement later.
- **Reuse before rewriting.** Inspect existing classes, assets, services, editor state, and scripts before generating more.
- **One owner per editable artifact.** Stateful editors and shared files require explicit handoffs; concurrent agents must not write to the same asset.
- **Visual-first where appearance shapes traversal.** Concepts, spatial references, technical blockouts and final 3D production are distinct artifacts.
- **Proof over confident reports.** Record PASS, FAIL, BLOCKED and NOT RUN and distinguish compile, Editor validation, manual playtest, packaging, and launch.
- **Ship the smallest coherent game.** Protect the executable, save/load, controls, critical route, finale and delivery requirements before optional polish.
- **Reproducible, privacy-safe examples.** Use repository-relative paths, no personal machine locations, no credentials, and no proprietary Engine source.
- **No hosted CI or GitHub Actions.** Examples use local execution and explicit evidence.

## Case-study integrity

The original jam repository is not mirrored here. Public case-study material is a selective, rewritten account of **documented decisions and available implementation reports**. Historical specs sometimes predate later execution reports; the study distinguishes planned, implemented, tested, human-validated and actually released work. Unknown results remain unknown. Missing or unaudited evidence is not a PASS.

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for scope, evidence, privacy, relative-path, intellectual-property and review requirements. Do not upload unrelated private repositories, sensitive logs, or binary Unreal assets without explicit rights and verification.

## Licensing

Original guides, prose and text templates are offered under [CC BY 4.0](LICENSE.md). Original scripts and code samples, when added and specifically designated, are offered under the [MIT License](LICENSE-CODE.md). Third-party artwork, engine source, plugins, trademarks, datasets and generated-media outputs retain their own applicable rights: see [THIRD_PARTY.md](THIRD_PARTY.md). The licenses do not relicense others' work.
