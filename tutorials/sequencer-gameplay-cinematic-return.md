# T16 — Sequencer, Gameplay Transitions and Cinematic Return: An Interruptible Story Beat

**Publication state:** A complete Unreal Engine 5.9 Editor/Blueprint execution tutorial. No Level Sequence, Blueprint, actual camera track or packaged executable was authored or played in an installed Editor for this documentation publication. **Engine execution, PIE, human acceptance and Windows packaging are NOT RUN.**

**Player-visible objective:** After a genuinely committed restoration, a short cinematic frames the newly functioning landmark and returns the **same possessed player** to the correct gameplay camera, movement, HUD and input context, whether it finishes normally, is skipped, is interrupted or the streamed sequence actor disappears. A save/load never replays the moment merely because the landmark is restored.

**Dependencies:** [T06 real interaction](contextual-interactions-pickups-sacrifices.md), [T07 persistent state](savegame-reload-world-partition.md), [T08 camera/input](camera-rigs-occlusion-enhanced-input.md), [T09 UI](umg-mvvm-hud-menus-settings-loading.md), [T15 fresh-commit event](niagara-chaos-deterministic-restoration.md) and a real, saved [T14 level](level-design-modeling-mode-geometry-script.md).

## 1. Identify actual ownership and cinematic trigger

Inspect the existing PlayerController, GameMode, HUD, enhanced-input mapping contexts, CameraComponent/Gameplay Camera rig, gameplay lock APIs, UI focus and save/loading coordinator. Find the currently integrated way the real game changes camera and disables player actions. **Reuse those methods**. Do not create a second input lock service, camera authority or duplicated progression flag.

The T15/T07 source publishes two distinct events:

- `OnProgressChanged` for both fresh actions and snapshot loads: project permanent restored meshes/collision/ability availability without a cinematic.
- `OnBeaconRestored(BeaconId)` **only after** a successful, new `TryDonateLight`: eligible to start **one optional cinematic**. The event does **not** commit progress, award anything or open collision.

Use a stable Editor-authored BeaconId, exactly matching the existing restoration actor and sequence binding. A Level Sequence **must not** call `TryDonateLight`, change SaveGame or grant abilities from an Event Track. Its camera/audio/VFX tracks are presentation only.

### Explicit transition state machine

```text
Gameplay (no cinematic lock owned)
    -> fresh OnBeaconRestored + matching ID + valid sequence/player
    -> Capturing (remember possessed pawn, original view target and UI state)
    -> Playing (acquire one owned movement/look lock; optional HUD treatment)
    -> Finished OR Skip OR Stop OR Level Unload OR Playback Error
    -> Returning (stop sequence, release only this transition's lock,
                  restore camera/view target and applicable input/UI)
    -> Gameplay (idempotent, even if multiple completion signals arrive)
```

Do not use `OnProgressChanged` as the cinematic trigger. Do not depend on the sequence's last frame for stable bridge geometry, collisions or save state.

## 2. Author the real short Level Sequence in the Editor

The sole authorized Unreal Editor operator:

1. Inspect the **existing** approved map, dirty packages, Level Sequence assets and camera rigs. Do not overwrite a previous sequence or take over another agent's Editor session.
2. Create a new saved Level Sequence asset in the approved cinematic Content folder, for example `LS_DemoBeaconRestored`. In the target map, place or reuse an actual Level Sequence Actor referencing it; give the Actor a stable authored name.
3. Add a real **Cine Camera Actor** to the map and bind it into the sequence. Avoid spawning another pawn or overriding the existing PlayerController. Create a **Camera Cuts** track using that camera, and inspect the cuts' timing.
4. Use a 3–5 second sequence for the teaching exercise: 0–1 s reveal the restored structure; 1–3 s move to show newly traversable passage; final second settle to the intended return framing. Keyframe a real camera transform/focal length, not an invented asset or synthetic screenshot.
5. Configure sequence playback to **not loop**. Do not use `Pause at End` to keep a cinematic view indefinitely. Verify all bindings resolve after saving and reopening the map.
6. Optionally add a sound/particle track using **already imported, licensed assets**; keep T15's event-driven reconstruction and T17's sound ownership consistent to avoid double playback.
7. Review **When Finished** behavior for every noncamera property/track; restore pre-animated values for temporary presentation changes, or intentionally retain the final state only when the authoritative game state already mandates it.
8. Verify the actual Camera Cuts track in the Sequencer viewport, then save the Level Sequence and the approved map. Unreal assets must be authored by the Editor/compatible tools, never written as binary data by a text agent.

**Dependency warning:** a working Level Sequence Asset is not itself a working game transition. The player must reach it through the actual runtime event and regain control on all exits.

## 3. Wire the sequence from the existing gameplay/controller system

Prefer the existing PlayerController's cinematic service or its Blueprint subclass. This tutorial uses an **operational Blueprint event graph procedure**, not a fake empty C++ coordinator.

1. At the existing controller initialization, acquire the current `UGPPDemoRunSubsystem` and bind a single event listener to `OnBeaconRestored`. On shutdown or handoff, unbind that listener. Do **not** bind again every time the widget opens, a region streams or a checkpoint changes.
2. On `OnBeaconRestored`, compare the supplied FName to the authored BeaconId and reject unrelated IDs. If already playing, reject the duplicate launch. Validate the bound Level Sequence Actor and its real Sequence Player.
3. Save the **actually possessed pawn** and the PlayerController's current `ViewTarget`. Capture the necessary prior HUD/input context. Prefer the game's existing reference-counted or token-based input-lock service.
4. Bind **both** the actual Sequence Player's `OnFinished` and `OnStop` or equivalent supported completion signals to a *single idempotent* return routine. Inspect exact Blueprint node names in UE 5.9. Bind once per player/sequence lifecycle.
5. Acquire exactly one **owned movement/look lock**, and explicitly leave the Skip action enabled in a valid Enhanced Input mapping context. `DisableInput` on the whole PlayerController is unsuitable when that also prevents Skip. If using `SetIgnoreMoveInput(true)` and `SetIgnoreLookInput(true)`, release **only the corresponding ownership** with matching false calls once; do not use global `ResetIgnoreMoveInput`/reset-look to clear unrelated gameplay locks.
6. Start playback via the existing Level Sequence Player's `Play` node. The camera-cut track takes cinematic view ownership for its active range. Avoid calling a competing `SetViewTargetWithBlend` during playback.
7. Bind an Enhanced Input **Skip Cinematic** action to the existing input controller, available during playback. On a deliberate skip, invoke the common return routine directly; stop playback without waiting for `OnFinished`.
8. In the common return routine, guard with an active transition token/bool: the **first** caller deactivates the transition; later callbacks do nothing. Stop the sequence if still playing, restore the captured gameplay camera/view target, release only owned locks, and restore correct HUD/input contexts. If pawn possession changed, validate a suitable current pawn before restoring; do not blindly possess or teleport the old one.
9. Handle Actor EndPlay/unload and controlled map changes by routing through the same return routine from the persistent owning controller. The Level Sequence Actor itself must not be the only holder of the cleanup logic.
10. Prefer an explicit disabled/bypassed cinematic option: the restoration remains committed, with the actual gate traversable and no presentation lock acquired.

**Runtime precision:** If the project uses a gameplay camera stack rather than the pawn's `CameraComponent`, restore the **existing camera authority's saved mode/rig**, not merely a hard-coded pawn view target. If a pause menu becomes active during sequence playback, choose a declared policy: either pause cinematic and maintain skip, or terminate cinematic then open menu; do not leave two UI/input owners fighting.

## 4. Return paths are executable contracts, not optimistic delegates

| Exit path | Trigger | Required observed outcome |
| --- | --- | --- |
| Normal | Sequence playback reaches end | Saved camera, movement/look input, HUD returned |
| Skip | Skip action pressed during playback | Stop sequence, cleanup once, no replay |
| Repeat skip / OnStop after skip | Second callback | No double unlock or extra SetViewTarget |
| Load | Snapshot changes while sequence active | Abort presentation, project loaded world state, restore gameplay |
| Unload | Level Sequence Actor streams out | Persistent controller cleans up, no broken camera |
| Restart/New Game | Run identity changes | No old cinematic state, correct starting HUD |
| Missing Sequence Actor | Fresh restoration with missing optional asset | Report error and continue playable gameplay |
| Disabled cinematics | User setting disables sequences | Progress commits normally, no input lock |
| Death/possess change | Controller changes pawn | Cleanup does not resurrect the old pawn or force possession |
| Window focus loss | Alt-tab, pause or focus switch | No permanent input deadlock |

If the real sequence cannot be played (invalid binding, plugin unavailable or asset missing), **skip presentation and retain committed gameplay**. Do not represent that as successful cinematic acceptance.

## 5. SaveGame, streaming, accessibility and scope

- The new restoration event is transient, never serialized as a required playback marker. T07 already persists permanent Light and beacon state; the result is applied on load regardless of the cinematic.
- For a mandatory story scene at a later checkpoint, model its **story completion** as real authority separate from sequence playback, commit at a defined transaction boundary and test fresh-process resume. Do not infer completion from sequence elapsed seconds alone.
- Provide Skip affordance with the actual mapped key/controller input, not only an overlay. Test that player-initiated Skip works while movement input is ignored.
- Avoid forced camera shakes, flashes or rapid cuts without accessibility consideration; preserve readable prompts/captions when spoken audio is present.
- Protect the critical path: an optional cinematic may fail without blocking traversal, the next objective or Save/Load.

## 6. Verification matrix — 26 independent cases

| ID | Observation |
| --- | --- |
| C01 | Audited current progression, camera, controller, HUD and lock owners |
| C02 | Real Level Sequence asset saved and reopens |
| C03 | Real Cine Camera bound and Camera Cuts track active |
| C04 | Sequence playback does not loop |
| C05 | Native T07 fresh commit event correctly matches BeaconId |
| C06 | Failed/duplicate donation cannot trigger sequence |
| C07 | SaveGame logical load cannot trigger sequence |
| C08 | Playing captures correct possessed pawn and view target |
| C09 | Movement/look intentionally blocked only during playing |
| C10 | Skip action still works while movement is blocked |
| C11 | Normal end restores camera/rig |
| C12 | Normal end restores movement and look |
| C13 | Normal end restores HUD and focus |
| C14 | Skip invokes return without waiting for end |
| C15 | Duplicate OnStop/OnFinished callbacks do not double-release locks |
| C16 | Existing independent input locks remain intact |
| C17 | Camera returns correctly with different gameplay camera rigs |
| C18 | Existing gate remains open immediately after fresh commit |
| C19 | Sequence does not grant another ability or alter SaveGame |
| C20 | Actor streaming/unload during playback restores control |
| C21 | Loading a different run while playing aborts safely |
| C22 | New Game does not retain a stale sequence/camera |
| C23 | Missing sequence asset leaves gameplay playable and logs failure |
| C24 | Alt-tab/pause/focus change does not cause permanent lock |
| C25 | Packaged Windows build runs cinematic and skip with correct inputs |
| C26 | Human reviewer accepts legibility, length, transitions and return |

Log **PASS**, **FAIL**, **BLOCKED** or **NOT RUN** per case, including the specific map, run/commit identity, actual Editor node/asset paths, build revision, logs and human observation. Do not mark the complete tutorial tested just because a Timeline or camera preview works.

## 7. Failure recovery

- **Player cannot skip:** confirm the Skip Input Action is still in an active mapping context; do not blanket-disable controller input.
- **Camera remains cinematic:** ensure common return routine executes on normal completion, skip, stop and Level Sequence Actor destruction; check actual camera authority and view target.
- **Infinite ignore-input:** use balanced, transition-owned lock/unlock, not global resets that can interfere with other systems.
- **Cinematic replays after loading:** the listener must use `OnBeaconRestored`, not `OnProgressChanged`.
- **Duplicate camera cuts:** inspect Level Sequence Actor instances, camera binding overrides and duplicate event subscriptions.
- **Sequence ended but world is unpowered:** cinematic did not commit game state, and must not pretend to. Fix the actual T07 donation transaction and gate projection.
- **Cinematic works in PIE but not packaged:** check cooked references, Level Sequence Actor streaming, map inclusion, packaged logs and actual input mapping assets.

## 8. Official references and next tutorial

- [Epic: Sequencer overview](https://dev.epicgames.com/documentation/unreal-engine/unreal-engine-sequencer-movie-tool-overview)
- [Epic: Sequencer basics](https://dev.epicgames.com/documentation/unreal-engine/how-to-make-movies-in-unreal-engine)
- [Epic: Cinematic Camera Actor](https://dev.epicgames.com/documentation/unreal-engine/cinematic-cameras-in-unreal-engine)
- [Epic: Enhanced Input](https://dev.epicgames.com/documentation/unreal-engine/enhanced-input-in-unreal-engine)
- [Epic: World Partition](https://dev.epicgames.com/documentation/unreal-engine/world-partition-in-unreal-engine)
- [T17 — reactive music, spatial audio and rights](reactive-music-spatial-audio-rights.md) covers committed-state audio routing and licensed output.

The installation's UE 5.9 headers, Blueprint node signatures, Sequencer option names and compiled packaged behavior are the final API/runtime authority. This is an **Editor-run procedure**; no Level Sequence binary was generated by publishing this Markdown.
