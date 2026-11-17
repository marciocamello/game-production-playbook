# Static Source and Executable-Helper Audit — 2026-11-17

**Repository:** [Game Production Playbook](../../README.md)
**Initial audited main revision:** `18296fc96d1ef54247c69aa9b62410bc68dd6507`
**Scope:** existing tutorial series T01–T20, repository-relative documentation links, native Unreal C++ examples, PowerShell local tools, Blender/Unreal Editor Python scripts, and the standalone Python Glass Box verifier.
**Method:** source review and direct GitHub file inspection; public Epic API cross-reference; measured local Python execution. This is **NOT** a full Unreal Engine 5.9 or Windows integration certification.

## Executive result

- **Documentation inventory:** 20 published tutorial pages, 197 numbered production roadmap items and 75 Markdown files inspected for repository-relative link destinations at the time of the link check.
- **Internal link check:** **PASS** — 75/75 Markdown files checked for standard Markdown local file/directory destinations against the real GitHub tree, with **0 unresolved relative paths**. This did not test external URLs, fragment anchors, or rendered markdown/UI correctness.
- **Source inventory:** 29 original `.h`, `.cpp`, `.ps1` and `.py` examples at initial baseline. A new test harness was then added, taking source inventory to 30. All 30 were included in a bounded static scan for obvious empty methods, TODO/FIXME/NotImplemented markers, deprecated Editor scripting calls and destructive patterns. A code scan is not UHT/UBT validation.
- **Standalone Python CLI:** **PASS (14/14)** — the actual T20 evidence auditor and newly committed real regression suite were executed on this host using exact byte copies verified against GitHub blob SHAs. Python 3.13.5, isolated Linux sandbox, 14 cases passed, 0 blocked.
- **Unreal Editor/UHT/UBT/PIE/packaging:** **BLOCKED/NOT RUN** — installed UE 5.9, Blender and PowerShell 7 were not available in the audit runtime, nor was the user's Windows Editor connected. No Unreal or Windows runtime outcome was invented.
- **GitHub Actions:** no hosted workflow was introduced.

## Confirmed source findings and completed changes

| ID | Risk / original behavior | Applied source correction | Verification state |
| --- | --- | --- | --- |
| A01 | T05 Character/Beacon and T09 UI Controller were declared native `final` although the tutorial requires editable Blueprint subclasses for asset assignments | Made those three native Actor classes explicitly `UCLASS(Blueprintable)` and removed C++ `final` | **Source changed; UE Blueprint creation NOT RUN** |
| A02 | The T09 menu used `FInputModeUIOnly` while its Escape shortcut was bound on the PlayerController, causing that controller binding not to receive UI-only input | Replaced with `FInputModeGameAndUI`, focused the actual widget and retained the paused-game design; unhandled keys can reach controller | **Source changed; PIE keyboard/gamepad tests NOT RUN** |
| A03 | T09 pushed a SoundMix once per controller lifetime but did not release its own modifier when the controller ended | Added a balanced `PopSoundMixModifier` during EndPlay, only if pushed | **Source changed; runtime audio lifecycle NOT RUN** |
| A04 | T09 `StartNewGame` recorded `GetWorld()->GetFName()` as MapId; PIE worlds can include an editor-only map prefix | Use `UGameplayStatics::GetCurrentLevelName(this, true)` and reject missing MapId | **Source changed; PIE/packaged round-trip NOT RUN** |
| A05 | The T07 PowerShell `-OutputJson` option used `Set-Content` and could overwrite an existing report in the SaveGame location | Reject an output path within SaveGames and atomically create a new report via `FileMode.CreateNew`; no replacement of existing evidence | **Source changed; actual Windows PowerShell run NOT RUN** |
| A06 | T14 blockout script used the deprecated `unreal.EditorLevelLibrary.get_editor_world()` call | Use `unreal.get_editor_subsystem(unreal.UnrealEditorSubsystem).get_editor_world()` and update integration guide | **Source changed; live UE 5.9 Editor call NOT RUN** |
| A07 | T20 evidence auditor was published without executable CLI regressions in the repo | Added [14 real negative/positive regression cases](../../examples/glass-box-audit/tests/test_evidence_auditor.py) and ran the exact committed test source against the exact committed auditor | **PASS locally (14/14), no UE claims** |

The source-level patches preserve the existing classes, state and responsibilities instead of adding duplicate gameplay authorities or unfinished replacement services.

## Reproducible local T20 evidence

**Execution environment:** Python 3.13.5 on Linux sandbox (2026-11-17). No network or engine use was needed to execute the Python verifier.

**Blob integrity:** the copied original [auditor](../../examples/glass-box-audit/scripts/audit_evidence_manifest.py) matched GitHub git-blob SHA `fdd9e1d19ddf8b9e9a4710d400038aa4ce0981aa`; the copied original [regression suite](../../examples/glass-box-audit/tests/test_evidence_auditor.py) matched `f6cf32854cb5c9205be9f6445220f9a2fbc0b31d`.

Actual command on the audited copy:

    python examples/glass-box-audit/tests/test_evidence_auditor.py

**Observed:** `REAL CLI TESTS: 14 passed, 0 blocked`, exit code 0. The following cases had their **expected** process exit codes independently checked: valid pass=0; NOT RUN open gate=2; FAIL with evidence=2; FAIL without evidence=1; bad hash=1; path traversal=1; duplicate ID=1; unknown status=1; invalid schema=1; empty tests=1; missing file=1; missing license reference=1; BLOCKED gate=2; symlink escaping evidence root=1. File contents and SHA-256 were real temporary test inputs, not game evidence. `python -m py_compile` also completed for the original auditor.

**Boundaries:** these cases test manifest schema, safe relative file access, hash consistency and reported outcome flags. They cannot authenticate gameplay footage, verify legal permission or replace human playtesting. Tests were executed locally on verified identical source bytes; the user's Windows workstation was not accessed.

## Version-sensitive API check

The relevant Epic public references were consulted for:

- [Blueprintable classes](https://dev.epicgames.com/documentation/unreal-engine/exposing-gameplay-elements-to-blueprints-visual-scripting-in-unreal-engine) and [class specifiers](https://dev.epicgames.com/documentation/unreal-engine/class-specifiers).
- [Input modes (UI Only)](https://dev.epicgames.com/documentation/unreal-engine/API/Runtime/Engine/FInputModeUIOnly) versus [Game and UI](https://dev.epicgames.com/documentation/unreal-engine/API/Runtime/Engine/FInputModeGameAndUI).
- [Gameplay Statics](https://dev.epicgames.com/documentation/unreal-engine/API/Runtime/Engine/UGameplayStatics), including GetCurrentLevelName, audio mix push/pop and subtitles.
- [Unreal Editor Subsystem](https://dev.epicgames.com/documentation/unreal-engine/API/Editor/UnrealEd/UUnrealEditorSubsystem/GetEditorWorld) in preference to deprecated [EditorLevelLibrary](https://dev.epicgames.com/documentation/unreal-engine/API/Plugins/EditorScriptingUtilities/UEditorLevelLibrary/GetEditorWorld).
- [Official UE 5.9 Python scripting](https://dev.epicgames.com/documentation/unreal-engine/scripting-the-unreal-editor-using-python).

Documentation cross-check **supports the target API choices** but cannot prove a particular plugin/module compiles in the installed user's UE 5.9 build. No precise 5.9 binary/headers were available for this audit.

## Remaining execution blockers — deliberately not marked complete

| Priority | Blocker | Required next real evidence |
| --- | --- | --- |
| P0 | T05 Character still owns an in-session LightStage while T07 subsystem owns durable progression; a real integration must reconcile the two instead of running competing mutable state authorities | Actual owned implementation and C++/PIE state synchronization |
| P0 | T06 focused pickup/hold-confirm/collision eligibility and T07 SaveGame world rehydration need project-level Actor/Controller integration; T09 intentionally does **not** implement a working Continue flow | A single coherent real gameplay map, streaming/checkpoint safety and fresh-process Save/Continue |
| P0 | Native C++ examples (including newly corrected headers and T18 tests) have not been compiled against UE 5.9/UHT/UBT in this audit environment | Exact installed engine target build, individual compilation errors and actual Automation test reports |
| P1 | T14 Editor Python, T11/T12 Blender, T13 import and T15 Niagara/Chaos require actual Editor/DCC/plugin calls | Authorized sandbox Editor project, real authored assets, saved/reopened packages and inspection logs |
| P1 | T16 Sequencer, T17 audio and T09 UMG widgets require real Blueprint/asset authoring; a markdown tutorial does not create binary assets | One operator-owned UE Editor session, saved assets, PIE/packaged integration and human acceptance |
| P1 | PowerShell 7 and Windows toolchain were absent; T02/T07/T19 scripts are statically reviewed but **not executed** | Actual `pwsh` command outcomes on Windows, with safe input paths, UAT archive and authentic video metadata |
| P1 | Regressions after source fixes have not run in a real game | Controller Escape focus, SoundMix lifetime, Blueprint subclass creation, map identity, SaveGame output protection and packaged gameplay checks |

No private original game project was modified or mirrored into the playbook. No GitHub Actions, stub methods, fake audio/mesh packages, invented tests, or unauthorized release were created.

## Next acceptance milestone

Use an **authorized separate small UE 5.9 reference project** (or a compatible existing test project after inspection), not a copy of unrelated private game source. First compile the existing C++ examples and T18 tests; resolve the T05/T07 state-authority handoff; create the real required Blueprint/UMG/Niagara/Sequencer/audio assets; execute the complete collect → donate → restore → recover → Save/Continue route; then package Windows and run it outside the Editor. Record each objective state transition, error and human acceptance individually.

**Overall status:** Static source and documentation audit **completed**. Targeted source safeguards **implemented but uncompiled**. Local T20 helper CLI **tested**. Full playable Unreal pipeline **NOT RUN / BLOCKED** pending access to the real engine and Windows environment.
