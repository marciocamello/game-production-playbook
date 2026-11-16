# Unreal Engine 5.9 — GAS Light Donation/Recovery Example

**Source status:** Eight original C++ implementation files are published in [Source/](Source/). They contain real gameplay logic, not empty classes or stubs.

**Verification status:** Source and educational instructions have been reviewed against public GAS/Gameplay Tags/Enhanced Input documentation. **This example has not been compiled, imported, played in PIE, or packaged against an installed Unreal Engine 5.9 by the playbook maintainers.** Integration is a required part of using it.

**Scope:** A single-player, one-map in-session gameplay demonstration. It implements movement, a camera, a GAS-granted Light ability, Enhanced Input, Gameplay Tag blocking, a once-only donation, a world beacon and ability recovery. **It does not implement SaveGame, World Partition streaming reconciliation, multiplayer networking, input remapping UI, narrative inventory or a distinct recovery pickup.** For those, follow the appropriate later tutorials rather than representing this as production-complete game progression.

The source is an **integration example**, not a replacement for an existing project's working GAS classes. If you already have a Character-owned ASC, interaction service or state subsystem, **reuse those systems** and apply only the relevant behavior instead of adding a second authority.

## Exact files provided

| File | Implemented responsibility |
| --- | --- |
| [GPPLightTags.h](Source/GPPLightTags.h), [GPPLightTags.cpp](Source/GPPLightTags.cpp) | Registered native Gameplay Tag `Demo.State.Light.Donated` |
| [GPPLightAbility.h](Source/GPPLightAbility.h), [GPPLightAbility.cpp](Source/GPPLightAbility.cpp) | Real `UGameplayAbility` activation, tag gate, commit, Point Light toggle and `EndAbility` |
| [GPPLightCharacter.h](Source/GPPLightCharacter.h), [GPPLightCharacter.cpp](Source/GPPLightCharacter.cpp) | Character-owned ASC, ability grant, actual input/movement/camera, in-session stage authority, donate/recover transitions |
| [GPPLightBeacon.h](Source/GPPLightBeacon.h), [GPPLightBeacon.cpp](Source/GPPLightBeacon.cpp) | Native placeable world Actor with Engine primitive mesh and real point light responding to committed donation |

The class names start with `GPP` to avoid colliding with common project classes. Header files use `*.generated.h` as their last include, as required by Unreal Header Tool. They deliberately do not prescribe a specific project module name or export macro; when integrating them into a separate module, follow that module's own `<MODULE>_API` convention where external symbol visibility requires it.

## Required target project

- Installed **Unreal Engine 5.9**, real C++ project with a working native game module and a valid Editor target.
- Enabled **GameplayAbilities**, **GameplayTags**, **GameplayTasks**, and **EnhancedInput** plugins/modules as appropriate for the actual project.
- Game module's `.Build.cs` adds dependencies on `Core`, `CoreUObject`, `Engine`, `InputCore`, `GameplayAbilities`, `GameplayTags`, `GameplayTasks`, and `EnhancedInput` (without duplicating existing entries).
- Editor-authored Input Actions / Mapping Context, a saved playable map with walkable collision, a game mode selecting the character, and one actual beacon Actor.
- A sole authorized Editor/MCP operator and a source owner assigned in the game's executable Feature Spec.

### Optional T08 camera and sprint extension

The **same** `AGPPLightCharacter` now exposes two optional Enhanced Input actions and adds real smooth zoom, sprint and camera-collision telemetry without replacing T05's GAS/Light implementation. No new Character or competing camera controller was created.

- Create `IA_GPP_Zoom` (Axis1D), map mouse wheel up/down to positive/negative values; set the Character Blueprint's `ZoomAction` to it. Actual zoom direction depends on the authored input modifiers.
- Create `IA_GPP_Sprint` (Boolean), map Left Shift; set `SprintAction` in the same Character Blueprint.
- The existing Spring Arm uses `ECC_Camera` collision probes and smooth interpolation of requested zoom distance. `IsCameraRetracted()` and `GetCameraRetractionCm()` report actual collision correction separately from the desired boom length.
- Sprint modifies `CharacterMovement.MaxWalkSpeed` only during the active input and resets on release/cancel, possession loss and PawnClientRestart. When menus or cinematics steal input focus, call the existing `ResetSprint()` through a real UI/Controller handoff.
- The original four T05 input assets remain sufficient for T05 alone. T08 behavior is only exercised after its real two additional assets are authored, assigned and tested.

**T08 tutorial:** [Camera rigs, occlusion and Enhanced Input](../../tutorials/camera-rigs-occlusion-enhanced-input.md).

## Integration sequence

1. Inspect the target project. Do not copy these files if it already implements the same ability/state authority.
2. For a project that genuinely needs the example, copy the **eight source files** into that project's existing C++ module's source folder. Keep Unreal Header Tool includes intact and regenerate/compile project files as appropriate.
3. Check `.Build.cs` dependencies and enable the required plugins. Do not create a second runtime module for this example.
4. Build the actual Editor target with your installed UE 5.9; resolve any API/version errors instead of claiming an unrun compilation PASS.
5. In Unreal Editor, author and save these native input assets, with **exactly these bindings**:

   | Asset | Value type | Binding in IMC |
   | --- | --- | --- |
   | `IA_GPP_Move` | Axis2D | W/S for +Y/-Y (Swizzle YXZ; Negate S); A/D for -X/+X (Negate A) |
   | `IA_GPP_ToggleLight` | Boolean | Q |
   | `IA_GPP_Donate` | Boolean | E |
   | `IA_GPP_Recover` | Boolean | R |
   | `IMC_GPP_Demo` | Input Mapping Context | All four Input Actions |

6. Author a **real Blueprint child** `BP_GPP_LightCharacter` of `AGPPLightCharacter`. Assign the mapping context and four actions in its default properties. Compile and save the Blueprint.
7. Open/create a genuinely playable map containing floor collision and a PlayerStart. Place a native `AGPPLightBeacon` (or its compiled Blueprint child) at a reachable location that starts farther than **250 cm** from the character so range rejection can be observed.
8. In the map's World Settings or existing GameMode configuration, select the Blueprint character as **Default Pawn**. Make sure a PlayerController possesses it and the correct map is configured as Game Default Map for eventual packaging.
9. Play in Editor and run the test scenarios below. Compare the character's actual light, beacon light and `Demo.State.Light.Donated` tag; do not rely only on a success string.
10. Execute a separate local Windows packaging and standalone launch check if the game milestone requires it. [T02](../../tutorials/unreal-project-bootstrap-local-build.md) provides build/package wrappers.

The character and beacon use **actual Engine basic-shape meshes** for a deliberately minimal readable lesson and real light components for feedback; they do not promise polished game art.

### Scope-specific behavior

- `WASD` moves the Character relative to the fixed elevated camera's yaw.
- `Q` asks GAS to activate the Light ability; on success its Character light toggles.
- `E` near the beacon, when Light is still **Owned**, commits exactly one **Donated** state change; the Character light turns off, the donation Gameplay Tag is added, and the beacon becomes illuminated.
- `Q` after donation is rejected because the ability's **Activation Blocked Tags** include `Demo.State.Light.Donated`.
- `E` again does not commit another donation.
- `R` near the **same** powered beacon changes Donated to **Recovered**; GAS no longer sees the blocking tag, so `Q` works again. The beacon remains illuminated.
- The tutorial's recovery uses the **same station**, not the separate recovery fragment that the *The Unmade* narrative requires. That distinct collectible/inventory design is a separate feature.
- World progress in this example lives in the current Character instance. A level reload or respawn creates a fresh character, so **no cross-session persistence is claimed**.

## Manual acceptance checklist — record results only after actual execution

| ID | Steps and observed requirement |
| --- | --- |
| L01 | Compile actual module/Editor target and save a real playable map |
| L02 | Start PIE, move in all four directions with the intended camera framing |
| L03 | Press Q: Character light toggles on/off and GAS activation fires |
| L04 | Press E farther than 250 cm from the beacon: no donation; light remains usable |
| L05 | Approach beacon and press E once: Character light off, beacon light on, Stage = Donated, blocking tag present |
| L06 | Press Q while Donated: no personal light activation; ability activation is blocked |
| L07 | Press E again: no duplicate state transition or second reward |
| L08 | Walk away and press R: recovery rejected |
| L09 | Return and press R: Stage = Recovered, blocking tag removed; beacon remains powered |
| L10 | Press Q: personal light toggles again; beacon stays powered |
| L11 | Package the project (if required) and launch newly archived executable outside the Editor; record actual behavior |
| L12 | If saving/streaming is required for a **real game feature**, implement and validate that in its own SDD/T07 pipeline; do not claim it is covered here |

Every check starts **NOT RUN** until a real operator records its outcome. Record any fail/blocker and the revision used. [T05](../../tutorials/gameplay-gas-tags-light.md) defines the full evidence contract.

## Original code license

These source files are original educational code licensed under the [MIT License](../../LICENSE-CODE.md). No private game code, Engine source or binary Unreal assets are distributed.
