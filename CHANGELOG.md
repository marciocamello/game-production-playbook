# Changelog

## 2026-11-16 — T12: source rig audit and Unreal animation retargeting

- Published [T12](tutorials/character-rigging-animation-retarget.md), covering rest pose and hierarchy compatibility, separate skeletal-mesh/animation-only FBX import, IK Rig/Retargeter and real AnimBP/PIE gates.
- Added a [read-only Blender audit script](examples/blender-rig-audit/README.md) for a real existing armature/mesh, optional rest-matrix comparison, vertex weighting, active Action samples and a new non-overwriting JSON report.
- Updated main navigation, tutorial index, Blender pipeline and implementation chapter. No fictional rigs, empty animation clips, GitHub Actions or private source art were added.
- Real Blender input audit, Unreal imports/Retargeter, human deformation review and packaged-game checks remain NOT RUN.


## 2026-11-16 — T11: Blender modular modeling, UV and PBR

- Published [T11](tutorials/blender-modeling-uv-pbr-export.md) with modeling cleanup, topology/UV/PBR rules, measured import scale and 15 concrete QA cases.
- Added [original Blender Python modular-kit builder](examples/blender-modular-kit/README.md) that creates actual floor/wall/pillar geometry, unwraps UVs, applies PBR parameters and exports three FBX files with a manifest when run.
- Script requires an isolated background Blender process and refuses existing output directories. No source art, existing files or GitHub Actions are overwritten.
- Updated the tutorial index, Blender pipeline and implementation chapter. Blender execution, FBX contents, Unreal import, PIE and packaging remain NOT RUN.


## 2026-11-16 — T10: art-direction contracts and generation-ready briefs

- Published [T10](tutorials/visual-references-generation-ready-briefs.md), with human-selected style bibles, rights-cleared multi-view references, technical Blender/Unreal briefs, a 12-case acceptance matrix and failure recovery.
- Added an [original worked two-state modular landmark brief](examples/visual-art-brief/README.md). It is intentionally a documented design example, not a fake model, reference render or Unreal asset.
- Reused the [existing canonical asset-brief template](templates/asset-brief.md); no duplicate authority or unlicensed third-party imagery was added.
- Updated main navigation, tutorial roadmap and visual-design chapter. Production art and in-engine checks remain NOT CREATED/NOT RUN.


## 2026-11-16 — T09: UMG/MVVM HUD, functional menus and persisted settings

- Published [T09 — UMG/MVVM HUD, Menus, Settings and Loading Boundaries](tutorials/umg-mvvm-hud-menus-settings-loading.md) with an explicit six-WBP Editor wiring plan, 27 real acceptance cases and detailed false-success prevention for Save/Continue.
- Added an [original four-file C++ HUD ViewModel and PlayerController implementation](examples/umg-mvvm-demo/README.md) with FieldNotify updates from the existing T07 run events, New Game, pause/resume, async Save feedback, current mapped controls and native user settings for graphics/VSync, sound class volume and subtitles.
- Reused the existing T05/T08 Character and T07 subsystem rather than introducing duplicate gameplay state. A complete Continue/Load coordinator remains unimplemented and is **not** presented as a working button.
- Updated canonical navigation, Unreal pipeline and UI chapter. No stubs, copied private source, GitHub Actions or fabricated test results were added.
- **Execution boundary:** source has not been compiled, Widget Blueprints have not been authored in a live UE Editor, and no PIE, settings fresh-process or packaged validation was performed by the playbook. Results remain NOT RUN.

## 2026-11-16 — T08: elevated camera, real zoom/sprint and collision diagnosis

- Published [T08 — Camera Rigs, Occlusion and Enhanced Input](tutorials/camera-rigs-occlusion-enhanced-input.md) with practical Spring Arm and Gameplay Cameras approaches, explicit Camera Asset authoring, collision debugging and 25 separate acceptance scenarios.
- Extended the **existing** [T05 GAS Character source](examples/gas-light-demo/README.md) rather than creating a second Character or camera manager: smooth bounded wheel zoom, Sprint/ResetSprint, explicit camera probe settings and actual Spring Arm collision-displacement reporting.
- Retained T05 backward compatibility by making both additional Input Actions optional for the original four-action lesson; completing T08 requires authoring and binding them in the real Editor.
- Reconciled the tutorial roadmap, main navigation and Unreal pipeline. No private source/paths, placeholder assets or GitHub Actions were added.
- **Evidence boundary:** C++ changes were published but not compiled in a local Unreal 5.9 Editor, bound to new native Input Actions, manually played, cooked or packaged during publication; all such checks remain NOT RUN.

## 2026-11-16 — T07: SaveGame, fallback and World Partition state reapplication

- Published [T07 — SaveGame, Reload and World Partition](tutorials/savegame-reload-world-partition.md), covering versioned snapshots, authoritative state, A/B slots, async save/readback verification, checkpoints, world projection, stream-in idempotency and fresh-process Continue.
- Published [original three-file C++ SaveGame/Run Subsystem](examples/savegame-ab/README.md) with real stage transitions and A/B slot persistence logic, plus a [read-only PowerShell file evidence tool](examples/savegame-ab/scripts/Inspect-SaveSlots.ps1).
- Added a 30-case test matrix across compile, Editor, manual PIE, corruption fallback in isolated copies, region streaming, checkpoint safety and packaged-game launch.
- Updated navigation and the T06 follow-up. The example is original but **has not been compiled, integrated or played in an installed UE 5.9 project** by the playbook maintainers; unperformed checks are NOT RUN.

## 2026-11-16 — T06: contextual interactions and voluntary sacrifice

- Published [T06 — Contextual Interaction, Fragments and Voluntary Sacrifice](tutorials/contextual-interactions-pickups-sacrifices.md), with concrete implementation contracts for actor focus, collision/line-of-sight checks, unique fragment IDs and idempotent collection.
- Specified replacement of the T05 direct-input donation/recovery paths with one context action, cancelable hold-to-confirm and a separate Recovery fragment.
- Added 24 independent human PIE acceptance scenarios, including wall obstruction, target change, duplicate IDs, canceled holds and anti-softlock traversal.
- Linked the guide from the tutorial index, project README, Unreal pipeline and T05 follow-up. This is a documentation update; it does not claim the reader's project was implemented, compiled or run.

## 2026-11-16 — T05: real GAS gameplay source and manual integration checks

- Published [T05 — Gameplay C++, Gameplay Tags and GAS](tutorials/gameplay-gas-tags-light.md), with a precise native ability lifecycle, derived Gameplay Tag blocking, in-session donation/recovery and human PIE acceptance cases.
- Added [original eight-file C++ GAS Light example](examples/gas-light-demo/README.md), including a native PointLight ability, elevated single-player Character, real Enhanced Input binding handlers and a light-powered world beacon.
- Linked the tutorial and source from canonical navigation and Unreal Engine pipeline. Source is MIT-designated original educational code, not a copy of any private game's implementation.
- **Execution boundary:** Example code was authored and documented, but not compiled, Editor-integrated, tested in PIE or packaged on an installed UE 5.9 machine in this publication. SaveGame/streaming, multiplayer, hold-confirm interaction, distinct pickup and user settings are out of the tutorial's narrow scope.
- No unrelated code, private absolute paths or GitHub Actions.

## 2026-11-16 — T04: multi-agent ownership, bounded parallelism and Editor handoffs

- Published [T04 — Multi-Agent Game Production](tutorials/multi-agent-production-handoffs.md) with explicit dependencies, one-writer-per-asset rules, exclusive Unreal Editor/MCP ownership, safe dirty-package transfer, handoff evidence and human acceptance.
- Added the [Artifact Ownership Matrix](templates/artifact-ownership-matrix.md), a practical [handoff example](examples/multi-agent-handoff/README.md) and a real read-only [Git preflight script](examples/multi-agent-handoff/scripts/Inspect-GitPreflight.ps1).
- Used current official Unreal MCP and Git documentation; the preflight uses Git's `--no-optional-locks` to avoid incidental index refresh locking.
- Updated canonical README, pipeline, templates and tutorial index. No private paths, game source mirroring, GitHub Actions or invented test successes.
- **Runtime limitation:** PowerShell preflight has not been executed on the reader's machine; Editor dirty packages and ignored files remain outside Git's status view.

## 2026-11-16 — T03: concept, design authority and executable SDD

- Published [T03 — From Concept to GDD, TDD and Executable SDD](tutorials/concept-to-gdd-tdd-executable-sdd.md), teaching documented human approvals, durable gameplay-state ownership, contract traceability and end-to-end verification.
- Added four source-neutral authoring templates: [Project Charter](templates/project-charter.md), [GDD](templates/game-design-document.md), [TDD](templates/technical-design.md) and [ADR](templates/architecture-decision-record.md).
- Reused the existing canonical Feature Spec template; no duplicate feature contract or fabricated implementation was created.
- Reconciled tutorial roadmap, main navigation, design chapter and template index. All repository artifacts remain English and free of personal absolute paths.

## 2026-11-16 — T02: real local Unreal builds and packages

- Published [T02 — Unreal Engine 5.9 project bootstrap and local build pipeline](tutorials/unreal-project-bootstrap-local-build.md).
- Added actual reusable PowerShell wrappers for Engine/project preflight, `Build.bat` Editor compilation and `RunUAT.bat BuildCookRun` Windows packaging in [examples/unreal-local-build/](examples/unreal-local-build/README.md).
- Enforced explicit version/target checks and unique package archives; no GitHub Actions or private machine paths.
- Linked the tutorial and scripts from the main index, tutorial roadmap and Unreal Engine pipeline.
- **Execution note:** wrappers are published as real commands but were not run against a live Windows Unreal 5.9 installation by this repository; build, PIE and packaged-game results remain unverified.

## 2026-11-16 — Practical tutorial series started

- Published the [20-topic technical tutorial roadmap](tutorials/README.md), differentiating authored guidance from topics not yet authored.
- Published [T01: Unreal Engine 5.9 MCP](tutorials/unreal-mcp-5-9.md): safe local setup, client selection, schema discovery, a real bounded lighting exercise, save/reopen checks and troubleshooting.
- Linked the new tutorial to the main README and Unreal Engine pipeline.
- All tutorial content is English; the instructions are referenced against Epic's UE 5.9 documentation. No reader's local Editor was operated or tested by this publication.

## 2026-11-16 — Public playbook foundation

- Initialized independent, public documentation repository with manual, AI-assisted and AI-driven production paths.
- Defined a 197-step production roadmap, evidence-oriented task contracts, and versioned guidance for Unreal Engine and Blender.
- Established CC BY 4.0 documentation and MIT code licensing notices without relicensing third-party content.
- Began a sanitized The Unmade case study based on historical design files and agent implementation reports.
- Added no hosted workflows, application code, binary Unreal assets, private repository history or device-specific paths.

This is a **documentation release**. It does not claim an Unreal game or tooling has been built from this repository.