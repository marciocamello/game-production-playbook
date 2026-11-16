# T18 — Native Tests, Editor Checks, Human PIE and Profiling: Evidence That the Game Works

**Publication state:** A complete UE 5.9 multi-layer test and profiling method plus [original native C++ validation test source](../examples/native-validation-tests/README.md). The test source has **not** been compiled/executed in an installed UE 5.9 engine, and no automated test run, live Editor session, manual PIE or standalone profiling has occurred in this playbook publication. All runtime outcomes are **NOT RUN**.

**Objective:** distinguish code-level assertions, saved asset correctness, player-visible PIE behavior, fresh-process persistence, packaged play, and measured runtime performance. A passing process exit is not a substitute for checking failed test cases; a passing compile is not evidence of a completed game.

**Prerequisites:** [T02 local build](unreal-project-bootstrap-local-build.md), [T05 gameplay](gameplay-gas-tags-light.md), [T06 interaction](contextual-interactions-pickups-sacrifices.md), [T07 SaveGame](savegame-reload-world-partition.md), [T08 cameras](camera-rigs-occlusion-enhanced-input.md), [T09 UMG](umg-mvvm-hud-menus-settings-loading.md), [T15 VFX](niagara-chaos-deterministic-restoration.md), [T16 Sequencer](sequencer-gameplay-cinematic-return.md), and [T17 sound](reactive-music-spatial-audio-rights.md). Execute only the layers applicable to your actual feature.

## 1. Establish exact build identity and a gate matrix

Before testing, record project and map revision, current commit, modified or untracked source, dirty Editor packages, Engine build/version, plugins, platform, test operator, GPU/CPU/RAM, renderer, resolution and graphics preset. Keep private paths local. Never run an automated Editor operation in another agent's active unsaved session.

| Layer | Owner | Observable evidence | Not equivalent to |
| --- | --- | --- | --- |
| Source/UHT/UBT | C++ owner | Actual target exit status, complete compilation errors | Feature behavior |
| Native automation | C++ owner | Individual assertions/cases from real automation results | Editor asset wiring |
| Asset/Editor | Exclusive Unreal operator | Saved/reopened assets, Blueprint/Niagara/Seq compile, reference checks | Gameplay route |
| Functional automation | Editor owner | Real map and Actor behavior observed in an automation map | Human game feel |
| Manual PIE | Human director | Actual inputs, expected/actual state, captures and failures | Packaged standalone |
| Fresh-process test | Human/test operator | Save, exit, relaunch, load and world projection | In-memory PIE restart |
| Standalone/package | Release owner | Correct executable launched independently | Shipping acceptance by itself |
| Profiling | QA/tech owner | Captured frame traces and measured bottleneck | A visual guess |
| Final acceptance | Human director | Version-specific yes/no decision | Agent confidence |

Use **PASS, FAIL, BLOCKED, NOT RUN** with the exact evidence link/path per case. An engine process's exit code 0 is not a global PASS when actual test assertions fail or warnings indicate broken game behavior.

## 2. Native C++ test using the already implemented T07 validator

The repository includes an original [native test source](../examples/native-validation-tests/Source/GPPDemoSnapshotValidationTest.cpp). It **calls the real existing T07** static method UGPPDemoRunSubsystem::ValidateSnapshot instead of cloning its business logic or adding test-only production stubs.

The test constructs real transient SaveGame objects using NewObject and exercises **eight assertions**: a valid Owned snapshot; null rejection; incorrect version rejection; duplicate collected ID rejection; valid Donated state; invalid uncollected Recovery reference; reused offering/recovery ID rejection; valid Recovered state.

To integrate with the existing game module:

1. Inspect current test infrastructure; if the project already tests SaveGame validation, extend those tests rather than install another near-duplicate.
2. Confirm the existing T07 classes and test target are in the same accessible module. Add the test CPP to the current game or test module and include correct public/private include paths. Do **not** create a blank module or copy another private game's source.
3. Use the existing module dependencies for Core/Engine and inspect whether tests need the installed Engine's Automation facilities. The source is guarded by WITH_DEV_AUTOMATION_TESTS and uses IMPLEMENT_SIMPLE_AUTOMATION_TEST.
4. Compile the Win64 Editor target with [T02 actual build wrapper](../examples/unreal-local-build/scripts/Build-Editor.ps1). Record real UHT/UBT logs and errors.
5. Open Unreal Editor with the sole authorized Editor operator; enable supported testing plugins if required. In **Session Frontend / Automation**, find the test **GPP.Persistence.Snapshot.Validation** and run it. Examine **every assertion** and recorded failure, not merely the overall process exit.
6. If adapting for command-line execution, verify the installed 5.9 Automation command syntax, test filter, shutdown conditions and report-output options. Do not blindly issue an immediate Quit command after scheduling asynchronous tests. Preserve logs and inspect the actual report status for all expected cases.

**Boundary:** This native test does not prove slot A/B filesystem I/O, async save readback, collision projection, input, Niagara, Sequencer, Windows packaging or human playability. Those require their own layers.

## 3. Real Editor and asset validation

The sole Editor/MCP operator audits the existing map and saves only explicitly owned assets. Inspect UHT-reflected C++ classes, Blueprint compile results, references and required plugins, then:

1. Reopen the map and confirm the actual project GameplayMode, PlayerStart, pawn/Controller and expected Enhanced Input mapping assets.
2. Check the real gameplay Actor IDs, collision profiles, fragment pickup geometry and line-of-sight traces.
3. Inspect the real [T15 Niagara System and presenter](niagara-chaos-deterministic-restoration.md), and the optional Chaos cache if actually authored. A C++ NiagaraComponent without a saved System is **not** an integrated VFX.
4. Inspect the real T16 Level Sequence Actor, Camera Cuts, skipped-cinematic path and event bindings. A saved sequence alone is **not** successful camera handoff.
5. Inspect T17 imported SoundWaves/MetaSounds, sound class/mix, attenuation and proper legal provenance; silent or unlicensed sounds fail.
6. Compile changed Blueprints, inspect Map Check / Output Log and resolve missing references. Close/reopen and check actual saved packages. Do not directly edit .uasset/.umap binary files.
7. If automation or commandlets cannot operate an Editor-only authoring tool in the installed version, record BLOCKED and use a real supported Editor/manual method; do not invent MCP tools.

## 4. Human PIE: follow the critical route from player input

Provide the human reviewer the exact current project/map and a short input-action sheet. Do not suggest that automated UI or native tests substitute for a human exercising the game.

Run a fresh PIE session with these observations, in order:

1. **Start:** main menu/New Game loads the correct map; player possesses intended pawn; camera and HUD show correctly.
2. **Movement:** move around real obstacles, verify capsule/camera collision and no traversal through walls or softlocking geometry.
3. **Collect:** approach a fragment with line of sight, observe context cue, interact once; reject duplicate pickup and blocked interaction.
4. **Donate:** hold the proper action; cancel/release halfway, leave range and switch targets; then complete exactly one valid donation and confirm real ability loss/updated gate state.
5. **Restore:** observe T15 geometry and Niagara if authored; new gate is physically passable; animation duration/skip does not control gate collision.
6. **Cinematic:** T16 triggers once on fresh commit; run normal finish, restart and test Skip; camera, input and HUD return correctly.
7. **Sound:** T17 cue sounds once and ambience reflects current state; mute or change volume through actual settings.
8. **Recovery:** collect the distinct recovery fragment and verify the correct ability state without replaying reconstruction.
9. **Save/Load:** save, close the process, restart, load and verify ability, checkpoint safety, collected IDs, world collision, audio and absence of one-shot replays.
10. **Ending:** traverse the rest of the approved critical path, ending and any playable return/epilogue. Confirm no softlock, dead buttons or missing map transitions.

A full implementation must also test incompatible/corrupted save files safely using disposable copied slots and intentional backup; never damage the player's only actual save. Preserve exact reproduction steps and screenshots/video where relevant.

## 5. Native, functional and smoke tests are not synonyms

Use Epic's Automation Test Framework for C++ assertions and regression. Use the Functional Testing framework/Automation-compatible map for tests that need real Actors, collision and world ticking. Keep **fast smoke** tests genuinely fast. If using screenshot comparison, preserve same scene/camera/render platform and tolerances; rendering changes can produce legitimate pixel differences.

At least one test must assert a *rejected* transaction, not only the happy path. For persistence, add actual fresh-process and streamed-Actor tests at higher layers. For cinematics, verify return on both OnFinished and Skip/OnStop. For audio, record a real audible or device-captured signal and game-state timing.

Do not report "all tests pass" based solely on UBT success, an empty test discovery list or an Editor that opened without crashing.

## 6. Profile measured gameplay, not assumptions

1. Pick a repeatable representative route segment and hardware profile. Record resolution, screen percentage, graphics settings, RHI, window mode and whether shaders are warm.
2. Use Unreal's actual profiling utilities: **Unreal Insights**, **stat unit**, **stat fps**, and when relevant **stat gpu**, Niagara profiling and memory/streaming tools. Check which console commands are available in the build configuration.
3. Record idle, movement, fragment interaction, initial restoration, VFX peak, cinematic camera movement, checkpoint load and streaming transitions. Inspect the *same* route with effects enabled and disabled if comparing cost.
4. Capture Game/Render/RHI/GPU frame times and stutters (p95/p99 when available), not only average FPS. Averages can hide an interaction freeze.
5. Attribute real bottlenecks: Niagara emitter count/bounds, Chaos bodies, shadow/light changes, mesh/material draw calls, animated components, audio/concurrency, UI invalidation, World Partition loads or save I/O.
6. Optimize an identified bottleneck, then rerun the exact capture and human visual acceptance. Document baseline, change and outcome; do not invent percentage gains.

A useful benchmark defines a **target** (for example 60 FPS at an approved configuration) and **measures** whether it is met. Declaring a target is not proof of measured performance.

## 7. Verification matrix — 27 checks

| ID | Observable check |
| --- | --- |
| Q01 | Build and engine/hardware identity recorded |
| Q02 | Git changes and dirty Editor assets inspected |
| Q03 | UHT/UBT Editor target returns actual successful build |
| Q04 | Native validation test is discovered, not silently filtered out |
| Q05 | All eight native assertions pass in actual UE execution |
| Q06 | Native negative cases fail validation correctly |
| Q07 | Actual Blueprint assets compile and reopen |
| Q08 | All required meshes, audio, Niagara, Sequence assets resolve |
| Q09 | Real input mapping and pawn/controller initialize |
| Q10 | Pickup blocks duplicates and out-of-range attempts |
| Q11 | Donation cancellation does not commit progress |
| Q12 | Successful donation updates durable state once |
| Q13 | Gate collision opens before visual completion |
| Q14 | Niagara presentation does not replay on reload |
| Q15 | T16 cinematic returns camera and input normally |
| Q16 | T16 Skip and interruption restore control |
| Q17 | T17 audio reacts once and settings apply |
| Q18 | Recovery cannot duplicate consumed fragment |
| Q19 | Fresh-process save/load restores correct abilities and world |
| Q20 | Stream-out/in projects state without replay/softlock |
| Q21 | UI/menus/settings navigation works with actual input |
| Q22 | Human plays full approved critical route |
| Q23 | Standalone package launches outside the Editor |
| Q24 | Reproducible frame-time capture with hardware/settings |
| Q25 | Bottlenecks are recorded rather than guessed |
| Q26 | Post-optimization recheck if any changes are made |
| Q27 | Human director records explicit final acceptance decision |

## 8. Failures and corrective loop

For any FAIL: preserve the first failing evidence; isolate minimal reproduction; assign one owner; fix the **real** code/asset, not its report; compile, repeat affected tests and relevant regressions; obtain human reacceptance. For BLOCKED: identify exact missing external dependency/tool/version or required approval; don't treat it as PASS. For NOT RUN: record why and leave the release gate open.

Use the [existing Test and Acceptance Plan](../templates/test-plan.md) as the canonical per-project record. Keep raw local logs and small privacy-safe summarized evidence in published materials as appropriate.

**Official references:** [Epic Automation Test Framework](https://dev.epicgames.com/documentation/unreal-engine/automation-test-framework-in-unreal-engine), [Epic Functional Testing](https://dev.epicgames.com/documentation/unreal-engine/functional-testing-in-unreal-engine), [Epic Unreal Insights](https://dev.epicgames.com/documentation/unreal-engine/unreal-insights-in-unreal-engine), [Epic Testing and Optimizing](https://dev.epicgames.com/documentation/unreal-engine/testing-and-optimizing-your-content), and [T02 build wrapper](unreal-project-bootstrap-local-build.md). Verify exact UE 5.9 commandlet flags and plugin/tool support before use.

**Next:** [T19 — Packaging, Real Gameplay Capture and Release](windows-packaging-gameplay-capture-release.md).
