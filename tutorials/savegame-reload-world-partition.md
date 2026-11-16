# T07 — SaveGame, Reload and World Partition: Durable State Without Replaying the World

**Publication status:** Complete technical tutorial, including an [original four-file C++ example](../examples/savegame-ab/README.md) (one SaveGame header, two subsystem files, one PowerShell evidence tool). The example was reviewed against public Unreal API documentation, but **has not been compiled in an installed UE 5.9, connected to a production map, run in PIE or launched as a packaged game by the playbook maintainers**. All actual execution checks begin **NOT RUN**.

**Target:** Unreal Engine 5.9, C++ + Blueprints, single-player Windows. The exact installed engine headers and runtime behavior are the final authority for any version-sensitive detail.

**Player-visible result:** the player collects a fragment, voluntarily donates a Light ability, restores a landmark, collects a distinct Recovery fragment and saves. After the process is closed and launched again, the game resumes at a **physically safe checkpoint** with the right fragment visibility, restored landmark/collision, ability availability, UI and traversal route. Streaming that region out and in must not respawn consumed items or replay one-time effects.

The full sample code is under [`examples/savegame-ab/Source/`](../examples/savegame-ab/README.md). The code supplies the **persistent logical-state owner and A/B slot operations**. It is **not an already integrated T05/T06 game**. A real game must connect gameplay, the pawn/ASC, Editor-authored Actors, checkpoint validation and loading UI as described below.

## 1. Read the actual project before modifying persistence

T05's [native Character](../examples/gas-light-demo/Source/GPPLightCharacter.cpp) owns its Light stage for an intentionally short-lived session; T06's [interaction tutorial](contextual-interactions-pickups-sacrifices.md) introduces author-assigned Offering and Recovery IDs and a cancelable sacrifice. Those designs deliberately **do not** save/restore across a fresh process. Before adding new code, inspect the target game's existing:

- `UGameInstanceSubsystem`/progression service, current SaveGame classes and slots;
- Character/ASC initialization, Gameplay Tag/ability grants and any `LightStage` enum;
- checkpoint rules, player transforms, spawn/possession ordering, input locking and loading UI;
- existing fragment/landmark IDs, pickups, restored barriers and world-state projection methods;
- World Partition settings, streaming sources, Data Layers and whether key controller Actors are always loaded;
- repository/Editor status, unsaved packages, available local UE 5.9 build tools and assigned file owners.

**Reuse comes first.** If a durable run subsystem already owns progression, add serialization and missing invariants to it. Do **not** install this tutorial's `UGPPDemoRunSubsystem` alongside another authoritative progression manager. If T05 was installed verbatim, its Character must stop owning the definitive enum when migrating to a persistent subsystem: the Character's `ApplyLightStage` becomes a **projection of the subsystem's stage**, not a conflicting second decision maker.

## 2. Official references and API limits

- [Epic — Saving and Loading Your Game](https://dev.epicgames.com/documentation/unreal-engine/saving-and-loading-your-game-in-unreal-engine) — `USaveGame`, `UGameplayStatics`, async and sync save/load, platform behavior.
- [Epic API — AsyncSaveGameToSlot](https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Runtime/Engine/UGameplayStatics/AsyncSaveGameToSlot) — caller-supplied delegate; completion reports slot, user index and success.
- [Epic API — AsyncLoadGameFromSlot](https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Runtime/Engine/UGameplayStatics/AsyncLoadGameFromSlot) — asynchronous disk read then game-thread object creation/callback.
- [Epic — World Partition](https://dev.epicgames.com/documentation/en-us/unreal-engine/world-partition-in-unreal-engine) — automatic streaming, sources, spatially loaded Actors and `Is Streaming Completed`.
- [Epic — Gameplay Ability System](https://dev.epicgames.com/documentation/unreal-engine/understanding-the-unreal-engine-gameplay-ability-system) — reconcile runtime ability availability from saved logical state.
- [T02 — Project Bootstrap and Local Build](unreal-project-bootstrap-local-build.md) — compile and Windows packaging as separate gates.

**Documentation version caution:** Epic documentation pages can resolve to a different selected engine version. Inspect signatures in the **installed UE 5.9 headers** when adapting code. A web page, this source example and an AI review do not prove that a project builds.

Epic recommends `AsyncSaveGameToSlot` for saves during gameplay to reduce blocking. A small synchronous `LoadGameFromSlot` can still be useful for startup checks/readback; large projects may need an async load coordinator and loading screen. The UE save API's successful callback **does not, by itself, establish filesystem-level atomicity, schema compatibility, playable-map readiness or crash-safe checkpoint restoration**.

## 3. Separate logical authority, serialized snapshot and world projection

| Layer | Owner | Responsibilities | Must not store as authority |
| --- | --- | --- | --- |
| Live run | Existing run/progression subsystem | Unique collected IDs, Light stage, permanent landmark state, checkpoint, run identity | A widget's text or a streamed Actor's visibility |
| Save snapshot | Versioned `USaveGame` subclass | Value-only copy of committed live state and generation | `AActor*`, transient world pointers, active GAS handles, timers |
| Save coordinator | Existing run subsystem or dedicated owned service | A/B slot selection, serialization, completion, validation, fallback | A second copy of gameplay decisions |
| ASC/Character | Pawn and GAS | Reconcile abilities/tags, local effects and movement from live state | Independent loaded ability-history truth |
| World Actors | Pickup, beacon, bridge/gate | Project persisted state as mesh/collision/light/material/audio | Grant collectible twice when loaded |
| Load coordinator | GameInstance/PlayerController-level flow | Load correct map, wait for region, validate safe checkpoint, place pawn, enable control | Declare “Continue ready” after bytes read only |
| UMG/MVVM | View of live run/IO operation | Save/Load progress, buttons, failures and visible ability prompts | An uncommitted SaveGame or duplicate stage variable |

The most important distinction is:

**`Deserialize` changes memory. `ApplyState` makes the world consistent. `PlayTransition` is only for a freshly committed player action.**

A restored gate must have correct *collision*, not only its material. A consumed fragment must be hidden and lose interaction collision after stream-in. SaveGame must not silently respawn an item that the player already used.

## 4. Define and validate a versioned snapshot

The [source example](../examples/savegame-ab/Source/GPPDemoRunSaveGame.h) declares a real `UGPPDemoRunSaveGame : USaveGame` class with these fields:

| Field | Meaning / validation |
| --- | --- |
| `FormatVersion` | Known schema version; unknown incompatible versions are rejected, not guessed |
| `Generation` | Monotonically increasing slot generation; choose newest valid snapshot |
| `RunId` | Stable ID for one playthrough, regenerated only by intentional New Game |
| `MapId` | Authored map identity; compare with available map list before Continue |
| `CheckpointId`, `CheckpointPosition` | Authored safe checkpoint and last candidate position; never trust location alone |
| `CollectedFragmentIds` | Distinct, nonempty author-assigned logical IDs; no Actor references |
| `OfferingFragmentId`, `RecoveryFragmentId` | Semantic identities of the two fragment roles in this teaching exercise |
| `PoweredBeaconId` | World restoration identity; remains powered after personal recovery |
| `LightStage` | Owned → Donated → Recovered, with no illegal reverse transition |

The example implements `ValidateSnapshot`: it verifies nonempty identities, duplicate collected IDs, stage/fragment/beacon consistency, finite checkpoint position, schema and generation. **Its state validation is intentionally narrower than a real game:** the load coordinator must additionally verify that the map exists in the cooked build, the checkpoint Actor exists or has a safe approved fallback, its collision is walkable, the player capsule fits and the required streaming region is active.

Do **not** serialize raw `AActor*` pointers, `TWeakObjectPtr` values, dynamic widget state, `FGameplayAbilitySpecHandle`, timers or runtime instance names. Their values are not stable game progression identifiers.

### Why `SaveGame` property flags are not the entire solution

The sample marks fields with `UPROPERTY(SaveGame)` for clarity. Unreal's ordinary `SaveGameToSlot` writes non-transient properties of the `USaveGame` object; it does **not** automatically discover every level Actor tagged `SaveGame`. Actor state must be **explicitly captured into the logical snapshot** by the project.

Data schema changes require a versioning/migration policy. A supported older version may be migrated by an actual, tested migration function; unrecognized formats must be rejected cleanly. Do not claim a migration exists because the number is present in the file.

## 5. Use an actual, single-owner state machine

The sample's [`UGPPDemoRunSubsystem`](../examples/savegame-ab/Source/GPPDemoRunSubsystem.cpp) is an original in-memory authority for the **narrow three-stage Light exercise**.

| Call | Actual result when eligible |
| --- | --- |
| `StartNewRun(MapId, CheckpointId, Position)` | Creates a new in-memory run ID and empty progress; does **not** erase old slots |
| `TryCollectOffering(FragmentId)` | Records first Offering ID once |
| `TryDonateLight(BeaconId)` | Requires Offering and Owned Light; stores beacon ID and Donated stage |
| `TryCollectRecovery(FragmentId)` | Requires Donated Light; records distinct Recovery ID and Recovered stage |
| `TryUpdateCheckpoint(Id, Position)` | Stores an authored checkpoint candidate for later map/physics validation |
| `IsFragmentCollected(Id)` | Queries collected state to project physical pickup visibility |
| `IsBeaconPowered(Id)` | Queries irreversible restoration to project actual landmark state |
| `GetLightStage()` | Supplies stage for ASC, input and UMG projection |
| `RequestAsyncSave()` | Freezes state to a snapshot and writes the non-latest A/B slot |
| `ReadBestSnapshotIntoMemory()` | Loads highest-generation valid snapshot, without claiming world readiness |

**The transaction is not triggered by saving.** T06's LOS/range/hold checks authorize the donation, then call `TryDonateLight`. On success, the pawn reconciles its ASC and the Editor Actor applies beacon state. Save simply serializes the already committed result.

### Required migration from T05

The T05 C++ code currently updates its own `LightStage` and `PoweredBeacon` on E/R. When integrating this subsystem in a real project, modify those exact paths:

1. Replace the Character's definitive stage queries/changes with the live run subsystem. Do not retain two writable stage fields.
2. Remove direct donation/recovery key bypasses as specified in T06; route actions through the validated focused target and confirmation transaction.
3. Call `TryCollectOffering` from the actual Offering pickup; `TryDonateLight` only after uninterrupted hold; `TryCollectRecovery` from the distinct Recovery pickup.
4. On `OnProgressChanged`, reconcile `Demo.State.Light.Donated` and personal Light availability using the existing ASC and ability methods.
5. Give/remove/reconcile ability specs **only as required by that saved stage**, rather than serializing grant handles.
6. Make existing beacon/pickup Actors read the subsystem instead of trusting the Character's previous raw Actor pointer.

The source sample alone does not modify the published T05 files. The Editor and source owners must perform that migration and validate it before describing a finished T07 feature.

## 6. Durable A/B slots: write the inactive generation, validate and fall back

A single slot may be enough for a small prototype, but this tutorial's real example implements two alternating slots, **`GPP_Run_A` and `GPP_Run_B`**, with increasing `Generation`.

### Save sequence

1. Reject save if no valid active run, if a save is already active or if the game is currently in a protected irreversible transition.
2. Inspect existing A/B slots and validate any readable snapshot.
3. If both slots exist but neither is valid, **preserve them** for explicit recovery. Do not overwrite ambiguous data automatically.
4. Choose the *other* slot relative to the highest-generation valid one; with no files, start at A.
5. Copy the current logical run into a **separate snapshot object**; assign a greater generation. Do not hand a mutable live state object to an asynchronous disk operation.
6. Invoke `UGameplayStatics::AsyncSaveGameToSlot` and prevent overlapping writes.
7. On callback, inspect the returned success, slot and user index.
8. Read the new slot back, validate schema/invariants and verify the saved generation and run ID.
9. Only then report that the **snapshot** was durably accepted; clear the in-flight reference and allow another save.
10. Preserve the previous valid slot as a recovery fallback.

The [C++ source](../examples/savegame-ab/Source/GPPDemoRunSubsystem.cpp) implements these operations with a `UPROPERTY(Transient)` reference to the in-flight snapshot so garbage collection cannot remove it during an async save.

**A/B limits:** alternating ordinary UE slot saves is **not a filesystem atomic-rename protocol**, does not guarantee against power loss, and does not automatically back up to cloud storage. A readback validates serialization and logical fields, not a full fresh-process gameplay restoration. The sample reads small save objects synchronously for validation; measure this before using it for large progression data.

### Save while the player keeps moving

The snapshot freezes at the moment `RequestAsyncSave` is invoked. If the player progresses further before completion, the callback can truthfully report **that snapshot saved** while **later actions remain unsaved**. The production UI must track live-state revision versus saved revision (or explicit dirty state) and request a later save. Do not label the entire current run “fully saved” merely because the last asynchronous operation completed.

For very large saves, a production coordinator should avoid repeated synchronous validation reads on the gameplay thread, and may use `AsyncLoadGameFromSlot` for the startup scan. Do not overlap platform save/load operations that the target platform does not support.

## 7. Continue/load: logical bytes are not playable readiness

Read A and B, reject invalid or incompatible candidates, select the highest valid generation, and fall back to the other valid slot. The published C++ `ReadBestSnapshotIntoMemory` performs this *logical* selection. If two valid snapshots claim an ambiguous equal generation, it fails rather than guessing.

A **real Continue operation** has additional mandatory steps:

1. Disable gameplay interaction, show an actual loading UI, and prevent save reentry.
2. Read and validate the snapshot **without altering the current run** if both slots fail.
3. Confirm `MapId` maps to an available cooked level and load that level.
4. Ensure the checkpoint region has a usable streaming source; wait for required World Partition cells and progression-critical actors.
5. Validate the authored checkpoint ID, accessible route and candidate transform (floor and capsule sweep). **Never teleport blindly to a serialized coordinate**.
6. Hydrate the run subsystem from the candidate. If the world cannot reconcile, fail Continue gracefully instead of enabling a half-initialized pawn.
7. Initialize/reconcile ASC actor info, granted abilities and derived Gameplay Tags from the loaded stage.
8. Project each available pickup/landmark via idempotent `ApplyState`, including real gate collision, ability effects and interaction eligibility.
9. Place/possess the pawn at the verified safe point; update UMG/MVVM from the authoritative run.
10. Enable player control **only after** mandatory world state is consistent. The handoff report separately records "SaveGame object loaded" and "playable state ready".

The C++ example **does not implement steps 3–10**; these depend on the actual map, checkpoint Actors, collision and game-specific pawn. This is an explicit integration requirement, not an optional polish task.

### New Game and options

**New Game** deliberately initializes a fresh run; it must not silently delete existing files before the player confirms. **Continue** must only be enabled when a compatible snapshot can be loaded **and** its map/checkpoint can be resolved. User options such as resolution, audio volumes and input remapping belong to their existing settings APIs/storage and must **not be reset** by New Game.

## 8. World Partition: apply state when actors become available

World Partition streams Actors according to streaming sources, spatial-loading configuration and Data Layers. Some templates have streaming disabled despite using World Partition; **check the real World Settings** before asserting your stream-out/stream-in test exercised anything.

### Author explicit IDs

In the Editor, assign stable, unique IDs to each required Actor:

- Offering pickup: `Fragment.Offering.001`
- Recovery pickup: `Fragment.Recovery.001`
- Restorable beacon: `Landmark.Beacon.001`
- Checkpoint: `Checkpoint.Return.001`

These are educational ID examples. Set them as instance-editable values on the actual actors. Validate uniqueness across each logical namespace and report **duplicate IDs as a blocking authoring error**, including cases in independently loaded World Partition cells. Do not call `FGuid::NewGuid()` in every Actor `BeginPlay` and expect persistent identity across processes.

### Idempotent actor registration and reapplication

For a fragment Actor, on `BeginPlay`/registration and every relevant run change:

1. Read the unique `FragmentId` from the placed instance.
2. Ask the **existing live progress subsystem** `IsFragmentCollected(FragmentId)`.
3. If collected: hide its mesh, disable relevant overlap/interaction collision and unregister it as a focus candidate.
4. If uncollected: show the mesh and enable its authored collision/interaction behavior.
5. Do not award the fragment from `BeginPlay`; grant it only when the authoritative `TryCollect*` transaction succeeds.

For the restorable beacon:

1. Read `PersistentBeaconId` from the placed instance.
2. Ask `IsBeaconPowered(PersistentBeaconId)`.
3. Call the **existing** `AGPPLightBeacon::ApplyPoweredState(Powered)`.
4. Reapply its physically functioning gate/passage state in the actual game's landmark Actor, if restoration changes traversal.
5. Never replay a one-shot Niagara/Sequencer restoration simply because `BeginPlay` ran again.

Bind to `OnProgressChanged` after registering the Actor and immediately call its real `ApplyState`. Unbind on `EndPlay`, invalidate weak references and cancel timers that referenced streamed-out Actors. Ensure late-spawning actors also query the current subsystem. One broadcast in the persistent service is not enough to update an Actor that was unloaded at the time.

**Do not store raw pointers to critical, streamable Actors in the SaveGame.** Resident lightweight controllers may be useful, but make an explicit load/stream decision: `Is Spatially Loaded = false` can keep actors resident (subject to Data Layers), whereas distant decorative meshes/pickups can remain streamable. Avoid hard references from always-loaded controllers that force distant geometry to remain loaded.

### Streaming readiness

An authored World Partition Streaming Source component can request the checkpoint region and expose **`Is Streaming Completed`**, per Epic's documentation. Completion of a selected streaming source is one prerequisite for placement, not proof that the checkpoint is safe or every gameplay dependency exists. Follow up with explicit actor registration and actual collision tests.

## 9. Editor production and source ownership

The code owner:

1. Audits existing GameInstance, run/persistence, save UI and T05/T06 methods. Reuses before adding.
2. Adds the example's **three** C++ files only if the game lacks equivalent classes: [snapshot header](../examples/savegame-ab/Source/GPPDemoRunSaveGame.h), [subsystem header](../examples/savegame-ab/Source/GPPDemoRunSubsystem.h), [subsystem implementation](../examples/savegame-ab/Source/GPPDemoRunSubsystem.cpp).
3. Confirms the existing runtime module's `Engine`, `Core` and `CoreUObject` dependencies (no extra save plugin required). Uses the installed engine's UHT/UBT and C++ headers. Records actual compile results.
4. Migrates the Character/T06 target transaction into the **single** subsystem authority, adds durable actor registration, reapplication and save/load coordination.
5. Exposes actual menu commands for New Game, Continue and Save, including failure reasons and in-flight states. No fake clickable controls.

The sole Editor/MCP operator:

1. Inspects dirty packages and existing maps, character Blueprint, Input Actions, beacon, fragments and checkpoints.
2. Authors/updates the real fragment IDs and checkpoint placements, with functional collision and a physically safe recovery path.
3. Applies World Partition streaming flags and streaming source(s) appropriate for that map, verifying that stream-in actually happens.
4. Integrates the game-mode/pawn/ASC projections, input gating, progress widget and real loading feedback.
5. Compiles/saves only assigned `.uasset`/`.umap` packages **through Unreal Editor**, then reopens and checks them.
6. Hands the actual map to the human PIE tester with exact changed paths and observed non-PIE results.

Do not edit Unreal package bytes outside compatible Editor operations. A native C++ compile does not verify a saved Blueprint or playable loading screen.

## 10. Run the independent slot-evidence tool

The repository includes a **real read-only PowerShell 7 script**:

[`examples/savegame-ab/scripts/Inspect-SaveSlots.ps1`](../examples/savegame-ab/scripts/Inspect-SaveSlots.ps1).

From the correct project/repo root, specify the **actual SaveGames directory** and, optionally, a separate JSON evidence file:

```powershell
pwsh -File ./examples/savegame-ab/scripts/Inspect-SaveSlots.ps1 \
    -SaveDirectory './Saved/SaveGames' \
    -SlotNames 'GPP_Run_A','GPP_Run_B'
```

For a local report in an existing writable artifacts directory:

```powershell
pwsh -File ./examples/savegame-ab/scripts/Inspect-SaveSlots.ps1 \
    -SaveDirectory './Saved/SaveGames' \
    -OutputJson './artifacts/save-slot-evidence.json'
```

These commands are examples using **relative paths**; an installed packaged game may store saves elsewhere according to platform/application configuration. Confirm the path used by the **process you actually launched**. The script reports **Found/Missing, bytes, UTC modification time and SHA-256**. It does not deserialize Unreal, determine which gameplay stage is saved, prove slot compatibility or launch a game. It must not be used to label a corrupted/valid save as PASS solely from the hash.

When a test expects a file to change, compare before/after metadata and hashes, then perform a **real fresh-process Continue**. Do not upload player-identifying file paths, raw SaveGames or private logs to a public playbook.

## 11. Exact human/automated acceptance scenarios

All statuses below are **NOT RUN** in this public repository. Run each appropriate case on the real integrated game, record engine revision, command/procedure, expected/observed state, FAIL/BLOCKED reason and evidence.

| ID | Test procedure | Required observation |
| --- | --- | --- |
| P01 | Compile the actual native Editor target | UHT/UBT succeeds; no missing generated classes or version errors |
| P02 | Compile/save all changed Blueprints and map | Correct parent classes, IDs, references and collision |
| P03 | Launch New Game | Fresh in-memory run, correct checkpoint and no accidental deletion of old saves |
| P04 | Pick up Offering once | ID collected only once; pickup disabled |
| P05 | Donate Light with valid held interaction | Light Donated, personal Q blocked, beacon permanently on |
| P06 | Request async save | UI displays saving; one in-flight save, callback and readback validate |
| P07 | Inspect SaveGames directory with PowerShell script | Actual A or B file exists, has nonzero bytes and recorded hash |
| P08 | Save again after a progress change | Other slot becomes newer generation; older valid slot preserved |
| P09 | Request another save during an active save | No overlapping write; later changes remain pending until a later save |
| P10 | Terminate game and launch a **fresh process** | Continue finds latest compatible snapshot |
| P11 | Continue after Light donation but before Recovery | Offering stays consumed, beacon powered, Q still blocked |
| P12 | Collect distinct Recovery and save | Recovered stage persisted, personal Q available, beacon still on |
| P13 | Close/relaunch and Continue after Recovery | No double pickup; permanent restoration remains |
| P14 | Leave streamable fragment area; return after unload/load | Consumed fragment does not respawn or regrant reward |
| P15 | Leave and return to restored landmark area | Functional gate/collision/visual state remains restored |
| P16 | Re-enter region repeatedly | No duplicated transitions, cinematics, sounds or grants |
| P17 | Recreate/replace possessed pawn after load | GAS spec/tag availability reconciles once; no duplicate ability |
| P18 | Corrupt **only one backup** in an isolated test copy | Newest valid fallback selected; no crash or default reset |
| P19 | Make both copies invalid in an isolated test copy | Continue refuses load safely; data not silently overwritten |
| P20 | Load a snapshot with unknown FormatVersion | Explicitly rejected; game stays responsive |
| P21 | Load inconsistent stage/IDs in an isolated test copy | Rejected by validation; no partial progress mutation |
| P22 | Have duplicate authored IDs in a test copy of the map | Duplicate detected and ambiguous interactions blocked |
| P23 | Verify missing checkpoint Actor or blocked floor | Safe fallback or explicit Continue failure; no fall/softlock |
| P24 | Check loading UI and player input while world streams | Controls remain disabled until game-world reconciliation completes |
| P25 | Start a new run with an existing prior run | Explicit user intent, new RunId, settings unaffected |
| P26 | Trigger an async save failure or denied storage in a safe test environment | Failure surfaced; previous valid snapshot retained |
| P27 | Save before a non-repeatable cinematic; attempt save during it | Last valid checkpoint retained; cinematic does not create a softlock |
| P28 | Package Windows and launch outside Editor | Real fresh-process Save/Continue works in the packaged build |
| P29 | Move/restore the **same** packaged build onto a separate test profile if required | Save location and platform-specific behavior documented |
| P30 | Change input, display or audio options, then New Game | User settings persist independently of run progress |

**Test safety:** do not corrupt or delete the only real player save. Run fault injection against **disposable copies and an isolated slot directory/user profile**. Preserve original backups. File hashes only provide evidence that bytes exist or changed; they are not proof of a valid SaveGame or gameplay invariants.

**Pass threshold:** a successful write is not enough. A complete feature must pass actual world-state reconstruction, relevant streaming cycles, fresh-process Continue and human gameplay acceptance. Tests not performed remain NOT RUN and required unresolved failures block the relevant validation gate.

## 12. Troubleshooting and recovery

| Failure | Likely cause | Corrective approach |
| --- | --- | --- |
| Save callback succeeds, Continue fails | Serialized fields invalid/incompatible or wrong game map | Validate readback/schema, map ID and checkpoint; preserve previous valid slot |
| Both slots appear usable but generation ties | Writer concurrency or duplicated snapshots | Refuse ambiguous selection and audit slot sequencing |
| Player regains Q before Recovery | Character-owned stale stage, default GAS grant or missing tag reconciliation | Make subsystem authoritative; reconcile ASC on every pawn initialization |
| Fragment returns on stream-in | Only actor-local `Destroy` or visibility state was used | Reapply collected-ID projection on every actor registration |
| Restored bridge blocks player | Material/animation changed but collision wasn't reapplied | Make functional collision follow restored logical state |
| Save during a visual effect records partial progress | Transaction and presentation are interleaved | Commit logical change once before presentation; snapshot only committed state |
| Load succeeds while map Actors are absent | World Partition source not ready / runtime streaming not enabled as assumed | Wait for actual region and actor registration; use safe fallback |
| Duplicate one-time audio/cinematic | `PlayTransition` called from `BeginPlay`/`ApplyState` | Split state projection from new-action presentation |
| Save for a new run unexpectedly points to an old checkpoint | Old slot was loaded instead of starting a new RunId | Explicit New Game authority and distinct Continue path |
| Saved file exists but state reset | Wrong SaveGames directory, user index, slot name or schema | Inspect actual process path and serialized candidate; verify fresh-process behavior |
| Save UI always says Saved | UI bound to callback rather than saved/current revision | Track later live changes separately and clear saved indicator when dirty |
| Corrupt save silently replaced | Default new-game branch ran after failed validation | Keep error state; require explicit New Game/recovery choice |

## 13. Required handoff and next tutorial

A real T07 handoff names the existing authority reused or migrated, exact C++/Blueprint/map paths, slot names and schema version, executed UBT/Editor/PIE/package checks, sanitized readback logs, actor-ID collision audits, checkpoint/streaming verification, and real human acceptance result. Do not claim published sample code was compiled by the playbook maintainers.

The following reference materials are directly connected:

- [Original A/B subsystem and file inspection example](../examples/savegame-ab/README.md)
- [T05 — Gameplay C++/GAS](gameplay-gas-tags-light.md)
- [T06 — Contextual Interaction](contextual-interactions-pickups-sacrifices.md)
- [Feature execution evidence model](../docs/06-production-planning/feature-execution.md)
- [Test plan](../templates/test-plan.md)
- [Agent handoff](../templates/agent-handoff.md)

**Next editorial topic:** T08 — Camera Rigs, Occlusion and Enhanced Input. T07's SaveGame work is not complete for a specific game until its integrated, playable cross-process and World Partition checks actually pass.
