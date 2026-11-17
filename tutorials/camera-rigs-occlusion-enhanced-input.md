# T08 — Camera Rigs, Occlusion and Enhanced Input in Unreal Engine 5.9

**Publication status:** Authored tutorial and original C++ extension of the existing [T05 Character](../examples/gas-light-demo/README.md). **This repository has not compiled the extension, created native Editor Camera Assets, run PIE or launched a packaged build against an installed UE 5.9 environment.** Those tests begin NOT RUN.

**Target:** Unreal Engine 5.9, C++ + Blueprints, single-player Windows PC, elevated/isometric exploration. The concrete source example uses the traditional **Camera Component + Spring Arm** path. A second Editor-only route explains **Gameplay Cameras / Camera Assets** when complex camera rigs and transitions are genuinely needed. These are **alternative camera authorities**; do not accidentally drive the same player view with both.

**Player-visible goal:** camera-relative movement, camera collision without clipping or uncontrolled framing collapse, bounded smooth wheel zoom, sprint that resets on release/cancel, and reliable return to gameplay after UI or cinematic control changes.

## 1. Sources, version-specific limits and project preflight

Official documentation reviewed for this tutorial:

- [Epic — Spring Arm Component API](https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Runtime/Engine/USpringArmComponent): `TargetArmLength`, `bDoCollisionTest`, `ProbeChannel`, `ProbeSize`, `GetUnfixedCameraPosition()` and `IsCollisionFixApplied()`.
- [Epic — Enhanced Input](https://dev.epicgames.com/documentation/unreal-engine/enhanced-input-in-unreal-engine): Input Actions, Mapping Contexts, modifiers and action-event bindings.
- [Epic — ETriggerEvent API](https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Plugins/EnhancedInput/ETriggerEvent): `Started`, `Triggered`, `Completed`, `Canceled`.
- [Epic — Gameplay Camera System Overview](https://dev.epicgames.com/documentation/unreal-engine/gameplay-camera-system-overview): Camera Assets, rigs, Camera Director and transitions; feature documented as **Experimental**.
- [Epic — Gameplay Camera System Quick Start](https://dev.epicgames.com/documentation/unreal-engine/gameplay-camera-system-quick-start): a concrete Editor authoring workflow.
- [Epic — APlayerController::SetViewTargetWithBlend](https://dev.epicgames.com/documentation/unreal-engine/quick-start-guide-to-implementing-automatic-camera-control-in-unreal-engine-cpp): switching view targets and transition parameters.
- [T05 — Gameplay C++, Gameplay Tags and GAS](gameplay-gas-tags-light.md) and the existing [Unreal local build guide](unreal-project-bootstrap-local-build.md).

Epic pages can display a default documentation version other than the Engine build on your machine. Check the actual installed UE **5.9** headers, plugin availability, Editor toolsets and asset types before assuming an API works. Gameplay Cameras must be tested with the installed plugin/version if selected for shipping.

Before making any code or Editor changes, inspect:

1. The real project revision, local modified files, native PlayerController/Character, Camera Component, Spring Arm, any existing Gameplay Camera Asset, and saved/unsaved map packages.
2. Existing Enhanced Input Mapping Contexts, Player Mappable key settings, actions, UI input modes and any sprint or zoom functionality. **Reuse it if already working**.
3. Player capsule dimensions, camera origin, level collision and target game camera angle; confirm which objects are *real blockers* and which are decorative or should fade/ignore the Camera channel.
4. Ownership: one source owner for each C++ file and **one operator for the Unreal Editor/MCP session**.
5. Target platform and intended input: keyboard/mouse, gamepad, touch, camera switch keys, camera accessibility and pause/Options requirements.

A static concept image and an Editor viewport screenshot do **not** establish how the actual possessed camera behaves in PIE.

## 2. Choose the camera authority that the game actually needs

| Approach | Best use | Required deliverables | Main risks |
| --- | --- | --- | --- |
| **Spring Arm + Camera Component** | Fixed elevated view, stable follow, smooth wheel zoom, simple levels | Real components and input bindings on an existing possessed Character | Boom retracts against `ECC_Camera` blockers; badly tuned capsule/level collision or camera offsets can destroy framing |
| **Gameplay Camera System** | Several authored rigs, designer-driven transitions, top-down/third-person or cinematic blends | Enabled Gameplay Cameras plugin, **saved Camera Asset** with rig(s), director selection, and a real consuming actor/component | Experimental plugin, authored assets may be missing, runtime camera ownership/transition may not be wired |
| **Explicit Camera Actors + PlayerController** | A few predetermined viewpoints or simple scripted transitions | Existing camera actors and actual `SetViewTargetWithBlend` calls, plus input restoration | Conflicting view-target ownership, forgotten return from cutscene, movement/camera mismatch |

For a compact, elevated single-player adventure, start with the **existing** Spring Arm unless authored multi-rig behavior demonstrably improves the game. Adding Gameplay Cameras or multiple controller managers just to appear technologically advanced is not a feature.

## 3. Why Spring Arm collision is not the same as camera zoom

The Spring Arm's `TargetArmLength` is the **requested** camera distance. With `bDoCollisionTest` enabled, Unreal traces a sphere along the desired camera route and can **retract** the actual child Camera to avoid clipping. The apparent camera distance can therefore change **without the user zooming**.

Two numbers must be measured separately:

- **Desired camera distance:** the current zoom objective and `TargetArmLength`.
- **Actual collision correction:** `IsCollisionFixApplied()` and the distance from `GetUnfixedCameraPosition()` to the current camera position.

The [T05/T08 Character source](../examples/gas-light-demo/Source/GPPLightCharacter.cpp) now exposes both camera collision diagnostics through `IsCameraRetracted()` and `GetCameraRetractionCm()`; it also emits a log only when the collision correction begins or clears. This helps distinguish a correct collision response from a bad target-length interpolation.

### Do not hide camera bugs by clamping the wrong variable

An earlier production camera pass in *The Unmade* discovered that inherited Spring Arm collision updates retracted an elevated camera near walls despite intended steady framing. Changing interpolation alone could not fix the underlying collision behavior. A separate later integration test found **one missing Camera Asset**, despite 34/35 tests being reported as passing; that failure was unrelated to zoom math.

**Diagnosis sequence:**

1. Freeze the intended zoom input; record `TargetArmLength` while moving near a wall.
2. Observe `IsCollisionFixApplied()` and the actual camera location. If the target is unchanged but the camera retracts, this is **collision response**, not user zoom.
3. Inspect all overlapping/intersecting Actor components and their `ECC_Camera` responses. Nonblocking decorative foliage, VFX or collision helper volumes should not accidentally block the camera probe.
4. Check actual CharacterMovement capsule shape, camera arm origin, `ProbeSize`, `ProbeChannel`, camera offset and stairs/arches along the travel route.
5. Decide, by approved level design, whether to (a) allow intentional camera retraction, (b) have *selected* obstructing elements ignore the Camera channel, (c) add a properly implemented occluder-fade/relocation system, or (d) author a different camera rig.
6. Test the result at the **real player angle**, including narrow stairs, bridges, tall arches and the distance at which interactions are readable.

**Never blanket-ignore all world collision** just to preserve the arm length. Conversely, never let a nonfunctional collision volume cause the entire screen to fill with the character's head. If constant-distance is a hard creative requirement, disabling collision must be paired with a verified alternative occlusion solution for the game's actual geometry.

## 4. Reuse the existing T05 Character: real C++ zoom and sprint

The public example previously contained:

- `AGPPLightCharacter` with actual CharacterMovement and an elevated `USpringArmComponent`.
- `UCameraComponent` attached at the boom tip.
- Camera-relative `Move` implemented using the Camera yaw.
- Four input actions for movement, personal Light, donation and recovery.

T08 **extends these exact files**, rather than creating a second Character/camera hierarchy:

- [GPPLightCharacter.h](../examples/gas-light-demo/Source/GPPLightCharacter.h) — optional `ZoomAction`, `SprintAction`, configurable distance/speed ranges, Blueprint-readable collision diagnostics and `ResetSprint()`.
- [GPPLightCharacter.cpp](../examples/gas-light-demo/Source/GPPLightCharacter.cpp) — new action bindings, frame-rate-independent boom interpolation via `FMath::FInterpTo`, camera collision probes, walk/sprint speed changes and reset on possession changes.
- [Existing T05 example README](../examples/gas-light-demo/README.md) — additional integration instructions without invalidating the original four-action exercise.

**No new runtime gameplay module, fake camera class or unintegrated function stub has been added.** The implementation is actual source for the existing demonstration character. The code still needs native build, asset binding and PIE acceptance in an installed game.

### Camera configuration in the source

The extended Character configures a fixed elevated boom and makes collision intent explicit:

```cpp
CameraArm->TargetArmLength = 560.0f;
CameraArm->SetRelativeRotation(FRotator(-55.0f, 45.0f, 0.0f));
CameraArm->SetUsingAbsoluteRotation(true);
CameraArm->bDoCollisionTest = true;
CameraArm->ProbeChannel = ECC_Camera;
CameraArm->ProbeSize = 12.0f;
CameraArm->bEnableCameraLag = false;
```

These are **illustrative starting values for the example**, not a recommended universal camera profile. Check real level scale, capsule, platform target and view angle. An optional later Camera Lag/rotation smoothing pass must be benchmarked separately; it must not be substituted for collision diagnosis.

### Smooth zoom

`IA_GPP_Zoom` produces an Axis1D value, normally from a mouse-wheel axis. The handler adjusts a **desired length**, clamps it to the owned min/max, and the Character's tick interpolates the actual `TargetArmLength` toward that requested length.

```cpp
DesiredCameraArmLength = FMath::Clamp(
    DesiredCameraArmLength - Axis * CameraZoomStep, MinimumLength, MaximumLength);
```

Frame-rate-dependent raw step changes are avoided by using `FMath::FInterpTo` in Tick with `DeltaSeconds`. Zoom bounds and step values are editable on the existing Character Blueprint. The **actual** view may still retract because of collision; diagnostics must distinguish both behaviors.

### Sprint and reset

`IA_GPP_Sprint` is Boolean. The existing Character binds `Started` to a real change in `CharacterMovement.MaxWalkSpeed`, and `Completed` and `Canceled` to restoring `WalkSpeed`. It also resets speed on `UnPossessed` and `PawnClientRestart`.

This avoids treating sprint as a second movement system. It does not require GAS for an unrestricted speed increase. If your game already uses GAS speed GameplayEffects or stamina, integrate with **that** authority instead of setting a competing `MaxWalkSpeed`.

When a pause menu, Options UI or cinematic takes focus without delivering a release/cancel event, the owning Controller/UI must invoke the existing public **`ResetSprint()`** before switching input mode. Do not assume lost key events will always fire a reset. The example provides the real reset function; the game must wire it in its own UI/Controller implementation.

## 5. Author the real Enhanced Input assets

A C++ `UInputAction*` property is **not** an Input Action asset. The sole Editor owner must author, assign, compile and save the following assets in the actual project.

Retain the original T05 actions for Q, E, R and WASD. Add:

| Asset | Value | Example binding | Real behavior |
| --- | --- | --- | --- |
| `IA_GPP_Zoom` | Axis1D | Mouse Wheel Axis with signed values | Adjusts smooth requested camera distance |
| `IA_GPP_Sprint` | Boolean | Left Shift | Starts movement-speed increase; release/cancel returns to walk |

Add both to the existing **`IMC_GPP_Demo`** and bind the assets to the existing **`BP_GPP_LightCharacter`** properties `ZoomAction` and `SprintAction`. Do **not** create another Mapping Context for the same action set unless the real project requires alternate control profiles.

**Editor input setup procedure:**

1. Inspect existing `IMC_GPP_Demo` and `IA_GPP_Move`, confirming four-direction movement from the earlier demo.
2. Create a new Axis1D `IA_GPP_Zoom`. Configure the appropriate installed Engine mouse-wheel key mapping and signed axis behavior. Confirm the sign by printing/inspecting real `FInputActionValue` values and use a **Negate** modifier if the game's requested wheel direction is opposite.
3. Create Boolean `IA_GPP_Sprint` and assign Left Shift. Use ordinary press/release trigger semantics; do not add a Hold trigger if you need `Started` immediately and `Completed` on release.
4. Extend the existing mapping context with **only** these actions. Do not clear unrelated mappings or break E, R, Q or movement.
5. Assign the two assets in the Character Blueprint's exposed `ZoomAction`/`SprintAction` properties. Compile/save the Blueprint and the Mapping Context.
6. Verify the Character is the actual possessed Default Pawn and that the GameMode/map is the intended one.
7. Run PIE and inspect the actual camera zoom, collision and sprint behavior.
8. Configure **player-mappable bindings** and a functional Controls page later if your approved project requires remapping (T09). Merely setting an IMC key is not a complete user Settings feature.

If no valid action or context is assigned, that feature cannot activate. The C++ example makes these two actions optional for T05 backward compatibility; **T08's own acceptance requires that the real assets exist and are assigned.**

## 6. Debug camera retraction and occluders in a real level

Use actual PIE geometry, not an empty Preview viewport.

**Test locations:** open plaza, close wall, bridge approach, under a low arch, stairs, foliage, behind a large landmark and a narrow path alongside the Character capsule.

### First measure, then change

1. In open space, inspect `TargetArmLength` and the Camera world location. `IsCameraRetracted()` should normally be false unless the camera probe overlaps a blocker.
2. Move toward a real wall while keeping wheel input constant. If the camera is displaced, `GetCameraRetractionCm()` reports the distance between the unfixed target and the actual camera while collision correction applies.
3. Check which component responds to **Camera trace channel**; distinguish the actor's physical collision (Pawn movement) from its camera occlusion response.
4. Check decorative objects and helper trigger volumes. Make a selected volume ignore `ECC_Camera` **only if** it is not an actual camera occluder.
5. Approach the same location from the other direction and at default/min/max zoom. Verify readable framing and no visible geometry clipping.
6. Repeat while sprinting, so the camera cannot be “fixed” by a slow walk-only test.
7. Capture a real PIE screenshot or clip, the related map/package revision and the observed retarget/retraction values.

`CameraArm->bEnableCameraLag` is intentionally off in the source so the first collision investigation has one fewer moving variable. Enable/tune camera lag only after the collision behavior is acceptable.

### A useful debug matrix

| Observation | Likely diagnosis | Corrective direction |
| --- | --- | --- |
| Desired distance remains 560 but view rapidly contracts at wall | Spring Arm collision retracting | Inspect actual blocker and probe channel, not zoom interpolation |
| Zoom changes while scroll input is zero | Another camera writer or Blueprint modifying TargetArmLength | Find that real writer and consolidate authority |
| Camera clips through wall | World mesh responds Ignore/Overlap to Camera or probe is disabled | Verify collision responses, probe size, level geometry |
| Camera sticks to character in open space | Boom origin overlaps blocker or self-collision; wrong inherited setup | Inspect arm attachment, capsule shape, collision and Actor hierarchy |
| Character changes facing but camera rotates unexpectedly | Component inherits pawn/control rotation, Blueprint overwrites settings | Inspect `SetUsingAbsoluteRotation`, yaw flags, existing camera systems |
| Camera view is correct in Editor but bad in PIE | Wrong possessed pawn/view target or different PlayerController | Verify actual active runtime camera and map overrides |
| Camera is fine at one zoom but hides interaction at another | Unvalidated min/max FOV/composition | Tune bounds using gameplay objective readability |
| Sprint never resets after menu focus changes | Release did not reach input subsystem | Call `ResetSprint()` in real UI handoff and retest |

Do not fix a collision problem by changing arbitrary interpolation parameters, hiding the entire level or forcing all walls to ignore collision.

## 7. Optional Gameplay Camera Asset route for multiple rigs

When the game needs actual authored camera transitions rather than one steady view, use the **Gameplay Cameras** plugin and its documented Editor authoring workflow. It is a **separate alternative** to the simple Spring Arm demonstration.

1. Confirm plugin support and version in the **installed UE 5.9** Editor. Epic describes Gameplay Cameras as **Experimental**; verify packaging compatibility before committing to a release.
2. Enable **Gameplay Cameras** in the Editor Plugins window and restart if required.
3. In the **Content Browser → Gameplay → Camera Asset**, create a **real saved Camera Asset**. Select a Camera Director type supported by the installed plugin. For one intended rig, a Single Camera Director may suffice; for switching, use an appropriate Blueprint/State Tree director actually available in the installed version.
4. Author an actual **Top Down / elevated rig**, its positional/rotation behavior and its desired transitions in that Camera Asset. The asset must contain a valid rig and director selection, not merely exist as an empty named package.
5. Add a supported Gameplay Camera Actor or Component that consumes that exact Camera Asset. Configure the PlayerController's actual camera/view-target integration and validate the active rig in PIE.
6. If switching between camera rigs, define **explicit trigger conditions** (e.g. reaching a bridge approach), select valid rigs and verify transitions in both directions. Ensure movement remains camera-relative and the active camera is used by interactions.
7. Verify the Camera Asset is saved, referenced, cooked, and still works in a **fresh packaged process**. Use the plugin's Camera Debugger when available to inspect actual active rigs and evaluators.

**Never claim a Gameplay Camera feature is integrated because a C++ camera component or `.Build.cs` dependency exists.** In the historical case study, an integration test failed precisely because the expected Camera Asset was null. If the asset has not been authored, the corresponding test is **FAIL or BLOCKED**, not PASS.

### Camera switching without the Gameplay Cameras plugin

For a small number of fixed view actors, the documented `APlayerController::SetViewTargetWithBlend` operation can transition to a real `ACameraActor` and back to the pawn. The return is part of the **actual feature**: restore the prior view target, input focus and movement after the cinematic. A design paragraph alone does not provide the Actor or bindings.

Do not activate **both** an implicit Spring Arm camera and an independent Gameplay Cameras director for the same view without a defined owner and transition contract.

## 8. Interaction design and accessibility constraints

Camera behavior must support the game, not merely look impressive.

- At every mandatory interaction, show target, prompt and object silhouette clearly from the active runtime camera.
- Allow the player to move away from an occluder and recover the intended view without becoming stuck.
- A camera angle must never force use of an ability already donated in a progression-critical area.
- Keep controls consistent after pause, cinematic and perspective changes; do not leave sprint enabled when input focus changes.
- Prefer camera transitions that avoid disorientation, excessive motion, flashing or unbounded speed/zoom.
- Test at representative resolutions and aspect ratios, not solely Editor viewport dimensions.
- When user Settings include adjustable camera behavior/sensitivity, persist them through the actual Options system (T09) rather than a temporary Blueprint variable.
- Consider always-visible focus cues or bounded transparency for tall occluders when the camera design requires it; such a system must be **implemented and validated** if adopted.

## 9. Actual acceptance tests (requirements only)

These tests are **not pre-filled PASS results**. They require a real project, actual Engine compile, Editor asset authoring and human PIE.

| ID | Verification action | Required observed outcome |
| --- | --- | --- |
| CAM-01 | Build modified T05 C++ with installed UE 5.9 | Compilation succeeds and native module loads without missing symbols |
| CAM-02 | Inspect existing Character/CameraArm/Camera components | Exactly one intended runtime camera authority; components attached correctly |
| CAM-03 | Create and save the two new Input Actions | Correct value types and real assets assigned to Blueprint |
| CAM-04 | Start PIE with actual pawn | Input, camera and normal gameplay controls work |
| CAM-05 | WASD in four directions | Camera-relative movement remains correct |
| CAM-06 | Mouse wheel forward/backward | Smooth zoom changes in the intended direction |
| CAM-07 | Continue scrolling past zoom limits | TargetArmLength clamps to configured minimum/maximum |
| CAM-08 | Repeat with uneven frame times | Zoom remains visually stable; no large overshoot or oscillation |
| CAM-09 | Hold Left Shift while moving | Actual CharacterMovement speed increases |
| CAM-10 | Release Left Shift | Speed returns to WalkSpeed |
| CAM-11 | Trigger an input cancellation | Sprint returns to WalkSpeed |
| CAM-12 | Enter pause/Options/cinematic during sprint | Actual input-owner handoff invokes ResetSprint and walk speed is restored |
| CAM-13 | Exit pause/cinematic | Correct Camera/view target and input mode are restored |
| CAM-14 | Open plaza without occluders | No unexplained camera collision correction |
| CAM-15 | Walk against a real blocking wall | Camera neither clips into the wall nor collapses unacceptably; retraction is diagnosed |
| CAM-16 | Walk under an arch or near stairs | Camera stays readable, collision and pawn movement behave correctly |
| CAM-17 | Walk past foliage and decorative triggers | Nonblocking decoration does not cause unintended camera collapse |
| CAM-18 | Sprint around a tight corner | No stuck camera, uncontrollable motion or softlock |
| CAM-19 | Change zoom at close wall | Collision retraction and requested zoom are distinguishable |
| CAM-20 | Compare full-screen window/resolution/aspect ratios | Interaction target and player remain legible |
| CAM-21 | Switch camera rig/actor, **if adopted** | Actual authored asset/actor activates and returns correctly |
| CAM-22 | Save/reopen camera Blueprint/Asset | Assigned controls and camera rig persist in real native packages |
| CAM-23 | Execute recorded camera-focused automated tests (when available) | Named test cases run and results inspected individually |
| CAM-24 | Cook/package a Win64 build | Correct camera/input/asset references included; no missing Camera Asset |
| CAM-25 | Fresh packaged launch and human traversal | Camera, sprint, zoom, interactions and runtime performance observed as acceptable |

**Do not convert conditional tests to PASS.** If Gameplay Cameras is not selected, CAM-21 is **NOT APPLICABLE** with the reason. If no menu system exists yet, CAM-12/13 remain **BLOCKED/NOT RUN**, not silently accepted. Record actual Engine build, map, camera packages and source commit with the results.

### Evidence hierarchy

- **Code inspection** confirms relevant methods exist; it does **not** prove compilation.
- **UBT PASS** proves the built C++ target, not input assets or camera rig.
- **Editor asset compile/save** proves a real authored Camera/IMC/Blueprint asset, not visual playability.
- **Human PIE** proves the observed route from the actual controlled pawn at the test location.
- **Cook/package PASS** proves build/cook/packing, not standalone launch.
- **Fresh-process packaged gameplay PASS** is required for shipping camera acceptance.

The original T05 code was extended in the public repository, not in a private game. No local Windows Unreal installation was connected to run these steps during publication.

## 10. Ownership, troubleshooting and production handoff

For manual teams: the game programmer owns the Character's source, the technical artist/level designer owns assigned camera and collision assets, and the human tester runs the actual route.

For AI-driven teams: source implementation can be separate from Blender/world art, but the **sole Unreal Editor owner** integrates the new input actions, saved Character Blueprint, collision responses and any Camera Assets. Keep existing dirty packages intact until their owner resolves them. Never simultaneously edit the same Camera Asset, map or Character source with a second agent.

The implementation/report owner supplies:

1. Exact source/asset paths and reused components.
2. Changes to Input Actions, IMCs, Camera Components, CharacterMovement and collision response.
3. Actual UBT/Editor/PIE/package results, including missing/unrun tests.
4. Collision debug readings and exact obstructing actor/components if a framing defect occurs.
5. Saved Camera Asset and rig/director details **only if actually authored**.
6. Human acceptance or a concrete rejection/fix owner.
7. Remaining scope cuts and performance/accessibility issues.

If the camera retraction persists, keep the failure visible and inspect the actual level geometry and inherited Spring Arm behavior. Do not hide it behind a passing compilation report or a missing asset masked by an “optional” test.

## 11. Next learning step

After camera and player controls are validated in the actual project, proceed to [T09 — UMG/MVVM HUD, menus, settings and loading](README.md). That tutorial will give the existing gameplay state a legitimate user-facing projection, implement actual Options persistence, and test complete input-mode transitions without pretending that a widget compilation validates playable UI.

Related: [T06 contextual interaction](contextual-interactions-pickups-sacrifices.md), [T07 SaveGame/World Partition](savegame-reload-world-partition.md), [The Unmade camera failures](../case-studies/the-unmade/failures-and-fixes.md), and [T02 local build/packaging](unreal-project-bootstrap-local-build.md).
