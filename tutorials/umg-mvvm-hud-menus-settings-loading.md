# T09 — UMG/MVVM: Functional HUD, Menus, Settings and Loading Boundaries

**Status:** Published tutorial with four **original C++ implementation files** and specific Unreal Editor/UMG wiring instructions. **Source has not been compiled or imported into an installed Unreal Engine 5.9 project; no WBP was authored by this repository and no human PIE or packaged-game validation has run.** Required execution checks remain NOT RUN.

**Reference version:** Unreal Engine 5.9, Windows PC, C++ + Blueprints, UMG and the **UMG Viewmodel** plugin (documented by Epic as **Beta**). Existing T05/T08 GAS Character and T07 SaveGame/Run Subsystem are reused; these separately published examples have **not** been integrated into a single tested game.

**Real player goal:** show the actual Light/progression state in the HUD, start a run from a functional Main Menu, pause and resume with correct input focus, view the actual active controls, apply/persist graphic and audio settings, and show the result of a real async SaveGame operation without inventing successful loading or Continue.

The concrete reusable C++ code and Widget Blueprint inventory are under [examples/umg-mvvm-demo](../examples/umg-mvvm-demo/README.md).

## 1. Authoritative references

Official documentation reviewed November 16, 2026:

- [Epic — UMG Viewmodel](https://dev.epicgames.com/documentation/unreal-engine/umg-viewmodel-for-unreal-engine): MVVM plugin, C++ FieldNotify, Viewmodel creation modes, view bindings, notification macros.
- [Epic — UMG UI Designer](https://dev.epicgames.com/documentation/unreal-engine/umg-ui-designer-for-unreal-engine): Widget Blueprints, hierarchy, events and layout.
- [Epic — UGameUserSettings](https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Runtime/Engine/UGameUserSettings): applying, saving, quality and display properties.
- [Epic — Enhanced Input](https://dev.epicgames.com/documentation/unreal-engine/enhanced-input-in-unreal-engine): Mapping Contexts, Input Actions, user mappings and runtime input.
- [Epic — UEnhancedPlayerInput::GetEnhancedActionMappingsView](https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Plugins/EnhancedInput/UEnhancedPlayerInput/GetEnhancedActionMappingsView): read-only current mapped controls.
- [Epic — UGameplayStatics](https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Runtime/Engine/UGameplayStatics): pause, SoundMix override and subtitles.
- [T05 — GAS Light example](gameplay-gas-tags-light.md), [T07 — SaveGame/World Partition](savegame-reload-world-partition.md), [T08 — Camera & Enhanced Input](camera-rigs-occlusion-enhanced-input.md).

**Version caution:** the selected Epic API page may show a default engine version. Use the installed **UE 5.9** headers, plugin versions and live Editor schema as the final authority, especially for Beta MVVM and Windows packaging.

## 2. Start by auditing existing UI authority

Do not create another UI service before checking whether the game already implements it.

Review its GameMode, PlayerController, active pawn, existing UMG widgets, native ViewModels, GameInstance and SaveGame services, Enhanced Input Mapping Contexts, User Settings, SoundMix/SoundClass assets, pause/input-mode transitions, HUD state events and any pending dirty Editor packages.

Then record in the canonical executable Feature Spec:

| Concern | Existing or selected authority | Real source of truth |
| --- | --- | --- |
| Light stage/fragment state | Existing T07 Run Subsystem | `GetLightStage()`, once-only fragment/landmark state |
| GAS capability | Existing T05 Character/ASC, reconciled to T07 | GAS tag/activation behavior, **derived from** run state |
| HUD | A single UMG ViewModel | Presentation of run state only |
| New Game | Session/Run coordinator | `StartNewRun` on successful player choice |
| Continue | **Requires actual map and safe checkpoint resume coordinator** | Validated SaveGame + world + pawn, not readback alone |
| Save | T07 `RequestAsyncSave` / `OnSaveCompleted` | Real async result and readback |
| Graphics/display | `UGameUserSettings` | Engine-managed settings |
| Master audio | Real SoundClass/SoundMix plus persisted preference | Audible class routing and config |
| Controls | Active Enhanced Input mappings | Player's resolved input action/key mappings |
| Menu navigation | One PlayerController/UI flow | Real pause state, widget lifetime, input focus |
| Loading presentation | Actual async operation or map transition | Completion callbacks and observed playable readiness |

**Do not** let ViewModel setters commit donations, grant fragments, claim a SaveGame was written or decide a restored bridge is traversable. User widgets invoke game/service commands, and the ViewModel observes the resulting state.

## 3. Reuse the published C++ rather than reinventing it

The project already has:

- [T05/T08 `AGPPLightCharacter`](../examples/gas-light-demo/Source/GPPLightCharacter.h): movement, GAS Light and sprint/reset.
- [T07 `UGPPDemoRunSubsystem`](../examples/savegame-ab/Source/GPPDemoRunSubsystem.h): in-memory state, progression events and an async save callback.
- [T07 `UGPPDemoRunSaveGame`](../examples/savegame-ab/Source/GPPDemoRunSaveGame.h): logical snapshots, separate from transient UI.

T09 introduces **only** these capabilities:

| New source | Actual code behavior |
| --- | --- |
| [`GPPDemoHUDViewModel.h`](../examples/umg-mvvm-demo/Source/GPPDemoHUDViewModel.h) + [`.cpp`](../examples/umg-mvvm-demo/Source/GPPDemoHUDViewModel.cpp) | Read-only `UMVVMViewModelBase` with three FieldNotify `FText` properties; changes from existing T07 progress/save events |
| [`GPPDemoUIController.h`](../examples/umg-mvvm-demo/Source/GPPDemoUIController.h) + [`.cpp`](../examples/umg-mvvm-demo/Source/GPPDemoUIController.cpp) | Creates UMG widgets, handles New Game, pause, real Save callback, Options/Controls navigation, input focus, exit and real options APIs |

The UI Controller references the existing T08 `ResetSprint()` on pause/menu takeover. The player **does not keep sprinting** just because game input loses focus. The same Controller owns menu widget lifetimes rather than spreading page state across unrelated Actor Blueprints.

**Required integration choice:** If the target game already has a working PlayerController and UI subsystem, extend that owner with this logic; do not install a second competing `AGPPDemoUIController`. The sample is one bounded teaching integration, not a blanket instruction to replace a game's architecture.

### Build dependencies

Use an existing native game module; add **only missing** `UMG`, `ModelViewViewModel` and `EnhancedInput` dependencies in its `.Build.cs`, in addition to the already required game modules. Enable the **UMG Viewmodel** plugin in the installed Editor. Include `MVVMViewModelBase.h`; do not invent C++ Viewmodel types.

## 4. MVVM notification contract: actual changes, not tick polling

A ViewModel is a presentation contract, not another progression manager.

The [native code](../examples/umg-mvvm-demo/Source/GPPDemoHUDViewModel.cpp) uses the documented `UE_MVVM_SET_PROPERTY_VALUE` macro to update three FieldNotify fields:

- `LightState`: `Owned` → personal Light available, `Donated` → unavailable, `Recovered` → available again.
- `Objective`: readable state-appropriate next action; no imaginary fragment inventory counters.
- `SaveStatus`: actual progress/failure/completion of SaveGame I/O.

The [UI Controller](../examples/umg-mvvm-demo/Source/GPPDemoUIController.cpp) creates the ViewModel once and subscribes to **existing** T07 delegates:

```cpp
RunSubsystem->OnProgressChanged.AddDynamic(
    this, &AGPPDemoUIController::HandleProgressChanged);
RunSubsystem->OnSaveCompleted.AddDynamic(
    this, &AGPPDemoUIController::HandleSaveCompleted);
```

Those callbacks refresh the ViewModel when the real run changes; they do not poll every frame or write progress into SaveGame objects. After `EndPlay`, event subscriptions are removed.

### One authoritative state, two projections

T05 originally owned an intentionally short-lived Character enum. T07 supplies durable authoritative state. **When integrating T05 and T07 in one actual game**, migrate the Character to **derive** its visible Light and GAS tag availability from the T07 service rather than independently deciding donation. Otherwise the HUD may truthfully report the subsystem stage but the on-screen light and ability may disagree. This mismatch is an **integration blocker**, not permission to introduce another boolean.

## 5. Author real native Widget Blueprints in the Unreal Editor

A `UUserWidget` class in source is **not** a created `.uasset`. One Editor/MCP operator must create, compile, save and verify six actual widgets:

| Required WBP | Ownership | Widgets and required events |
| --- | --- | --- |
| `WBP_GPP_HUD` | HUD/UI owner | Three TextBlocks for `LightState`, `Objective`, `SaveStatus`, MVVM View Bindings |
| `WBP_GPP_MainMenu` | Menu owner (same Editor session) | New Game, Options, Controls, Quit |
| `WBP_GPP_Pause` | Menu owner | Resume, Save, Options, Controls, Quit |
| `WBP_GPP_Options` | UI/settings owner | Real Graphics Quality selector, VSync checkbox, Master Volume slider, Subtitles checkbox, Back |
| `WBP_GPP_Controls` | UI owner | Read-only current active Action → Key rows; Back |
| `WBP_GPP_Saving` | UI owner | Actual `Saving...` status overlay driven by accepted async request |

Place them under the game's owned UI namespace, for example `/Game/Core/UI/`, **only after inspecting existing assets**. A game with equivalent WBP assets should extend them instead.

### Concrete HUD ViewModel binding steps

1. Open `WBP_GPP_HUD` in the Widget Blueprint Designer.
2. Choose **Window → Viewmodels** and add `UGPPDemoHUDViewModel`; use the **Manual** creation mode so the Controller and HUD share **one ViewModel instance**.
3. In the widget initialization flow, resolve **Get Owning Player → Cast to AGPPDemoUIController → GetHUDViewModel**, then invoke the MVVM **Set Viewmodel** node for the ViewModel that this WBP declared. Check the actual UE 5.9 node signatures/tool schemas.
4. Create three text fields, with real Widget names and accessibility-friendly color/contrast.
5. In **View Bindings**, bind each TextBlock's **Text** property to the corresponding FieldNotify property. Bindings update when the ViewModel setter broadcasts; do not use raw every-frame widget-text evaluation as the primary solution.
6. Compile, **save and reopen** the WBP. Exercise the real T07 `OnProgressChanged` event and confirm the expected text changed on screen.

**If the Editor cannot create the required MVVM binding through its MCP tooling, stop the MCP mutation and use an actually supported Editor/manual route.** Do not claim a C++ ViewModel alone is a completed HUD.

### Concrete menu-event wiring

In each WBP Button's **OnClicked** event, get its owning PlayerController, cast to `AGPPDemoUIController`, call the corresponding method, and handle the returned success/failure. For the Main Menu, wire:

- **New Game** → `StartNewGame()`; on `false`, show an actual failure message and leave the menu on screen.
- **Options** → `ShowOptions()`.
- **Controls** → `ShowControls()`.
- **Quit** → `QuitGame()`.

In Pause, wire **Resume** → `ReturnToGame()`, **Save** → `RequestSave()`, and shared navigation buttons. In Options and Controls, use `BackToPreviousMenu()` to return to the originating main/pause context.

In Controls, call `GetActiveControlBindings()` on WBP creation or when shown and render **all returned entries** in a ScrollBox/ListView. This is actual runtime mapping data; the source does not offer fake rebinding buttons. A production remapping feature requires actual Enhanced Input User Settings and conflict/persistence checks.

## 6. Menu focus and pause correctness

The controller's screen switch code creates the required WBP, removes the prior screen, calls `SetGamePaused(true)`, sets `FInputModeGameAndUI` focused on that widget, and enables the mouse cursor. `ReturnToGame` removes the menu, unpauses, restores `FInputModeGameOnly` and hides the cursor.

The existing **Escape** key binding opens/closes Pause during gameplay. Under Game-and-UI input, a widget gets the first chance to handle a key and **unhandled Escape reaches the Controller's pause binding** (configured to execute when paused). The authored **Resume and Back buttons remain mandatory** and must be keyboard/gamepad navigable. Test Escape with both an unhandled focused widget and a focused control that consumes the key.

Opening any screen resets the existing T08 sprint speed via `ResetSprint()`, avoiding the common case where an input release never arrives after UI focus changes. If the target game uses another pawn or GAS-driven sprint, call that real speed authority instead.

**Failure mode to verify:** Menu creation can fail if a Widget class is unassigned. The source reports and returns `false`; don't fabricate a menu from a fake asset. Inspect the active GameMode/PlayerController, save owned WBPs and correct missing class references.

## 7. Real display, graphics, audio, game and controls options

A settings menu is only complete for a particular option when it **changes its actual runtime subsystem**, persists where required, and survives a fresh-process verification. Fake sliders or UI-only booleans are not allowed.

### Display/graphics: native User Settings

The sample's `SetGraphicsQuality(0..4)` updates the real `UGameUserSettings` scalability quality, calls `ApplySettings(false)` and `SaveSettings()`. `SetVSyncEnabled(bool)` uses the same persistent Engine service. These are operational settings, not decorative toggles.

Quality presets and VSync may have hardware or platform constraints. A `true` method return proves that the command path ran with expected inputs and a settings object, **not** that every rendering change is visibly confirmed on all devices; verify actual runtime result and persisted config.

**Display limitation:** the supplied source does not offer risky screen resolution or fullscreen selectors. A production release that promises them must implement `UGameUserSettings`' confirmed/revertible video mode flow, an actual timeout with correct pause behavior, and hardware testing **before displaying those selectors**. Do not present a nonfunctional resolution button.

### Audio: audible SoundClass/Mix routing

The operator authors and assigns a real `SoundMix` and **Master SoundClass** and routes actual sounds under that class. The controller uses `UGameplayStatics::PushSoundMixModifier` plus `SetSoundMixClassOverride` to apply `Volume` in `[0,1]` and persists the preference in `GGameUserSettingsIni`. On a fresh process it loads and reapplies the preference.

If either audio asset is missing, the command returns false and **does not claim success**. A moving slider cannot replace playback tests. Test audible music/SFX and inherited child SoundClasses as appropriate.

### Game: real subtitles preference

`SetSubtitlesEnabled(bool)` calls the Engine subtitle setting and stores the value separately from Run SaveGame. The functional check uses authored subtitle-bearing audio. Without subtitles present, the switch may be connected correctly but its **visible effect remains untested**.

### Controls: actual resolved mappings

`GetActiveControlBindings()` iterates the **current** `UEnhancedPlayerInput::GetEnhancedActionMappingsView()` and displays Action/Key names. It does not claim to offer fully persistent rebinding or automatically create input assets.

A production controls remapping UI needs User Settings, conflicts, supported devices, persistence and a safe binding for menu navigation. Add that only as a real independently specified behavior.

## 8. New Game, Continue, Save and loading: do not invent success

### New Game

The game starts with a valid real map, GameMode and possessed Pawn. `StartNewGame()` calls the existing `UGPPDemoRunSubsystem::StartNewRun` with the current map and real Pawn spawn, and it only enters gameplay if the subsystem accepted the request. It refuses an already active run instead of silently overwriting progress. For production, a second New Game during an existing run needs an explicit destructive-progress confirmation and a deliberate reset pathway.

This short example uses `GetWorld()->GetFName()` as its **in-session** map identity. A packaged Continue implementation must use **validated authored map identifiers**, not PIE-prefixed transient world names. This is part of T07's separate integration contract.

### Save

On Pause → Save:

1. The button calls `RequestSave()` on the **real T07 Run Subsystem**.
2. If a save is already in progress, missing or invalid, the operation returns false; the controller displays `Save rejected`.
3. Only after the real `RequestAsyncSave()` accepts the request does the HUD show `Saving...` and the `WBP_GPP_Saving` overlay appear (if assigned).
4. `OnSaveCompleted(bool)` is the **only** route that clears the Saving overlay and changes the status to verified success/failure.
5. T07's real readback and schema checks determine that callback; the UI does not infer Save success from file existence alone.

**No progress bar percentage is invented** because the existing T07 Save API does not report byte-level progress. Show indefinite activity until completion. Test actual callback behavior with pause active, and preserve the previous snapshot on failure.

### Continue / Load

The existing T07 `ReadBestSnapshotIntoMemory()` can validate and select an A/B snapshot. It does **not** guarantee that the player is in the right map, the World Partition cell loaded, the checkpoint safe, GAS reconciled or the actor states projected. A Continue button that only calls this method would be **fake gameplay completion**.

The sample therefore **does not implement or display Continue**, by deliberate scope boundary. For a real game's approved T09 feature, integrate the full T07 load coordinator:

1. Read and validate candidate snapshot.
2. Resolve and load the **actual** authored map, with a loading screen whose lifetime survives level transition if necessary.
3. Wait for required World Partition streaming completion and validated spawn/checkpoint collision.
4. Restore pawn location, GAS tags/abilities, fragments, permanent landmark visuals and collision idempotently.
5. Rebuild ViewModel projection after world state is applied.
6. Return input and remove the loading screen only when a real playable state has been reached.
7. Handle missing/corrupt snapshots without claiming Continue success.

This is an **actual implementation requirement**, not a readiness milestone. It cannot be marked Implemented by merely creating `WBP_Loading` or animating a progress bar for a timer.

## 9. Actual acceptance plan (not test results)

Every case below is **NOT RUN** at publication. Execute on a real UE 5.9 game project and capture evidence individually.

| ID | Operator action | Required real result |
| --- | --- | --- |
| UI-01 | Compile the actual source module with MVVM enabled | Native Editor target compiles; no missing generated headers |
| UI-02 | Inspect six authored Widget Blueprints | Real WBPs exist, compile, save and reopen |
| UI-03 | Launch the configured game map | Intended PlayerController and Pawn exist; no duplicate UI |
| UI-04 | Click New Game | Real `StartNewRun` accepted; gameplay unpauses and input returns |
| UI-05 | Start New Game twice/with active run | Second action cannot overwrite an active run silently |
| UI-06 | Begin game and observe HUD | Objective and Light stage read from existing subsystem |
| UI-07 | Donate Light via the **actual integrated** gameplay path | ViewModel displays Donated; ASC blocks Light |
| UI-08 | Recover via a distinct **actual** fragment | ViewModel displays Recovered; personal Light works again |
| UI-09 | Press Escape during gameplay | Pause screen opens; cursor and input focus switch correctly |
| UI-10 | Click Resume | World unpauses; gameplay input and camera resume |
| UI-11 | Pause while sprinting | Player speed resets instead of remaining accelerated |
| UI-12 | Open/Back from Options and Controls | Return to correct main/pause context, no dead buttons |
| UI-13 | Click Save with active valid run | Async request accepted, “Saving...” appears |
| UI-14 | Wait for actual Save completion callback | Overlay clears; success based on T07 verified readback |
| UI-15 | Request Save while another save is pending | Rejection shown, not false success or duplicate write |
| UI-16 | Fail a Save operation under controlled conditions | Failure visible, old save preserved, UI recovers |
| UI-17 | Select graphic preset | Actual Engine scalability changes and is saved |
| UI-18 | Toggle VSync | Engine setting applies and persists in new process |
| UI-19 | Move Master Volume slider | Actual routed audio changes continuously |
| UI-20 | Restart executable | Master volume preference reapplied from saved config |
| UI-21 | Toggle subtitles on a valid authored subtitle cue | Actual subtitle visibility changes and persists |
| UI-22 | Open Controls after remapping (if supported by target project) | Displays resolved current Action/Key mapping, not hardcoded keys |
| UI-23 | Use keyboard/gamepad on every menu | Real focus, Back/Resume and accessibility routes work |
| UI-24 | No valid snapshot/unsupported snapshot | Continue unavailable; no false load-success screen |
| UI-25 | Load a saved run **only once real world resume exists** | Correct map, checkpoint, GAS, world and UI state restored before control |
| UI-26 | Cook/package Windows | Widgets and MVVM dependencies included; no missing asset errors |
| UI-27 | Launch packaged executable fresh | New Game, HUD, Pause, Save, Settings and Controls work as recorded |

**UI-25 is BLOCKED until an actual load coordinator is integrated.** Do not mark it PASS from this narrow example. Requirements can be conditionally **Not Applicable** only when the human explicitly cuts them from the target game's approved scope; they cannot disappear from a required end-to-end feature without a decision.

Acceptance evidence should cite source revision, actual Engine build, the assigned Widget Blueprint package paths, screenshot/video from *real PIE*, on-disk settings and save callback logs, independently verified packaged launch, plus a named human reviewer.

## 10. Common integration errors

| Symptom | Real diagnosis and corrective action |
| --- | --- |
| FieldNotify property not listed | Verify `UMG Viewmodel` plugin, `ModelViewViewModel` dependency, correct UHT and Blueprint exposure |
| C++ ViewModel exists but HUD stays blank | Author WBP and View Bindings, assign the one live ViewModel instance; inspect initialization order |
| HUD reads Owned while actual personal Light is donated | T05 Character and T07 Run are competing; consolidate authority and reconcile GAS |
| Pause menu opens but cannot resume | Verify Game-and-UI focus, actual Resume/Back event and PlayerController class |
| Esc unpauses only sometimes | Input routing depends on game-only/UI-only modes; handle widget's menu keys explicitly |
| Saving overlay disappears immediately | Someone is confusing command acceptance with async callback; only completion event dismisses it |
| Continue opens wrong or empty world | Restore logical data + map/stream/checkpoint + Actors/GAS before showing gameplay |
| Settings switch changes text but not graphics | Bind button to `UGameUserSettings` command and inspect actual engine value |
| Volume slider moves but game remains equally loud | Wrong/missing SoundClass routing or SoundMix asset, no routed playback, platform audio |
| Controls page shows outdated bindings | Query resolved Enhanced Input mappings after user change, not cached template text |
| Gameplay resumes with sprint stuck | Reset the real active CharacterMovement/GAS sprint authority on focus takeover |
| Widget compiles in Editor but missing in packaged build | Save/cook references, actual GameMode/Controller assets, enabled MVVM plugin and packaging log |

No defect is corrected by creating a fake success flag, replacing the real PlayerController without inspection, using a timer to pretend loading is finished, or deleting unowned dirty packages.

## 11. Ownership and evidence

**Code owner:** preserve existing T05/T07 contracts, extend the actual game's UI/session authority, compile and report concrete source and tests.

**Editor owner:** one active Unreal Editor/MCP operator authors all six real WBPs and any SoundClass/SoundMix assets, binds controls, saves/reopens packages and reports exact results. They do not claim human PIE from Editor compilation.

**Human Game Director/tester:** actually plays menu/gameplay interactions, validates feedback/readability and accepts or rejects against the approved Feature Spec.

**Documentation owner:** reconciles real reports and human decisions, marking each stage Planned → In Progress → Implemented → Tested → Human Validated → Released. Save, load and settings test cases remain explicit; no fabricated PASS or private asset publication.

**Next tutorial:** [T10 — Visual references and asset-production briefs](README.md), connecting approved art direction to traceable, legally usable 3D production inputs.
