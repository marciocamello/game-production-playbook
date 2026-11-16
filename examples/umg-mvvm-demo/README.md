# Unreal Engine 5.9 — Functional UMG/MVVM HUD and Menu Operations

This example supplies **four original C++ implementation files**, not empty classes or dummy widgets:

- [GPPDemoHUDViewModel.h](Source/GPPDemoHUDViewModel.h) and [GPPDemoHUDViewModel.cpp](Source/GPPDemoHUDViewModel.cpp): event-driven `UMVVMViewModelBase`, three `FieldNotify` properties and real stage/save feedback.
- [GPPDemoUIController.h](Source/GPPDemoUIController.h) and [GPPDemoUIController.cpp](Source/GPPDemoUIController.cpp): operational New Game, pause/resume, Options/Controls navigation, Save, Quit, persistent graphics/VSync, audio volume, subtitles and current Enhanced Input mapping display.

**Status:** Source authored and published. **No installed UE 5.9 build, Editor-imported Widget Blueprint, manual PIE or packaged-game run has been performed by this repository.** Every execution check remains NOT RUN unless a reader actually performs it. The code targets the existing T05/T08 Character and the existing T07 `UGPPDemoRunSubsystem`; source integration and Editor asset creation are real prerequisite work, not implied by this download.

## 1. Reuse rather than duplicate

Before copying any file, inspect whether your actual project already owns its UI session in a PlayerController, HUD class, GameInstance, ViewModel, input router, audio subsystem or Settings UI. **Extend that working owner instead of creating a second PlayerController or progress authority.** These files are a complete bounded teaching implementation for a compatible project that has **not** already implemented the behavior.

The example is independent of the original *The Unmade* repository. Its names, code and source paths do not mirror private game implementation.

## 2. Required modules/plugins and source compatibility

The target native game module must already compile and own the T05/T08 Character source, T07 SaveGame and Run Subsystem source, and real input mappings. Check actual existing `.Build.cs` before extending it.

Required modules for the combined example: `Core`, `CoreUObject`, `Engine`, `InputCore`, `EnhancedInput`, `GameplayAbilities`, `GameplayTags`, `GameplayTasks`, `UMG`, `ModelViewViewModel`, plus other **already consumed** modules. Enable the official **UMG Viewmodel** plugin in the real Editor before compiling MVVM.

Do not install another progression subsystem: T09 calls the **existing** `UGPPDemoRunSubsystem` methods and subscribes to its existing `OnProgressChanged` / `OnSaveCompleted` delegates. T07 has no complete map-resume coordinator; consequently, this example **does not present a Continue action**.

### Existing T05 state versus T07 authority

T05's Character has its own in-session `LightStage`. T07's Run Subsystem is the intended durable authority. If you integrate both, first **reconcile the existing Character and GAS projection with the T07 run state** as specified in [T07](../../tutorials/savegame-reload-world-partition.md); do not leave two competing state machines. T09's ViewModel reads the T07 state, not the separate T05 Character enum. This example does not secretly solve that integration gap.

## 3. Create the actual UMG widgets

The four C++ files **cannot replace** these Editor-authored native assets. One authorized Editor operator creates, wires, compiles and saves them:

| Widget Blueprint | Base class | Required real behavior |
| --- | --- | --- |
| `WBP_GPP_HUD` | `UserWidget` | Actual `LightState`, `Objective`, `SaveStatus` text bound via the shared native ViewModel |
| `WBP_GPP_MainMenu` | `UserWidget` | New Game, Options, Controls, Quit buttons invoke controller operations |
| `WBP_GPP_Pause` | `UserWidget` | Resume, Save, Options, Controls and Quit invoke controller operations |
| `WBP_GPP_Options` | `UserWidget` | Real quality 0–4, VSync, Master Volume 0–1, subtitles, Back |
| `WBP_GPP_Controls` | `UserWidget` | Displays actual active Input Action/key mappings from the controller and Back |
| `WBP_GPP_Saving` | `UserWidget` | Explicit saving-in-progress overlay, shown after `RequestAsyncSave()` is accepted |

In the **HUD Designer**, open **Window → Viewmodels** and add the native `UGPPDemoHUDViewModel`. Use the **Manual** creation type and set the ViewModel instance in the widget's initialization logic by resolving **Get Owning Player → Cast to `AGPPDemoUIController` → GetHUDViewModel**, then using the installed MVVM **Set Viewmodel** node. Connect `LightState`, `Objective` and `SaveStatus` through the View Bindings panel to actual `TextBlock.Text` properties. Verify that changing T07 run state produces a field notification and updates the text.

For each button, use `OnClicked` in its owning Widget Blueprint to get its owning controller, call the matching native UFUNCTION and **handle the returned bool**. For example, a failed Save request must not display “Saved.”

In GameMode/World Settings, set the real active PlayerController class to `AGPPDemoUIController` (or integrate the methods into the **existing** PlayerController) and the Default Pawn to the existing T05/T08 Character. Assign all six Widget classes in the controller's default properties. Do not use the base `APlayerController` and expect these methods to appear.

## 4. Wire real sound routing

To make volume audible rather than a text-only slider:

1. Author a **real** `SoundClass` for game audio (master class) and route actual music, ambience and SFX under it or to child SoundClasses.
2. Author a **real** `SoundMix` and assign both sound assets to the controller properties `MasterSoundMix` and `MasterSoundClass`.
3. The native function calls `PushSoundMixModifier` and `SetSoundMixClassOverride` to adjust the assigned class at runtime. It persists volume in the platform's `GameUserSettings` INI through `GConfig`.
4. Confirm sound playing at volume 1.0 is audible and volume 0.0 is effectively silent, without muting unrelated audio routes unintentionally.
5. Restart the game in a fresh process and verify the saved volume is reapplied. A missing SoundClass/Mix causes the function to return `false`, not fake a success.

**Audio compatibility:** Unreal's audio device and SoundClass routing should be validated on the installed build and real playback hardware.

## 5. Graphics, display and game settings

- `SetGraphicsQuality(int32)`: rejects invalid levels and applies `UGameUserSettings::SetOverallScalabilityLevel` + `ApplySettings` + `SaveSettings`.
- `SetVSyncEnabled(bool)`: applies and persists real VSync through `UGameUserSettings`.
- `SetSubtitlesEnabled(bool)`: calls Unreal's subtitle service and persists the preference through `GConfig`. Verify with actual subtitle-bearing dialogue/audio; a checkbox changing alone is not sufficient.
- `GetActiveControlBindings()`: reads **current resolved mappings** from `UEnhancedPlayerInput`, rather than listing guessed keys. It supplies a Controls **viewer**, not remapping. Do not place fake “Rebind” controls in this example.
- **Resolution/fullscreen changes are intentionally not supplied** by this source: they require a safe confirmation/revert flow and a fresh-process check. Do not add unimplemented resolution selectors.

Menu input: the controller binds Escape to `TogglePause` in the game. UMG is given UI-only input while a menu is visible, so **Resume/Back buttons must work independently**; do not assume Escape will always reach game input in a UI-only mode. More comprehensive keyboard/gamepad navigation and remapping must be integrated and tested to satisfy a production game's accessibility/rebinding contract.

## 6. New Game, Save and Continue boundaries

**New Game:** The main menu starts with the real level already loaded and the real Default Pawn possessed. `StartNewGame()` calls the existing `StartNewRun(MapId, CheckpointId, PawnLocation)` once, and only transitions into gameplay on success. It refuses an active run. The example intentionally does not overwrite progress from a running session.

**Save:** A pause-menu button calls the existing `RequestAsyncSave()`. The ViewModel shows `Saving...` only for an accepted request. It waits for the actual `OnSaveCompleted(bool)` event, clears the overlay and reports either `Save verified` or a failure. This does not prove that the scene can be restored without the rest of T07's map/Actor integration.

**Continue:** The existing T07 `ReadBestSnapshotIntoMemory()` restores only logical values. It does **not** load/validate the correct map, stream needed World Partition regions, spawn the pawn at a safe checkpoint or reconcile GAS/Actors. The example therefore **does not expose a Continue button or a pretend successful loading screen**. Implement that actual route and acceptance before enabling Continue in any game's real UI.

**Saving overlay versus level loading:** `WBP_GPP_Saving` represents real **asynchronous save I/O**, not progress for a map load. A level-loading screen needs a real map transition coordinator and its own verifiable lifetime, not a timer-based decorative animation.

## 7. Basic runtime verification (all NOT RUN here)

| Test | Actual requirement |
| --- | --- |
| UI-01 | Native Editor target builds with UMG/MVVM dependencies |
| UI-02 | All six actual WBP assets exist, compile, and are saved |
| UI-03 | The actual GameMode selects the UI controller and the intended pawn |
| UI-04 | Main Menu New Game opens gameplay only on a real successful `StartNewRun` |
| UI-05 | Pause/Resume works and restores input and mouse cursor |
| UI-06 | HUD reflects T07 Owned → Donated → Recovered without polling/fake flags |
| UI-07 | Save overlay appears after accepted async save and clears on real callback |
| UI-08 | Invalid/busy Save requests do not show “Saved” |
| UI-09 | Graphic quality and VSync actually apply and persist |
| UI-10 | Master Volume changes actual playing audio and survives fresh-process launch |
| UI-11 | Subtitles setting affects real authored subtitles and persists |
| UI-12 | Controls page displays current mapped keys (read-only, no fake rebinding) |
| UI-13 | Continue is not presented until the actual T07 load coordinator exists |
| UI-14 | Packaged standalone game includes and uses real HUD/menu assets |
| UI-15 | Human manual PIE and packaged interaction are recorded separately |

See the full [T09 tutorial](../../tutorials/umg-mvvm-hud-menus-settings-loading.md) for failure modes, Editor handoff and detailed acceptance.

## Rights

These four source files are original MIT-designated educational code under [LICENSE-CODE.md](../../LICENSE-CODE.md). This example includes no third-party Unreal binary asset, private game code or personal machine paths.
