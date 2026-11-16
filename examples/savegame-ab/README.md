# Unreal Engine — Versioned SaveGame and Alternating A/B Slots

**Example status:** Original educational C++ source implementing a real **logical-state** SaveGame subsystem and A/B slot I/O. It has been reviewed against public Epic SaveGame APIs but **has not been compiled in an installed UE 5.9 project, integrated with an Editor map, tested in PIE or tested in a packaged build**. Do not report those unrun validations as PASS.

**Full integration tutorial:** [T07 — SaveGame, Reload and World Partition](../../tutorials/savegame-reload-world-partition.md).

## Provided files

| File | Real behavior |
| --- | --- |
| [`GPPDemoRunSaveGame.h`](Source/GPPDemoRunSaveGame.h) | UHT-reflected `USaveGame` class, versioned snapshot, fragment IDs, powered beacon ID, Light stage and safe-checkpoint candidate |
| [`GPPDemoRunSubsystem.h`](Source/GPPDemoRunSubsystem.h) | GameInstance subsystem API, owner of logical transitions, progress and save-completion delegates |
| [`GPPDemoRunSubsystem.cpp`](Source/GPPDemoRunSubsystem.cpp) | Real validation, nonduplicating stage changes, async A/B save, readback checks, highest-valid-generation load and error handling |
| [`Inspect-SaveSlots.ps1`](scripts/Inspect-SaveSlots.ps1) | Read-only PowerShell 7 inspection: file presence, size, UTC timestamp and SHA-256 |

There is **no empty `.cpp` file** for the data-only SaveGame class; all its reflected fields and defaults are defined in its header.

All educational code here is offered under the repository's [MIT code license](../../LICENSE-CODE.md). No private gameplay source, proprietary Unreal Engine source or binary Unreal content is shipped.

## Scope and boundary of the example

**Implemented in the provided C++ source:** NewRun logical initialization; unique Offering collection; one permanent Light donation to a named beacon; unique Recovery collection and stage change; checkpoint value recording; field and state validation; two alternating SaveGame slots; async save callback with readback and generation verification; a logical load selector; state-change notifications.

**Not implemented by these files:** the Editor-authorized playable map; input/UI; focused LOS/hold interaction; integration of these calls with the T05 Character and T06 fragment actors; streaming actor registration; checkpoint capsule/floor safety checks; loading-screen orchestration; ability grant/tag reconciliation after a fresh process; compiled tests; packaged Windows execution. These are **mandatory steps for a full real game feature**, not stubs to be counted as completion.

The public [T05 demo](../gas-light-demo/README.md) intentionally uses a Character-owned stage. Do not install this GameInstance subsystem and leave that Character stage as a separate writable authority. **Migrate the existing functionality into one progression owner**, or use an already-working owned subsystem instead of this sample.

## Add to a real C++ game

1. Inspect the game's existing `USaveGame`, GameInstance/Save services, progression owner, `AGPPLightCharacter` and interaction Actors. If the game has a functioning equivalent, **reuse it**; copying these files would create competing systems.
2. If needed, place the **three C++ source files** into the game's existing game module. Do not add a separate blank module. Confirm the Build.cs contains the existing `Core`, `CoreUObject` and `Engine` dependencies and use correct includes/UHT naming for that installed UE build.
3. Compile the real Win64 Editor target using the [T02 local build pipeline](../../tutorials/unreal-project-bootstrap-local-build.md). Record the actual UBT/UHT output. An unrun compile remains NOT RUN.
4. In the game-specific, single-player progression startup, choose **New Game → StartNewRun** or **Continue → ReadBestSnapshotIntoMemory**, then carry out the required world/map/checkpoint reconciliation before accepting player input.
5. Connect the existing focused interaction, confirmed donation and Recovery fragment callbacks to `TryCollectOffering`, `TryDonateLight` and `TryCollectRecovery`. The interaction service remains responsible for LOS, range, selection and hold completion; this narrow subsystem cannot check Editor scene geometry.
6. Give all real actors stable Editor-assigned IDs. On each actor's BeginPlay/re-registration and on `OnProgressChanged`, use `IsFragmentCollected`/`IsBeaconPowered` and actually enable/disable collision, lighting and interaction.
7. Reconcile GAS's derived `Demo.State.Light.Donated` tag and grant/eligibility after pawn creation, new run and logical load. Do not serialize a GAS ability handle.
8. Provide a real Save UI invoking `RequestAsyncSave`, and check `OnSaveCompleted` for **snapshot readback success**. If more gameplay occurred during an async save, mark it dirty and request a later save.
9. Add the actual map-loading, streaming completion, safe-spawn validation, reapplication and input restoration to the game's existing load coordinator. `ReadBestSnapshotIntoMemory` alone is **not successful Continue**.
10. Run the complete human PIE and packaged-game test matrix in T07. Only real checks with evidence may be marked PASS.

### Small UMG / Blueprint call contract

- Acquire the GameInstance subsystem with `Get Game Instance Subsystem` and select `GPPDemoRunSubsystem`.
- New Game: after explicit user approval and safe starting map/checkpoint selection, call `StartNewRun`; observe true/false.
- Save: only while gameplay is in a safe committed state, call `RequestAsyncSave`. Subscribe to `OnSaveCompleted`; show an actual save error on false. Do not call Save twice while `IsSaveInProgress` is true.
- Continue: call `ReadBestSnapshotIntoMemory` only during the real loading flow; validate map/checkpoint, stream/reconcile, then enable controls.
- In world Actors: subscribe to `OnProgressChanged` and also immediately apply the current snapshot because an Actor may appear after load or stream-in.

### Example real state flow

```text
StartNewRun(MapId, CheckpointId, SafePosition)
    -> TryCollectOffering("Fragment.Offering.001") [true once]
    -> TryDonateLight("Landmark.Beacon.001")   [true once]
    -> RequestAsyncSave()                      [one snapshot, slot A]
    -> OnSaveCompleted(true)                    [write + readback validated]
    -> fresh process / ReadBestSnapshotIntoMemory()
    -> world coordinator loads correct map and required cells
    -> pawn reconciles ASC and actors ApplyState
    -> TryCollectRecovery("Fragment.Recovery.001")
    -> RequestAsyncSave()                      [slot B; A retained]
    -> fresh process / Continue
    -> Light = Recovered, beacon still powered, fragments consumed
```

The sequence is **the intended real exercise**. It is not an execution log from this repository.

## Slot evidence tool (read-only)

Run the included PowerShell 7 script on a local project's **actual save directory**, not an assumed path:

```powershell
pwsh -File ./examples/savegame-ab/scripts/Inspect-SaveSlots.ps1 \
    -SaveDirectory './Saved/SaveGames' \
    -SlotNames 'GPP_Run_A','GPP_Run_B'
```

For a separate JSON file, first create/select an existing writable report directory and pass `-OutputJson './artifacts/save-slot-evidence.json'`. The tool **does not delete, change, deserialize or validate** Unreal save slots. It reports only file-level evidence. A successful hash alone does not establish a valid game state.

## Failure cases and exact recovery expectation

| Case | Expected implementation response |
| --- | --- |
| No previous slots | New Game can start; Continue is unavailable |
| One slot valid, one invalid | Continue chooses valid slot; later writes target the other |
| Both slots invalid | Refuse silent overwrite; preserve files for recovery |
| Duplicate IDs | Reject second logical collection; real map authoring must also catch duplicate placed actor IDs |
| Unknown schema | Reject; add a **tested** migration if supported |
| Save in progress | Do not start overlapping writes |
| Save callback failure or readback mismatch | No success report; previous valid backup preserved |
| Same generation in both slots | Refuse ambiguous selection rather than guessing |
| Player moved while save in flight | Only frozen snapshot is saved; newly changed live state requires another save |
| Streamed Actor reappears | Reapply persistent state; no duplicate reward/effect |
| Missing/unsafe checkpoint | Safe fallback or explicit Continue failure; do not teleport blindly |

## Verification boundaries

Review and API lookup are **not** compile, PIE, map integration, gameplay acceptance or a fresh-process packaged test. The **actual local test results are NOT RUN** until a reader executes them. See [T07 test matrix](../../tutorials/savegame-reload-world-partition.md#11-exact-humanautomated-acceptance-scenarios).

The T07 code deliberately targets a **small single-player demo snapshot**. Large saves need measured I/O and potentially async loading. Multiplayer requires server authority, replication and persistence rules not present in these files.
