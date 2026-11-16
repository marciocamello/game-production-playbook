# Changelog

## 2026-11-16 — T20: Glass Box, source provenance and evidence-based postmortem

- Published [T20](tutorials/glass-box-provenance-postmortem.md), covering decisions, AI participation, artifact rights, exact execution states, private/public boundaries, failures, scope cuts, factual postmortem and 27 acceptance checks.
- Added an original [read-only Python evidence manifest auditor](examples/glass-box-audit/README.md) for existing real files: unique IDs, safe relative paths, SHA-256 verification, named test outcomes, and separate integrity/error/open-gate exit statuses.
- Reused the case-study evidence inventory, canonical templates and T18/T19 evidence; no fabricated logs, licensed assets or private project source were uploaded.
- **Runtime status:** the auditor has not been executed against a real game evidence manifest here; public release/human acceptance are NOT RUN.

## 2026-11-16 — T19: packaged Windows game, authentic gameplay video and release

- Published [T19](tutorials/windows-packaging-gameplay-capture-release.md), reusing the existing [T02 BuildCookRun PowerShell wrappers](examples/unreal-local-build/README.md).
- Added a real [read-only release inspector](examples/release-evidence/README.md) for the existing Win64 archive/target executable, cooked content, SHA-256 and optional ffprobe video resolution/duration/sha verification.
- Separated archive presence from independent executable launch, critical-route gameplay, fresh-process Save/Load, video authenticity, submission and downloaded-artifact verification; 28 explicit release gates.
- **Runtime status:** UBT/UAT, standalone launch, OBS capture, video probe and upload have NOT RUN in this publication.

## 2026-11-16 — T18: native assertions, Editor/PIE QA and profiling

- Published [T18](tutorials/native-tests-editor-pie-profiling.md), with a strict distinction between compilation, native Automation, Editor asset checks, manual PIE, fresh-process SaveGame, standalone launch, profiling and human acceptance.
- Added an [original C++ test](examples/native-validation-tests/README.md) with eight real assertions against the already implemented T07 UGPPDemoRunSubsystem::ValidateSnapshot, no stubbed validator or cloned business rules.
- Recorded 27 evidence gates and regression/error-handling guidance. **Runtime status:** source authored but not compiled, run in UE 5.9, profiled or playtested; all engine checks remain NOT RUN.

## 2026-11-16 — T17: actual reactive sound design and legal source inventory

- Published [T17](tutorials/reactive-music-spatial-audio-rights.md) with a real MetaSound/Audio Component Editor exercise, fresh-commit spatial cue, state-projected ambiance, optional Quartz, applied T09 volume settings, correct SaveGame playback and 28 listening/legal tests.
- Reused T07/T15 gameplay events and existing provenance templates, without duplicating state, shipping unlicensed recordings or declaring nonexistent audio packages integrated.
- **Runtime status:** source audio, Editor MetaSound/attenuation assets, listening tests and packaged audio validation have NOT RUN here.

## 2026-11-16 — T16: interruption-safe Sequencer and gameplay return

- Published [T16](tutorials/sequencer-gameplay-cinematic-return.md), with an explicit 3–5-second Level Sequence authoring exercise, fresh T15 event trigger, one input-lock owner, idempotent finish/skip/unload cleanup and camera/HUD restoration.
- Added 26 distinct acceptance cases and separate source-vs-Editor/package execution boundaries.
- **Runtime status:** no real Level Sequence asset, runtime playback, human PIE or packaged skip test was performed by this documentation publication.


## 2026-11-16 — T15: Niagara, Chaos and deterministic restoration

- Published [T15](tutorials/niagara-chaos-deterministic-restoration.md), with real authoritative-vs-presentation separation, an Editor-authored Niagara burst procedure, optional nonblocking Chaos Geometry Collection/Cache workflow, and 29 distinct acceptance scenarios.
- Extended the **existing T07 Run Subsystem** with `OnBeaconRestored(BeaconId)`, emitted only after a successfully committed donation; load, New Game and ordinary progress events never emit a fresh-restoration signal.
- Added an [original native three-piece restoration presenter](examples/restoration-vfx/README.md) that animates actual Engine Cube components, activates an assigned Niagara System only on fresh restoration, and snaps to durable state on progress changes/World Partition stream-in. It does not own ability, gate collision or SaveGame state.
- Updated canonical navigation, T07 documentation, Unreal pipeline and presentation/implementation chapters. No invented Niagara/Chaos binary assets, private game source, GitHub Actions or fabricated acceptance evidence.
- **Runtime status:** published C++ was not compiled in a UE 5.9 installation; Niagara/Chaos assets have not been authored, recorded, played or packaged, and no PIE/human acceptance was executed. All such checks remain NOT RUN.


## 2026-11-16 — T14: level design, Modeling Mode and Geometry Script

- Published [T14](tutorials/level-design-modeling-mode-geometry-script.md), with a real route dependency contract, Modeling Mode and Geometry Script authoring, World Partition/Actor ownership guidance and 25 distinct gameplay/asset acceptance scenarios.
- Added a [guarded Unreal Editor Python blockout author](examples/level-blockout/README.md) for seven actual structural StaticMeshActors in an approved existing map, with wrong-map/duplicate-prefix refusal and no automated deletion/save-all.
- Updated main navigation, tutorial roadmap, Unreal pipeline and spatial design chapter. No fake gameplay gates, copied private level data or GitHub Actions.
- **Runtime status:** the authored source has not been executed in a UE 5.9 Editor. Real map generation, collision, gameplay state, PIE, World Partition, cook/package and human acceptance remain NOT RUN.


## 2026-11-16 — T13: Unreal FBX import and real actor/material/collision integration

- Published [T13](tutorials/unreal-static-mesh-import-integration.md), covering Unreal 5.9 Content Browser/FBX import, dimensions, UVs, Material Instances, simple/UCX collision and stable Actor ownership.
- Added an [original guarded Editor Python importer](examples/unreal-import-static-mesh/README.md) for one existing FBX and one new Static Mesh package. It checks actual inputs, refuses overwrites and saves only the new target asset.
- Updated main navigation, the Unreal pipeline and implementation chapter. No binary Unreal assets, fictitious materials or unauthorized map changes were added.
- **Not executed:** installed Editor import, actual bounds/materials/collision, human PIE, cook/package and packaged gameplay remain NOT RUN.


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