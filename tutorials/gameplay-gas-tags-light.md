# T05 — Gameplay C++, Gameplay Tags and GAS: A Real Light Ability

**Tutorial status:** Published with eight original C++ source files implementing an observable GAS-powered ability and in-session gameplay-state changes. **The example has not been compiled or played against Unreal Engine 5.9 in this public repository.** Any actual local test remains NOT RUN until performed and evidenced.

**Engine:** Unreal Engine 5.9 C++ / Blueprints, Windows PC, standalone single-player. Existing systems in a reader's game always take priority over replacing them with this example.

**Goal:** deliver a genuine **press → GAS activation → visible world effect → state change → ability blocked → recovery** path using native Gameplay Tags and Enhanced Input. Learn the responsibilities of `UAbilitySystemComponent`, `UGameplayAbility`, the player Character and a placeable world beacon. T06 will extend the interaction contract to proper target focus, pickup identity and voluntary hold confirmation; T07 will extend it to SaveGame and streaming lifecycle.

The complete educational source and integration procedure are in [examples/gas-light-demo](../examples/gas-light-demo/README.md).

## 1. Authoritative API references

Reviewed for this tutorial on November 16, 2026:

- [Epic — Using Gameplay Abilities](https://dev.epicgames.com/documentation/unreal-engine/using-gameplay-abilities-in-unreal-engine?application_version=5.9)
- [Epic — Gameplay Ability System overview](https://dev.epicgames.com/documentation/unreal-engine/understanding-the-unreal-engine-gameplay-ability-system?application_version=5.9)
- [Epic — Native Gameplay Tags](https://dev.epicgames.com/documentation/unreal-engine/using-gameplay-tags-in-unreal-engine?application_version=5.9)
- [Epic — Enhanced Input](https://dev.epicgames.com/documentation/unreal-engine/enhanced-input-in-unreal-engine?application_version=5.9)
- [Epic API — GiveAbility](https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Plugins/GameplayAbilities/UAbilitySystemComponent/GiveAbility)
- [Epic API — TryActivateAbilityByClass](https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Plugins/GameplayAbilities/UAbilitySystemComponent/TryActivateAbilityByClass)
- [Epic — Ability Granting and Removal](https://dev.epicgames.com/documentation/unreal-engine/using-gameplay-abilities-in-unreal-engine)

**API caution:** UE 5.9 installations may differ in preview/build and plugin versions. Inspect the installed `GameplayAbility.h`, `AbilitySystemComponent.h`, `NativeGameplayTags.h` and Enhanced Input headers before adapting the source. Documentation references describe supported patterns; they do not substitute for a successful compile.

## 2. Authority model: why four responsibilities matter

| Runtime component | Owns | Must not own |
| --- | --- | --- |
| Character | Actual player movement, physical light component, **one in-session authoritative progression enum**, input requests | Permanent asset data copied from a level |
| Ability System Component (ASC) | Granted ability specs, Gameplay Tags and ability activation checks | A second competing progression enum |
| Gameplay Ability | Checks activation eligibility, commits activation, turns the character's Point Light on/off, ends execution | SaveGame truth or player inventory |
| Beacon Actor | Real world mesh and Point Light **projecting** the committed donation state | Independent decision that a donation or recovery occurred |

The character is authoritative **only for this narrow, in-session standalone tutorial**. Real projects with an existing persistent progression subsystem must reuse it instead. A replicated multiplayer game requires server RPCs, possession-aware ASC initialization and client/state replication, which this single-player sample intentionally does not implement.

### The intended observable flow

```text
Character spawns (Light = Owned, beacon off)
       |
Press Q -> ASC TryActivateAbilityByClass -> UGameplayAbility Commit
       |                                         |
       +----> Character Point Light toggles <---+
       |
Press E near beacon -> authority checks stage and range
       |
Light = Donated -> Add loose gameplay tag -> personal light off
       |
Beacon Point Light becomes visible
       |
Press Q -> GAS ActivationBlockedTags rejects light
       |
Press R at the powered beacon -> Light = Recovered
       |
Remove blocking tag -> Q works again, beacon remains on
```

The example's `E` and `R` are deliberately **immediate, proximity-based actions** for demonstrating GAS and state authority. They are **not** a production-quality interaction system with line-of-sight, hold-to-confirm, physical mechanism parts or recovery pickups. Those belong in T06. Do not represent the demo as the complete *The Unmade* fountain feature.

## 3. Read the real source, not invented pseudocode

| Component | Files | Real implementation |
| --- | --- | --- |
| Native tag | [GPPLightTags.h](../examples/gas-light-demo/Source/GPPLightTags.h), [GPPLightTags.cpp](../examples/gas-light-demo/Source/GPPLightTags.cpp) | `UE_DECLARE_GAMEPLAY_TAG_EXTERN` + `UE_DEFINE_GAMEPLAY_TAG_COMMENT` register `Demo.State.Light.Donated` |
| GAS ability | [GPPLightAbility.h](../examples/gas-light-demo/Source/GPPLightAbility.h), [GPPLightAbility.cpp](../examples/gas-light-demo/Source/GPPLightAbility.cpp) | An instanced `UGameplayAbility`, blocked by donation tag, calling `CommitAbility` and `EndAbility` |
| Player Character | [GPPLightCharacter.h](../examples/gas-light-demo/Source/GPPLightCharacter.h), [GPPLightCharacter.cpp](../examples/gas-light-demo/Source/GPPLightCharacter.cpp) | C++ Character + ASC + native camera + 4 Enhanced Input actions + state ownership + donation and recovery |
| World Actor | [GPPLightBeacon.h](../examples/gas-light-demo/Source/GPPLightBeacon.h), [GPPLightBeacon.cpp](../examples/gas-light-demo/Source/GPPLightBeacon.cpp) | Placeable cylinder marker and visible warm-world Point Light |

All eight source files are **new educational examples** and do not duplicate/relicense the private source of any game. Copy them only into a project where that behavior is not already implemented.

### Native tag versus a plain boolean

`EGPPLightStage` is the authoritative rule: Owned, Donated or Recovered. The ASC's `Demo.State.Light.Donated` tag is **derived**, not a second source of progression truth. GAS checks this tag through `ActivationBlockedTags`. The Character's `ApplyLightStage` adds or removes the tag only when its count/state needs to change, and extinguishes the personal light on donation. There is no unrelated UI boolean pretending to represent ability availability.

### Ability lifecycle

The Character owns the ASC, calls `InitAbilityActorInfo(this, this)` and grants the ability via `GiveAbility(FGameplayAbilitySpec(...))` once on the authoritative actor's first BeginPlay for this tutorial. On Q, Enhanced Input requests activation with `TryActivateAbilityByClass`. The GAS ability checks its blocked tag, commits and toggles a real Point Light, then finishes via `EndAbility`.

A return value from `TryActivateAbilityByClass` is **not a guarantee that all downstream actions completed**. Observe actual lighting and activation eligibility; do not label a log line a gameplay PASS.

**No invented cooldown/cost system:** there is no numeric ability cost here, so there is no fake AttributeSet or GameplayEffect merely to demonstrate every GAS class. Add an actual GameplayEffect only when a required gameplay cost or modifier exists.

## 4. Integrate in an existing Unreal 5.9 C++ project

Preflight: inspect the game's source/Blueprints first, check `.uproject` plugin state and module dependencies, identify the current Editor owner and preserve dirty/untracked assets. If this exact gameplay already exists, **do not add the demonstration classes**; inspect it and use the same acceptance requirements.

The example is **source for an existing native module**, not a standalone runnable project. It deliberately does not create an arbitrary empty `.uproject`, duplicate runtime module, fake Blueprint asset or fabricated map.

### Step A — Add real native source

Place the eight files from [the example source](../examples/gas-light-demo/Source/GPPLightCharacter.cpp) into an authorized target game's existing source module and regenerate project files if necessary. Keep the four Unreal Header Tool `.generated.h` includes as the final include in their respective headers.

Add **only missing** dependencies in the existing module's `.Build.cs`: `Core`, `CoreUObject`, `Engine`, `InputCore`, `GameplayAbilities`, `GameplayTags`, `GameplayTasks` and `EnhancedInput`. Enable the corresponding GameplayAbilities/EnhancedInput plugins as required by the actual game. Do not copy another project's `.Build.cs` wholesale or create duplicate classes.

Build the real Editor target using the project's existing local build scripts, or follow the [T02 build tutorial](unreal-project-bootstrap-local-build.md). Record the actual results; this repo has **not** run that compilation.

### Step B — Author actual input assets in the Editor

The Editor operator creates and saves five real assets:

| Asset | Type | Input mapping |
| --- | --- | --- |
| `IA_GPP_Move` | Input Action, Axis2D | W (+Y), S (-Y), A (-X), D (+X), with appropriate Swizzle/Negate modifiers |
| `IA_GPP_ToggleLight` | Input Action, Boolean | Q |
| `IA_GPP_Donate` | Input Action, Boolean | E |
| `IA_GPP_Recover` | Input Action, Boolean | R |
| `IMC_GPP_Demo` | Input Mapping Context | Contains the four actions and mappings |

W/S use `Swizzle Input Axis Values` (e.g. YXZ to map the key's 1D value into Y); S and A additionally use `Negate`. Choose consistent controller/gamepad mappings if needed by the game's real target. The example already calls `RemoveMappingContext` and `AddMappingContext` on its own context when the pawn restarts; it **never calls `ClearAllMappings`**, because that would remove other systems' mappings.

### Step C — Integrate the native classes and gameplay map

Create a real Blueprint child of `AGPPLightCharacter`. Assign all four Input Actions and the Mapping Context to its exposed properties. Compile and **save**. In a real authorized level:

1. Author a floor with proper collision and a PlayerStart.
2. Place one `AGPPLightBeacon`; its native code uses an actual built-in Engine cylinder mesh and world Point Light.
3. Use an existing GameMode, map override or project GameMode to select your new Blueprint Character as the Default Pawn; do not create another GameMode if an existing one works.
4. Configure the Game Default Map to this saved playable level if using standalone packaging.
5. Confirm the correct character is possessed by the real PlayerController, the camera is elevated and input works.
6. Save only owned content packages; keep unknown dirty assets unchanged.

**Do not edit `.uasset` or `.umap` bytes through a filesystem script.** Use the Unreal Editor or an authorized, schema-verified MCP tool as explained in [T01](unreal-mcp-5-9.md).

## 5. Execute the actual ability-and-donation test

| ID | Action | Expected real observation |
| --- | --- | --- |
| G01 | Start PIE in the authorized level | Character is controllable from the elevated camera; beacon initially dark |
| G02 | Press WASD | Character moves relative to camera without losing input |
| G03 | Press Q | Personal blue/cyan Point Light toggles on/off |
| G04 | Press E while farther than 250 cm from beacon | Donation is rejected; personal Light remains available |
| G05 | Move within 250 cm, press E once | LightStage becomes Donated; personal light turns off; beacon light turns on |
| G06 | Press Q after donation | GAS rejects activation due to donation tag; personal Light stays off |
| G07 | Press E repeatedly | No duplicate donation, extra beacon state or progress change |
| G08 | Move beyond 250 cm, press R | Recovery rejected, tag still blocks Q |
| G09 | Return to original beacon, press R | LightStage becomes Recovered; beacon remains on; blocking tag removed |
| G10 | Press Q again | Personal Light works; beacon remains powered |
| G11 | Reload map / restart process | **Expected reset in this T05-only sample**; persistence is not implemented |
| G12 | Cook/package/launch if required | Separate build and runtime results; do not infer them from C++ compilation |

Read the actual character Stage and ASC tag in Editor diagnostics, observe component visibility, and use the runtime logs to explain failures. Do **not** treat `TryActivateAbilityByClass` returning true as proof that every action was visible in the world.

Every test begins **NOT RUN** for this repository. After a real run, mark cases PASS, FAIL, BLOCKED or NOT RUN individually, with actor/asset revision, Editor version and logs.

## 6. Failure, restart and state ownership

This example intentionally keeps all progression on a single Character during one play session. It does not serialize `EGPPLightStage`, store stable beacon IDs or guarantee reapplication after World Partition stream-out. Consequently, **it must not be integrated unchanged into a game whose approved SDD requires persistent restoration**.

For a real production feature, the Gameplay Tag should be **derived from** a durable state authority (such as an existing GameInstance subsystem), the Beacon should **reapply its world visual state** when streamed in, and the SaveGame should serialize logical states rather than raw ability handles or pointers. The next gameplay tutorial (T06) covers real interaction and pickups; T07 covers save/load and streaming.

### Edge-case table

| Failure / edge case | Implemented in the example? | Further required work |
| --- | --- | --- |
| Tag blocks Q after donation | Yes, via GAS and derived loose tag | Verify in the actual installed Engine |
| Repeated donation without double credit | Yes, `Owned` stage gate | Real PIE test |
| Recovery while too far away | Yes, same-beacon range gate | Real PIE test |
| Beacon remains powered after recovery | Yes, no reversal of beacon light | Real PIE test |
| Valid interaction line-of-sight | **No** | T06 implementation |
| Hold-to-confirm/cancellation | **No** | T06 implementation |
| Separate physical mechanism/recovery pickups | **No** | T06 implementation |
| Durable restoration after process restart | **No** | T07 implementation |
| Streaming Actor re-registration | **No** | T07 implementation |
| Multiplayer replication/prediction | **No** | A separately approved network feature |
| User settings/input rebinding UI | **No** | T09 |

These omissions are **explicit scope boundaries** of the example, not partially implemented functions or fake feature claims.

## 7. Troubleshooting and corrective work

| Symptom | Inspect and correct |
| --- | --- |
| `GameplayAbility.h` / `AbilitySystemInterface.h` not found | Actual module dependencies, enabled GameplayAbilities plugin, build target |
| Native Gameplay Tag not registered | Include `NativeGameplayTags.h`, compile tag `.cpp`, check Engine registry |
| Input actions never fire | Confirm real IMC/IA assets assigned, Boolean/Axis2D types, Enhanced Input Component and PlayerController possession |
| Character does not move | Check floor collision, pawn possession, movement input axis modifiers, default GameMode |
| Q does nothing even before donation | ASC ActorInfo, granted ability in BeginPlay, real Character, input binding and activation logs |
| Q works after donation | Gameplay Tag count and `ActivationBlockedTags`, Stage reconciliation and wrong actor instance |
| E does nothing close to beacon | Actual 3D Actor distance, placed beacon class, PlayerController, stage |
| R returns light without keeping beacon powered | World Actor state projection or duplicate state systems; compare actual live Actor |
| Scene works in PIE but not in packaged game | Saved default map, cooked Blueprint/IMC references, plugins and fresh-process behavior |
| Compile passes but scene has no visual feedback | Built-in mesh availability, light visibility/intensity, lighting/exposure and real map integration |

A failure must result in a **real correction by its assigned owner**, followed by the relevant tests. Do not substitute stub functions, fake scene screenshots or untested API assertions.

## 8. Multi-agent execution and evidence gate

- **Game Director:** approves gameplay behavior, real game applicability, manual PIE and final acceptance.
- **Code owner:** implements or reuses ASC/Ability/Tag logic; builds and reports actual native tests and source.
- **Sole Editor/MCP operator:** creates the actual input assets, Character Blueprint, map placements and settings; compiles/saves/checks native assets.
- **Independent reviewer:** audits source and reports without overwriting the code owner's files.
- **Release owner:** performs local cook/package and standalone launch checks when required.
- **Documentation owner:** reconciles factual results into the canonical GDD/TDD/spec and development log.

An implementation **is not Human Validated** until a named reviewer plays the actual game behavior and accepts the real outcome. The source example is included as original MIT-licensed instructional code, not a mirrored original game feature.

## 9. Next

- [T06 — Contextual interaction, pickup identity and irreversible choices](README.md) will replace immediate station inputs with proper target selection, cancellation, once-only collectibles and physical world consequences.
- [T07 — SaveGame and World Partition](README.md) will introduce durable logical authority, unique IDs, actor registration and fresh-process replay checks.
- [T02 — Local Build and Packaging](unreal-project-bootstrap-local-build.md) supplies real local UBT/UAT invocation.
- [The Unmade design and GAS decisions](../case-studies/the-unmade/decisions.md) show how these concepts informed an approved narrative system, with evidence boundaries.
