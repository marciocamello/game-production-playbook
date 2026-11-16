# T15 — Niagara, Chaos and Deterministic Restoration: Visual Reconstruction Without Gameplay State Drift

**Publication state:** This is a reproducible Unreal Engine 5.9 procedure with [original C++ presentation source](../examples/restoration-vfx/README.md) and a small backward-compatible extension of the existing [T07 progression example](../examples/savegame-ab/README.md). **No local UE 5.9 build, Niagara asset authoring, Chaos Cache recording, PIE, stream-in test or packaged Windows launch was performed for this publication. All those gates remain NOT RUN.**

**Objective:** after the player really restores a landmark, three separate pieces visibly return to a stable assembled pose and an authored Niagara burst plays; a real already-existing progression gate stays authoritative and traversable; save/reload and World Partition reapply the final state **without replaying the effect**. Optionally add Chaos-assisted debris without making its simulation a second gameplay state machine.

**Prerequisites:** A real Unreal C++ project, the [T05 ability](gameplay-gas-tags-light.md), [T06 deliberate interaction](contextual-interactions-pickups-sacrifices.md), [T07 state authority](savegame-reload-world-partition.md), [T13 imported or Engine-shipped meshes](unreal-static-mesh-import-integration.md) and a [T14 collision-checked route](level-design-modeling-mode-geometry-script.md). The T07 code demonstrates one restored beacon; adapt it to an existing run authority if your game already has one.

## 1. Commit gameplay state before presentation

The restoration transaction belongs to the run/progression owner, **not** to Niagara, Chaos, a Level Sequence, a streamed mesh Actor or a timer.

```text
Real validated player donation (T06)
    -> UGPPDemoRunSubsystem::TryDonateLight(BeaconId) succeeds (T07)
    -> durable in-memory beacon/ability state commits exactly once
    -> OnProgressChanged: all projections immediately snap to authoritative state
    -> OnBeaconRestored(BeaconId): ONLY the successful fresh donation
    -> presenter plays piece motion and Niagara
    -> presentation finishes or is interrupted; gameplay stays restored

SaveGame load, New Game or World Partition stream-in
    -> OnProgressChanged / BeginPlay projection only
    -> presenter snaps to Broken or Restored, NO one-shot effect
```

The new event is deliberately separate from the pre-existing `OnProgressChanged`. We added `FGPPDemoBeaconRestored` to the **existing T07 subsystem**, and broadcast it only after `TryDonateLight` has committed. `ReadBestSnapshotIntoMemory`, checkpoints and recovery never broadcast it. This avoids replaying Niagara merely because data was loaded.

Do **not** install `UGPPDemoRunSubsystem` in parallel with an existing authoritative run manager. In that case, add an equivalent fresh-commit notification to the existing manager and adapt the presentation subscriber. Keep stable, Editor-assigned beacon IDs consistent between interaction, subsystem, presenter and real gate.

## 2. Real, reusable native presentation actor

Read the complete [T15 source and integration contract](../examples/restoration-vfx/README.md). `AGPPDemoRestorationPresenter` has three actual movable `UStaticMeshComponent` pieces (using the real Engine Cube for an immediately visible training result) plus one `UNiagaraComponent`.

1. Check for an existing restoration presenter and reuse it if present; do not create a competing Actor. Check the project module, enabled Niagara plugin and installed UE 5.9 API signatures.
2. If the project uses the T07 sample in its existing module, add the two T15 source files to the **same module**. Add `Niagara` to that module's Build.cs dependencies (not a blank new module); compile with T02's local build workflow.
3. Open the authorized Unreal Editor, under exclusive Editor ownership. Create a Blueprint based on the compiled `AGPPDemoRestorationPresenter` and place one instance beside the existing powered-beacon landmark. Do not replace the interaction or collision gate.
4. In Details, set the instance's `BeaconId` to **exactly the same FName** passed to `TryDonateLight`. Verify the three native mesh components and their final relative transforms. The native default is `DemoBeacon`; changing just the Blueprint but not the donation call will intentionally produce no reconstruction.
5. The native `StartingOffset` positions make the pieces visibly broken before restoration and animate them to their saved final relative transforms. For finished art, replace the cubes with imported approved modular meshes while keeping component identity and the real collision gate separate.
6. Set all presenter piece collisions to **NoCollision**; inspect each component in the Editor to confirm. The existing gate/bridge Actor independently uses `IsBeaconPowered(BeaconId)` to set traversal collision on BeginPlay and every progress change.
7. Implement/verify the real T06 offering pickup and hold-confirm donation calling the T07 transaction. A fake Editor toggle or `BeginPlay` auto-donation is not an acceptance test.
8. Save the Blueprint and level, close/reopen and inspect dependencies. Never write `.uasset` or `.umap` bytes directly.

**Built source behavior:** `OnProgressChanged` snaps pieces to the current correct state and deactivates effects. `OnBeaconRestored` starts a bounded, eased, two-second three-piece assembly; all pieces remain nonblocking; the actor ticks only during motion. `EndPlay` unsubscribes from both delegates. The Niagara component is activated **only if a real Niagara System asset has been assigned**. Source files do not create a fictitious Niagara package.

## 3. Author and integrate an actual Niagara System

Use the Niagara Editor, not text-file approximation:

1. Enable/verify the Niagara plugin in the installed project. Under the approved Content namespace, create a **Niagara System** (e.g. `NS_RestorationBurst`) from the installed `Simple Sprite Burst` template. If the template differs in your 5.9 build, choose its available equivalent and document that choice.
2. In the Niagara stack, select a CPU Sprite emitter for a small one-shot effect. Configure **one burst** of about 20–32 particles rather than indefinite spawning. Set a short lifetime (~0.5–1.5 s), small sprite sizes, and a modest upward/outward velocity. Use a valid emissive or unlit translucent Sprite material cleared for this project.
3. Set particle color/opacity over life so the burst visibly fades; check that the system does not restart in a loop and that its bounds enclose the full motion.
4. Compile/save the Niagara System. Preview it in the Niagara Editor and the actual level, from the gameplay camera and intended brightness; ensure the effect does not overwhelm the interaction prompt or cover the player.
5. In the `AGPPDemoRestorationPresenter` Blueprint, select the inherited `RestorationFX` component and assign this **saved Niagara System**. Keep `Auto Activate` off. The native code activates it from the committed event and deactivates it when presentation is complete or the Actor unloads.
6. In PIE, complete the real pickup/donation sequence. Record that the burst starts **once** for the newly committed beacon and that pressing the interaction again never causes duplicate bursts.
7. Test without the Niagara asset: the real native three-piece animation must still complete and game progress must remain correct. This is a resilience check, not permission to label the Niagara work integrated.

For a more sophisticated result, add material parameter changes to permanent authored architecture, but apply the final material state immediately from progress on load. Avoid keeping a transient dynamic material instance as the only record of completion.

## 4. Chaos-assisted reconstruction: optional visual layer

Chaos Geometry Collections represent destructible/fractured geometry, **not a SaveGame authority**. The T15 core actor intentionally remains fully playable without Chaos.

**Optional in-Editor exercise with real assets:**

1. Choose an existing **decorative, nonblocking** mesh such as a stone ornament. Confirm rights, closed/watertight geometry, no intersecting shells and good scale. Duplicate/prepare the source into an approved experiment namespace; preserve the actual gameplay gate.
2. In Unreal Editor **Fracture Mode**, create a new **Geometry Collection** from that source. Fracture with a small, intentionally limited number of pieces and inspect the collection hierarchy, physical material, mass and collision.
3. Enable the relevant installed **Chaos Caching** tools only after verifying the plugins and tool schemas. Simulate and record a short destruction motion with the actual cache tooling supported by this 5.9 installation. Save the Geometry Collection and recorded cache in owned Content paths.
4. Inspect cache playback from a **separate decorative Actor**, with no Pawn-blocking role. If the installed Cache/Sequencer authoring supports reverse time playback, test it as a rebuild presentation. If that exact operation is not supported or fails in packaged builds, use the T15 deterministic modular motion and restrict Chaos to a transient debris accent.
5. On fresh successful restoration only, initiate the decorative playback via the existing committed-event notification. Do **not** enable physical simulation for the rebuilt walkway, gate or support structure, and do not wait for a Chaos callback before applying final traversal collision.
6. On skip, interruption, stream-out or load, stop or discard the decorative cache playback and project the final authored mesh/collision state immediately. A cache's last frame is not a durable game-state source.
7. Check the Chaos cache's cooked references and playback in a **separate packaged Windows process**. An Editor preview is insufficient, particularly where plugins or caching facilities are experimental in the installed version.

**Decision rule:** if Chaos has no demonstrable visual advantage over deterministic piece animation, remove the Chaos runtime dependency from the shipped content. A heavy physics technique is not a requirement for every restoration.

## 5. Lifecycle, collision and ownership invariants

| Event | State authority | Visual result | Traversal result |
| --- | --- | --- | --- |
| Before first donation | `IsBeaconPowered=false` | Pieces scattered, burst inactive | Real gate closed |
| Correct fresh donation | `TryDonateLight=true` once | Moving pieces plus authored Niagara | Real gate opens from committed state |
| Canceled hold / invalid donation | No commit | No new sequence or burst | Unchanged |
| Repeated donation attempt | `TryDonateLight=false` | No replay | Remains open |
| Save during transition | Existing T07 save rules | Transition may continue locally | Restored |
| Load while actor exists | T07 logical state replaced | Immediate snap, no fresh burst | Reproject restored |
| Stream-out then stream-in | Authority retained in GameInstance | BeginPlay snaps to current state | Gate reprojects from authority |
| New Game after completed run | New unpowered run | Pieces return to broken pose | Gate closes |
| Skip/camera change/FX disabled | No state changes allowed | Snap or finish presentation | Remains correct |

**Important:** T07's snapshot load is logical-only. A real Continue flow must still load the correct map, validate checkpoint/capsule safety, reconcile the pawn/ASC and reapply gate collision. This tutorial does not claim the entire game loop is complete from the presenter alone.

**Ownership:** Codex/C++ owner changes the T07 delegate and T15 class; the sole Unreal Editor/Claude operator owns the Niagara System, optional Geometry Collection/Chaos Cache, Blueprint placement, materials and map. A different auditor may inspect source or evidence without opening a conflicting editor session. Human acceptance is required after actual PIE.

## 6. Verification matrix (record PASS / FAIL / BLOCKED / NOT RUN)

| ID | Required observed check |
| --- | --- |
| V01 | Existing project and module audited for equivalent presenter/event code |
| V02 | Correct installed UE 5.9 and Niagara module/plugin confirmed |
| V03 | UHT/UBT compiles T07 event and both T15 C++ files without errors |
| V04 | New `OnBeaconRestored` fires for exactly one successful donation |
| V05 | Rejected duplicate or invalid donations emit no fresh-commit event |
| V06 | A non-donation progress change emits no fresh-commit event |
| V07 | Real three meshes visible before and after in the camera |
| V08 | Native movement reaches final authored transforms without drift |
| V09 | Animation duration remains close to configured value across frame rates |
| V10 | No presenter piece blocks Pawn collision before, during or after |
| V11 | Already-existing gameplay gate blocks the player before restoration |
| V12 | Game authority opens that gate immediately on successful donation |
| V13 | Player crosses the gate before animation ends without an invisible wall |
| V14 | A real saved Niagara System is assigned to the inherited component |
| V15 | Niagara burst plays only after fresh successful donation |
| V16 | Niagara is absent or disabled: geometry and gameplay still complete |
| V17 | Existing SaveGame can save a restored state while VFX is in progress |
| V18 | Loaded game snaps to restored state and does not replay the burst |
| V19 | Loading an unpowered saved run restores the broken presentation |
| V20 | Streaming the region out/in does not restart VFX or gate collision |
| V21 | An ongoing animation interrupted by a progress snapshot snaps safely |
| V22 | New Game resets presenter state without leaking event bindings |
| V23 | Actor EndPlay and reload do not double-bind delegate listeners |
| V24 | Optional Chaos pieces are cosmetic, nonblocking and have no save authority |
| V25 | If Chaos used, cache record/playback and skip behavior verified in Editor |
| V26 | Niagara and optional Chaos assets survive cook with references resolved |
| V27 | Standalone packaged Windows build launches and completes the real route |
| V28 | Profiler measures effect cost at target resolution and quality |
| V29 | Game Director accepts timing, legibility, collision and audiovisual result |

Do not collapse these into a single "build passed" status. A C++ source commit is not evidence that the Niagara System exists, that a Chaos Cache was recorded, or that the packaged game works.

## 7. Failure recovery

- **Restored mesh but no burst:** inspect Niagara asset assigned to `RestorationFX`, `Auto Activate`, emitter one-shot/lifetime and whether the fresh event actually fired. Confirm `BeaconId` matches.
- **Burst plays after loading:** check for an accidental `OnBeaconRestored` broadcast during load or a Niagara component with `Auto Activate` enabled. A normal `OnProgressChanged` must only snap.
- **Visual closes but collision stays open (or vice versa):** audit the real gate's `IsBeaconPowered` projection; the presenter must not be used to set gate collision.
- **Duplicate burst:** test multiple Blueprint instances with the same beacon ID, duplicated binding or an invalid repeated transaction; do not "fix" by adding an arbitrary global boolean that breaks New Game.
- **Player stuck during rebuild:** disable collision on decorative pieces and separate those from the authoritative blocking gate. Do not disable the gate before an actual committed restoration.
- **Niagara GPU cost or missing effect:** use Niagara profiling, bounded particles, short lifetimes and correct fixed bounds; do not assume millions of particles are necessary.
- **Chaos cache missing in package:** inspect cooked dependencies, plugin availability and packaged logs; revert to deterministic static-mesh assembly if caching cannot be demonstrated reliably.
- **A streamed actor misses the fresh event:** it must show the correct final state through BeginPlay projection. Never require that event for correctness.

## 8. Evidence, limits and references

Deliver the exact C++ commit, real Blueprint/Niagara/Geometry Collection/Chaos package paths, screenshots or recording from **actual gameplay**, event and collision observations, SaveGame readback, PIE cases, frame-time measurements and a separately launched Windows executable result. Report failures and unrun cases, not invented success.

**Official references:** [Epic Niagara overview](https://dev.epicgames.com/documentation/en-us/unreal-engine/creating-visual-effects-in-niagara-for-unreal-engine), [Niagara Quick Start](https://dev.epicgames.com/documentation/en-us/unreal-engine/quick-start-for-niagara-effects-in-unreal-engine), [UNiagaraComponent API](https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Plugins/Niagara/UNiagaraComponent), [Geometry Collections](https://dev.epicgames.com/documentation/en-us/unreal-engine/geometry-collections-user-guide), [Chaos destruction overview](https://dev.epicgames.com/documentation/en-us/unreal-engine/destruction-overview), [Niagara performance](https://dev.epicgames.com/documentation/en-us/unreal-engine/measuring-performance-in-niagara), [T07 SaveGame](savegame-reload-world-partition.md) and [T14 authored blockout](level-design-modeling-mode-geometry-script.md). Verify the installed UE 5.9 signatures, Editor menus and plugin versions before execution; the public docs may currently default to a different engine version.

**Next tutorial:** [T16 — Sequencer, gameplay transitions and cinematic return](README.md) will add explicit camera/input handoff and skip/restore behavior, without giving cinematic presentation authority over durable progress.
