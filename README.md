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
8. [Templates and reusable checklists](templates/README.md)

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
