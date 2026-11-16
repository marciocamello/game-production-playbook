# T06 — Contextual Interaction, Fragments and Voluntary Sacrifice

**Documentation status:** Published. This chapter is an **execution tutorial**, not proof that the described T06 feature has already been integrated into a reader's game. No Unreal Editor, local compile, PIE or standalone validation was performed for this public publication.

**Engine:** Unreal Engine 5.9 (C++ and Blueprints), single-player. Check the actual installed engine build when an API or plugin differs.

**Goal:** implement one coherent **focus → prompt → collect a uniquely identified fragment → hold to confirm an ability sacrifice → restore a functional world landmark → recover through a separate, accessible fragment** loop. Reject invalid input, repeated requests, obstructed targets and cancellation without a partial state change.

**Start from an existing project:** this tutorial extends the [T05 GAS Light demo](gameplay-gas-tags-light.md). Its existing `AGPPLightCharacter` owns `EGPPLightStage` and the Ability System Component, and `AGPPLightBeacon` already projects committed restoration. **Do not create a second Character, GAS authority or progress manager.** If your game has its own interaction component and progression subsystem, extend those instead. The name "fragment" and the Light mechanic are educational examples, not a requirement to copy any specific game's fiction.

## 1. Verified engine references and actual scope

Official documentation:

- [Epic — Enhanced Input](https://dev.epicgames.com/documentation/unreal-engine/enhanced-input-in-unreal-engine?application_version=5.9) — `Started`, `Triggered`, `Completed`, `Canceled`, and trigger configuration.
- [Epic — Single Line Trace by Channel](https://dev.epicgames.com/documentation/en-us/unreal-engine/using-a-single-line-trace-raycast-by-channel-in-unreal-engine) — Visibility traces and blockers.
- [Epic — Traces Overview](https://dev.epicgames.com/documentation/unreal-engine/traces-in-unreal-engine---overview?application_version=5.9) — channel/object-type filtering and hit semantics.
- [Epic — Collision Overview](https://dev.epicgames.com/documentation/unreal-engine/collision-in-unreal-engine---overview?application_version=5.9) — overlap/block/ignore behaviors.
- [Epic — Timer Manager](https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Runtime/Engine/FTimerManager/SetTimer) — time-based confirmation and cancellation.
- [Epic — Gameplay Tags](https://dev.epicgames.com/documentation/unreal-engine/using-gameplay-tags-in-unreal-engine?application_version=5.9) — use GAS tags for eligibility, not a parallel save authority.
- [T05 example code](../examples/gas-light-demo/README.md) — the **actual** baseline to inspect before modifying its bindings or transitions.

This tutorial documents the *required real implementation and test procedure*. Its component specifications are **not substitutes for source code or Blueprint graphs in the reader's target project**. Do not mark the feature Implemented until the required actual functions, actors and input assets exist and are integrated.

## 2. Define one authoritative state machine

A working interaction must have a single place where the decision is committed. For the T05 baseline, keep the Character's `EGPPLightStage` as the **only in-session stage authority**; its ASC donation tag is a derived reflection, and the beacon's light is presentation.

Add a unique-ID collection ledger to the same existing progression owner (or reuse the game's persistent progression subsystem if already implemented). Do **not** store a competing "light donated" boolean in the widget or in each pickup.

| Operation | Conditions | Committed state | Player-visible consequence |
| --- | --- | --- | --- |
| Pick up Offering fragment | Correct target, valid ID, range, clear line of sight, not previously collected | Add Offering ID **once** to collected-ID set | Fragment disappears; collection cue and feedback |
| Donate Light | Offering ID recorded, Light = Owned, beacon is focused/reachable, confirmation held for full duration | Light = Donated; record powered beacon ID | Personal Light off; beacon warm light on; GAS tag blocks Q |
| Pick up Recovery fragment | Light = Donated, correct Recovery target, valid unique ID and unobstructed reach | Add Recovery ID **once** and change Light = Recovered | Personal Light available again; restored beacon stays lit |
| Duplicate collection | ID already collected or actor consumed | No mutation | No reward, no second event |
| Duplicate donation | Light is Donated or Recovered | No mutation | Beacon and ability state unchanged |
| Invalid recovery | Light not Donated, target unavailable, out of range or blocked | No mutation | No recovery |
| Canceled confirmation | Key released, target changed, LOS blocked, player moved away or game input interrupted before deadline | No mutation | Confirmation indicator resets |

**Atomicity is a design requirement:** validate the entire transaction again **at commit time**, not just when the prompt appears. A timer finishing is not permission to bypass current target eligibility.

### Transaction order and rollback

A recommended synchronous single-player commit sequence is:

1. Validate actor identity, target availability, player state and collected-ID uniqueness.
2. Reject any missing prerequisite **before any mutation**.
3. Record the authoritative ID/stage transition once.
4. Project visuals, GAS tags, sound and UI from the resulting committed state.
5. If a required gameplay dependency cannot be resolved, reject the operation **before step 3**, rather than collecting an item and later declaring the landmark restore failed.
6. Keep visual failures recoverable: a missing Niagara system must not roll back an already committed donation.

For this single-player exercise, perform mutations on the gameplay thread in the current world. Multiplayer RPCs, prediction and authoritative replication are outside the example; do not export this sequence as multiplayer-safe.

## 3. Assign ownership and inspect what T05 already does

Use the [ownership handoff](multi-agent-production-handoffs.md) and preserve the current Editor's unsaved packages.

| Work item | Target area | Owner |
| --- | --- | --- |
| Native input refactor | Existing `AGPPLightCharacter::SetupPlayerInputComponent`, `DonateLight`, `RecoverLight` | One C++ owner |
| Unique fragment identity/collection transaction | Existing progress owner + one real pickup Actor/Blueprint type | One C++/Blueprint owner |
| Interaction focus/LOS and hold timer | Existing player interaction service, or the existing Character if no service exists | One gameplay owner |
| Real fragment meshes, collision, prompt and sounds | Editor-authored content in the designated game Content namespace | One Editor/MCP operator |
| Actual level placement and accessibility | Assigned playable map | Same Editor owner |
| PIE acceptance and softlock checks | Integrated playable build | Human reviewer |

The T05 sample's current `E` binding directly calls `DonateLight` and `R` directly calls `RecoverLight`; `FindNearbyBeacon` only chooses by distance. **These are deliberately not sufficient for T06.** Remove/replace their bindings in the target project's copy; do not layer a new Blueprint `E` action **on top of** those old handlers, or one key press can commit two separate actions.

Preserve existing `ActivateLight`, `TogglePersonalLight`, `ApplyLightStage`, ASC initialization, Gameplay Tag derivation, point lights and movement. Reuse `AGPPLightBeacon::ApplyPoweredState` instead of writing an extra restoration boolean system.

## 4. Build a real, target-aware interaction interface

An interactable must supply **actual target-independent rules** and a deliberate action. In an existing Blueprint-focused game, create one `BPI_ContextInteractable` interface with the following **implemented functions**, or use the game's existing interface:

| Interface function | Return | Actual responsibility |
| --- | --- | --- |
| `CanInteract(Player)` | Boolean | Checks target's current, authoritative state and whether this player may use it |
| `GetInteractionVerb(Player)` | Text | Returns a concrete prompt such as "Collect", "Recover", or "Hold to restore" |
| `TryInteract(Player)` | Boolean | Requests the authority-owned transaction and reports whether it **committed** |

**Eligibility is not authority.** The player controller/interaction service still applies range, line of sight and focused-target checks. The target does not get to grant itself arbitrary progression rewards. A Blueprint interface message must call a **real authoritative function**; wiring it to `Print String` or a placeholder return value is not an implementation.

For a C++-heavy project, a native Unreal interface is also valid, but do not author both Blueprint and native interfaces for the same contract without a clear need. Use native `UINTERFACE`/`IInterface` conventions and the installed engine's generated-header rules if choosing C++.

### Stable identities

Create exactly two *real placed* collectible instances for the exercise:

| Placed instance | Author-assigned ID | Purpose |
| --- | --- | --- |
| First visible fragment | `Fragment.Offering.001` | Gates the sacrifice |
| Fragment made reachable by restoration | `Fragment.Recovery.001` | Enables post-sacrifice recovery |

IDs are **examples**, not runtime-derived Actor labels. Store them in an instance-editable `Name` property `FragmentId`. Require each instance to have a nonempty **unique** ID; on detecting duplicates, report the error and block those pickups. **Do not use actor display name, randomly generated IDs at BeginPlay or memory addresses** as durable state identities.

For multi-level streaming projects, use a namespaced stable ID scheme (level region + author-set ID) and validate duplicate registration as cells load. T07 covers reapplication after SaveGame and World Partition; T06's set is deliberately in-session only.

## 5. Author two actual pickup Actors

Create a concrete pickup actor, for example `BP_GPP_FragmentPickup`, or reuse an existing pickup class. It must have:

- A real Static Mesh component (an existing project mesh or the Engine Sphere), with visible contrast against the background. No missing asset reference.
- A small query-only pickup sphere or volume for **candidate discovery**; response to Pawn = Overlap.
- A geometry component or separate deliberate target component that can be used for the **Visibility** line-of-sight check.
- An instance-editable `FragmentId: Name` and `Kind: Offering | Recovery`.
- A state projection method such as `ApplyCollectedState(bool)` that shows/hides the mesh and enables/disables candidate collision without granting items.
- Concrete `CanInteract` and `TryInteract` implementations that delegate to the player's existing authoritative ledger.

**Do not call `Destroy Actor` as the only record of collection** when the actor must support later level reload/stream-in. The current game state must be capable of projecting the hidden/consumed state onto an actor when it reappears. A T06-only in-session demonstration may keep the actor hidden and disabled, but T07 must own actual serialization.

### Pickup Blueprint transaction

For the Offering instance, connect the implemented `TryInteract` flow in this order:

1. Read `FragmentId`, `Kind` and `Player`; reject invalid parameters.
2. Ask the **existing progression owner** to atomically record this ID if it has not been collected and if the Light is in the correct stage.
3. If the operation returns success, apply the consumed visual/collision state, send the real collection feedback, and return **true**.
4. On any rejection, return **false**, leaving the actor and collected count unchanged.

The Recovery instance calls the authority-owned recovery transaction instead. It may only commit when stage = Donated; on success it updates the stage to Recovered, removes the **derived** donation Gameplay Tag through the existing `ApplyLightStage` logic, and leaves the previously restored beacon on.

In a C++ version, use `TSet<FName>` or the game's existing typed ID registry for idempotent collection checks. **Do not implement a new SaveGame subsystem within T06**; persistent serialization is the next tutorial.

## 6. Discover target candidates, focus and line of sight

For a controller-independent elevated camera, do not assume a screen-center ray can hit ground-level collectibles. Use **player-centered proximity candidate discovery**, then select the best valid candidate with a consistent rule.

A concrete implementation route:

1. Add a **real Sphere Collision** component to the player, radius **250 cm** for this exercise, set to Query Only, and respond to dedicated interactable object type(s) by Overlap. Alternatively, use a bounded sphere query in the existing interaction service.
2. On overlap begin/end, maintain a candidate set of actual interactable Actors. Use weak references or remove invalid actors when streaming/destruction occurs. Avoid unbounded `Get All Actors Of Class` every Tick.
3. On focus update (e.g. **10 Hz**, or on candidate/movement change), reject actors that are destroyed, consumed, ineligible, too high/low to reach or farther than 250 cm from the player.
4. Cast a single **Visibility** trace from near the character's eye/chest to a clearly defined target anchor. **Ignore the player**, not walls; either treat a hit on the chosen target as success or ignore the target and treat any intervening blocking hit as occlusion. Pick one consistent policy and verify actual component collision.
5. Resolve ties by closest eligible target and a predictable angle/priority rule. The same target-selection algorithm must drive **both the prompt and the committed request**.
6. Publish a focused-target change to the HUD/Text Render only when it changes. When none is valid, clear the prompt and deny input.

**Collision setup matters:** an overlap sphere can discover a pickup even when a wall blocks physical access. That sphere **must not** be sufficient to approve the interaction. Verify that level walls respond `Block` to Visibility. Transparent VFX and decorative foliage should not accidentally obstruct interaction unless game design demands it.

**Focus failure cases:** wall between player and fragment; another interactable closer; moved-away player; rotated camera; missing collision; level stream-out; blocked target; unpossessed pawn. No eligible target means **no transaction**.

### Real on-screen feedback

For this small exercise, an existing UMG interaction widget or a **Text Render Component attached to the Character** can show a functional prompt from `GetInteractionVerb`. Display a distinct action for each state:

- `E — Collect fragment` when the Offering is valid.
- `Hold E — Donate Light` only when the Offering is collected and the beacon is eligible.
- `E — Recover Light` on the distinct Recovery fragment after donation.
- No prompt for invalid/consumed targets.

This is actual user-facing feedback, not a logging-only implementation. Do not claim MVVM/UMG integration unless the widget is created, bound, compiled and visible; the full UI architecture is T09.

## 7. Make sacrifice voluntary: hold, cancel, revalidate and commit

The baseline T05 `E` and `R` direct action bindings must be **replaced** in the target implementation by one context action (reuse `IA_GPP_Donate` if it already exists, rename intentionally if required). Keep `Q` bound to GAS Light activation.

Use a boolean Input Action bound to a **Started** handler and **Completed/Canceled** handlers, or a correctly configured Hold trigger plus explicit cancellation handling. For a native timed hold, the concrete contract is:

1. On **E Started**, resolve the actual focused target. A fragment commits once on press after eligibility recheck; a donation target records the candidate and starts a **0.85-second, one-shot `FTimerManager` timer**. The duration is an editable design choice, not an engine requirement.
2. During the hold, update real progress feedback from the timer's elapsed/remaining time.
3. On **E Completed** or **Canceled**, clear the timer, reset progress UI and weak target reference if a donation has not committed.
4. On the timer callback, ensure the key is still held, the same original target is still focused, the player has not moved beyond 250 cm, LOS remains clear, the Offering ID is committed, `EGPPLightStage == Owned`, and the beacon has not already been powered by this transaction.
5. **Only after all conditions pass**, call the existing authoritative donation transition once, record the powered beacon identity and invoke `ApplyPoweredState(true)`.
6. Clear the hold state and announce the committed restoration via UI/audio.

In this exercise, do **not** play a cinematic or spawn a cosmetic effect as a prerequisite to recording the game state. Effects may fail to render; the authoritative donation is still deterministic.

### Timing and input subtleties

- Enhanced Input `Completed` and `Canceled` differ according to trigger configuration. Bind and verify both when relying on release/cancellation; do not assume `Completed` always fires on early release.
- A **one-shot timer** is not itself proof the button remains pressed. Maintain and reset an explicit input-held flag or use a verified Held trigger and its lifecycle.
- Menu opening, loss of possession or input-context removal must cancel a pending hold; otherwise a pending timer could commit after the player loses control.
- If the input is an ordinary Boolean `Pressed` action, verify that its trigger state lifecycle matches the bind events you use.
- If two actors overlap, locking the held target is necessary; do not silently donate to a second beacon because it moved nearer before timeout.
- An instant Offering pickup and a held sacrifice must remain **distinct user intentions**; do not let one press both collect the fragment and immediately begin the sacrifice.
- Prevent reentrant callbacks and double commits by verifying the stage/ID at each request.

## 8. Wire the existing GAS ability and beacon state

This step is about extending a working implementation, **not introducing another ability architecture**.

In the T05 baseline, the stage `EGPPLightStage` has Owned, Donated and Recovered values. Its `ApplyLightStage` function projects the `Demo.State.Light.Donated` Gameplay Tag into the ASC, while `UGPPLightAbility` blocks light activation when that tag exists.

Your completed T06 integration must:

1. Keep `EGPPLightStage` as the only stage truth in this demo.
2. Add the Offering and Recovery IDs to the same progression owner, with uniqueness checks.
3. Refactor the existing `DonateLight` request so it requires a **resolved valid beacon, previously committed Offering and successful hold**; the previous radius-only `FindNearbyBeacon` must no longer bypass focus or LOS.
4. Refactor `RecoverLight` so it cannot be invoked directly with `R` at the beacon; it runs only through a **real distinct Recovery fragment transaction**. A direct legacy `R` binding is a bypass and must be removed.
5. Continue to use existing `ApplyLightStage`, `TogglePersonalLight` and `AGPPLightBeacon::ApplyPoweredState` methods as designed.
6. Verify one powered beacon stays powered when the personal Light is recovered.
7. Reject further donation/recovery requests when they would cause an impossible state transition.

**Important integration warning:** the source files published for T05 remain an **unchanged baseline**. Do not read this tutorial as a claim that T06 is already compiled into those eight files. A finished T06 integration necessarily modifies the *reader's real project*; replacing the existing input and transition code is mandatory. Adding only the two pickup actors leaves a broken bypass through the old T05 bindings.

## 9. Anti-softlock level placement

An ability-loss puzzle is only meaningful when the player can recover or adapt. Before authoring the real level, document a reachable route between sacrifice and recovery.

Example playable route:

1. The Offering fragment is visible and reachable from the spawn area without needing Light.
2. The beacon is reachable with collision and camera framing that permit the held interaction.
3. Committing donation opens a path/activates a mechanism or illuminates a traversal cue.
4. The Recovery fragment becomes accessible **without requiring the donated Light ability**. It can be reached using base movement.
5. Collection restores Light while the beacon remains powered.
6. A failed/canceled interaction leaves the player in a recoverable initial state.

Avoid confusing a **visual transition** with a traversable opening: the actual blocking collision/nav route must reflect the committed gameplay state. Do not wait for a Niagara/Chaos animation to finish before a critical route can open if the player can skip or interrupt presentation.

For production projects, persistence and stream-in reapplication must preserve this route after a load; test those separately when T07 is integrated.

## 10. Integration plan for the real Editor operator

The C++ owner completes the native input/transaction changes and compiles the **actual installed Engine target** first. The sole Editor/MCP operator then:

1. Inspects the assigned map and existing character assets; preserves unrelated dirty packages.
2. Authors the real `BPI_ContextInteractable` and the pickup implementation **if they do not already exist**.
3. Creates a real player proximity component and target prompt only if the existing interaction service lacks those capabilities.
4. Assigns actual input mappings, including the E Started/Completed/Canceled lifecycle, and **removes the legacy donation/recovery bypass bindings**.
5. Places the two uniquely identified fragments and the existing beacon at verified reachable locations.
6. Sets collision channels/responses and target anchors, including LOS-blocking walls.
7. Compiles and saves the created/modified Blueprints and the owned level through Unreal Editor, not by editing asset bytes.
8. Reopens the level and verifies references, unique IDs, input mappings and actor state projection.
9. Hands the actual integrated map to the human for manual PIE. Records Editor checks **separately** from PIE checks.

The result is **not complete** at step 7 if no playable map is saved, target collision is wrong, the character still has old input handlers, or the runtime path has not been tested.

## 11. Human PIE acceptance — scenarios to execute

Every row below starts as **NOT RUN** in this documentation repo. Do not mark it PASS until executed and evidenced on the actual integrated project.

| ID | Exact test | Required observation |
| --- | --- | --- |
| I01 | Load the assigned map in PIE | Possessed player, movement, camera and real prompt are operational |
| I02 | Observe Offering from outside 250 cm | No valid action until in reach |
| I03 | Approach Offering with clear LOS | Correct `E — Collect fragment` prompt |
| I04 | Put an actual wall between player and Offering | No pickup, no ID recorded |
| I05 | Press E on Offering once | ID recorded exactly once; mesh/candidate collision disabled; visible feedback |
| I06 | Press E repeatedly on the consumed Offering | No second ID/reward/event |
| I07 | Hold E at beacon **without** a collected Offering (fresh run) | Rejected; stage remains Owned |
| I08 | Begin hold with valid Offering, release E early | Timer canceled; Light remains Owned; beacon dark |
| I09 | Begin hold, then move out of range | No donation |
| I10 | Begin hold, turn behind obstruction or lose LOS | No donation |
| I11 | Begin hold, switch focused target | No donation to either unintended target |
| I12 | Hold E for 0.85 s on authorized beacon | Exactly one stage change to Donated; beacon on; personal light off |
| I13 | Press Q after donation | GAS blocks personal Light |
| I14 | Repeat E/hold on powered beacon | No duplicate sacrifice/restore reward |
| I15 | Find Recovery fragment across the actual playable route | Reachable **without** donated Light |
| I16 | Attempt Recovery pickup before donation (fresh run) | Rejected; no collected ID or stage change |
| I17 | Press E on reachable Recovery after donation | Recovery ID added once; stage Recovered; personal Q allowed |
| I18 | Press Q after recovery | Personal Light works; beacon remains on |
| I19 | Repeat interaction with consumed Recovery | No second collection or state change |
| I20 | Open pause/options or change input context during hold | Hold cancels; no delayed donation |
| I21 | Duplicate a fragment ID in a test copy of the level | Duplicate is detected and both ambiguous pickups are blocked/reviewed |
| I22 | Test target actor unload/destroy during focus | No crash, stale prompt or invalid commit |
| I23 | Inspect output and scene after all interactions | Correct stage, collected-ID count, ASC tag, actor visibility and collision |
| I24 | Launch a newly packaged build outside Editor if the milestone requires it | Same interaction behavior in standalone, independently evidenced |

**Important:** The current T05 sample intentionally resets its state after world reload. T06 does **not** invent persistent storage. Save/load and World Partition streaming tests are **BLOCKED / NOT RUN until T07 is actually implemented**; do not report T06 as providing them.

Record the failed ID, expected vs observed behavior, reproduction, screenshots/short recording, command/log, game revision and responsible owner. A C++ compilation PASS cannot replace human I01–I24 gameplay evidence.

## 12. Common production failures

| Symptom | Probable cause | Specific corrective action |
| --- | --- | --- |
| E both picks up and donates | Legacy native input and new Blueprint binding both active | Remove old T05 direct `E` binding; one context action owns input |
| R recovers without pickup | Legacy `RecoverLight` key path still active | Remove the bypass; recovery must be authorized by the distinct fragment transaction |
| Pickup through wall | Overlap/range used as permission instead of LOS | Add Visibility trace and verify wall response is Block |
| Prompt says Collect but E fails | Focus and commit choose different targets, or state changed | Resolve and revalidate the **same** target |
| Release still triggers donation | Hold timer or input-held flag not canceled | Handle both Completed/Canceled and interruption paths |
| Two fragments grant the same reward | Duplicate/empty IDs or actor-local state only | Enforce stable unique IDs in a central collection ledger |
| Fragment reappears after streaming | Only `Destroy Actor` used for progress | Reapply consumed state from persistent authority; implement T07 |
| Recovery inaccessible | World layout requires a donated ability | Rework the actual collision/traversal route; do not assume visual effects grant passage |
| Beacon turns off after recovery | Beacon light is driven from current personal ability state | Project landmark restoration from durable committed restoration state |
| "Success" log but no visual effect | Editor asset not saved or wrong world/material visibility | Reopen asset, inspect actual actor/component properties and run PIE |

## 13. Evidence and next dependency

A complete real T06 feature handoff must contain:

- The changed existing C++/Blueprint inputs, interaction functions and authoritative state owner.
- Actual unique pickup Actors and their saved package/level paths.
- Readable player-facing focus and hold feedback visible in gameplay.
- Evidence of each committed/rejected transition and collision/LOS behavior.
- Real compile and Editor checks, human PIE results and known standalone gaps.
- No secondary progression authority, no invented success and no unresolved softlock.

Use [the feature execution contract](../docs/06-production-planning/feature-execution.md), [test plan](../templates/test-plan.md) and [agent handoff](../templates/agent-handoff.md). The next tutorial, **T07 — SaveGame, reload and World Partition**, will take the stable fragment IDs, collected-ID ledger, Light stage and powered-beacon identity and **actually serialize/reapply them** instead of treating an in-session demonstration as durable progression.

**Editorial conclusion:** this document is complete as a reproducible production tutorial. Implementing T06 in any real game remains a separate, observable coding/Editor/testing task.
