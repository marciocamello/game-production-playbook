# Template — Testing and Acceptance Plan

## Identify the build under test

Record the game/feature revision, engine version, target platform, map, test runner, graphics context and real tester.

## Evidence matrix

| Layer | Check | Record |
| --- | --- | --- |
| Native | Target compile/link, cases and relevant regressions | Real command, build/test log, failures |
| Editor | Import/Blueprint/asset validation, map save/reload | Actual inspected packages and output |
| PIE | Player input, controls, camera, interaction, level traversal | Human observations and step sequence |
| Persistence | Save, process exit/restart, stream-out/in, no replay of one-time events | Actual snapshots and observed state |
| Presentation | Real UI/Audio/VFX/Sequencer runtime behavior | Manual checks with known conditions |
| Packaging | Cook/package result and external executable launch | Logs, target, actual launch and play |
| Release | Public download, installation, permissions and credits | URL, checksum if useful, launch check |

## Outcome vocabulary

Use **PASS** only for executed checks that met the stated assertion. Use **FAIL** for executed checks with observed failures; **BLOCKED** for checks prevented by a stated external limitation; **NOT RUN** for unexecuted checks. Do not silently omit test cases or let a successful process exit overwrite individual failed cases.

## Human game acceptance

The designated reviewer follows the actual critical path and records: inputs attempted, expected/actual results, defects, screenshots/video when permitted, patch/retest evidence, and acceptance decision. Initial Editor screenshots are not runtime acceptance.

## Release regression

After late UI, animation, level or VFX changes, rerun the end-to-end route in both PIE and the packaged build. A previous success from a different revision is historical evidence, not proof of the final executable.