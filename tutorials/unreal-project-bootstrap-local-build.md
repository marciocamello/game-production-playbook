# T02 — Unreal Engine 5.9 Project Bootstrap and Local Build Pipeline

**Tutorial status:** Published technical instructions with reusable PowerShell build/packaging scripts. **Not tested against a live Windows Unreal Engine installation by the playbook maintainers.** A reader must execute the checks against an authorized real game project and record the results.

**Reference environment:** Unreal Engine **5.9**, Windows, PowerShell 7+, C++ + Blueprints. The 5.9 release was documented as **Preview** in Epic's November 2026 release notes. Toolchain, platform and plugin compatibility must be checked against the actual installed 5.9 build.

**Practical objective:** start from a real C++ Unreal project, confirm its authoritative modules and targets, set the correct startup map, compile an **Editor** target with Unreal Build Tool (UBT), cook and package a **Windows game** with Unreal Automation Tool (UAT), locate the executable, and test its actual startup outside the Editor. Never confuse these gates.

## 1. Authoritative references and scope

Official Epic documentation consulted on November 16, 2026:

- [Packaging Unreal Engine Projects](https://dev.epicgames.com/documentation/unreal-engine/packaging-your-project) — the build/cook/stage/package process, Windows tutorial, startup map and Development/Shipping behavior.
- [Build Operations: Cooking, Packaging, Deploying and Running](https://dev.epicgames.com/documentation/unreal-engine/build-operations-cooking-packaging-deploying-and-running-projects-in-unreal-engine) — UAT, `BuildCookRun` and platform operations.
- [Unreal Engine Build Tool Target Reference](https://dev.epicgames.com/documentation/unreal-engine/unreal-engine-build-tool-target-reference) — targets and `.Target.cs`.
- [Setting Up a C++ Development Environment](https://dev.epicgames.com/documentation/unreal-engine/setting-up-your-development-environment-for-cplusplus-in-unreal-engine) — IDE, code generation and C++ workflow.
- [Visual Studio Development Environment for Unreal Engine](https://dev.epicgames.com/documentation/unreal-engine/setting-up-visual-studio-development-environment-for-cplusplus-projects-in-unreal-engine) — current compatible IDE, MSVC and Windows SDK requirements.
- [Unreal Engine 5.9 Preview Release Notes](https://dev.epicgames.com/documentation/unreal-engine/unreal-engine-5-9-release-notes?lang=en-US) — release-specific caveats.

The automation example lives at [`examples/unreal-local-build/`](../examples/unreal-local-build/README.md). These scripts do not create dummy projects, implement fictional gameplay, modify third-party Engine source, or generate GitHub Actions.

### What this tutorial does and does not prove

| Gate | Executed operation | Valid claim after observed success |
| --- | --- | --- |
| Bootstrap | Real game project, modules, target files, map and required toolchain | Project is configured for an attempted build |
| Editor build | `Build.bat` invokes UBT for an existing Editor target | Editor target compiled and linked under the actual environment |
| Editor check | Open game, compile/save real assets, manually play PIE | Stated Editor checks and player behavior passed |
| Package | `RunUAT.bat BuildCookRun` for Win64 with build/cook/stage/package/archive | A package was produced with a game executable |
| Standalone launch | Open the archived executable as a separate process | Game started and the selected startup map behaved as observed |
| Acceptance | Complete actual player route, state changes, persistence and return/exit | The specific tested gameplay requirements were accepted |

Every later gate needs its own evidence. A zero exit code from UAT is **not** proof of an executable launch or a successful game.

## 2. Preflight: operating system, Engine and ownership

Use **an already authorized game repository**. An instructional repository can document the process but must not edit an unrelated project by default.

1. Confirm Windows and install or select PowerShell **7+**.
2. Install Unreal Engine **5.9** (the intended project version) and launch the Editor at least once.
3. Use a supported Visual Studio environment with **Game development with C++**, the compatible MSVC compiler, and the Windows SDK required for the installed UE build. Requirements can change with the 5.9 preview; verify them in the official version-specific support table and installed toolchain, rather than guessing one fixed MSVC version.
4. Ensure adequate writable storage for derived data, shaders, compilation and packaged builds. Verify that Defender or security tooling is not silently quarantining generated binaries.
5. Record the selected branch/revision, changed source files, dirty Unreal packages, current Editor owner and target build configuration.
6. **Do not** reset the working tree, delete `Intermediate/` or `Saved/`, discard unsaved packages, or overwrite another developer's build just to obtain a clean screenshot.

A source-engine checkout may be useful for **read-only API research** where licensed, but it must not be substituted for the installed Engine whose build tools will actually compile and package the game.

### Minimal real repository structure

The directory tree below is an **illustrative contract**, not a project shipped inside this playbook. A newly created C++ project should contain its own real generated module and target files; use that project's actual names.

```text
<game-repository>/
  <game-project>/
    <GameName>.uproject
    Config/
      DefaultEngine.ini
      DefaultGame.ini
      DefaultInput.ini
    Content/
      ...
    Source/
      <GameName>.Target.cs
      <GameName>Editor.Target.cs
      <GameName>/
        <GameName>.Build.cs
        Public/
        Private/
  docs/
  specs/
  scripts/
```

**Do not manufacture the illustrated files as empty placeholders.** Use the Unreal Editor's C++ project/class creation flow and preserve the actual module and target names it creates. A Blueprint-only project will not have C++ source targets by default and should follow the Editor packaging route below.

## 3. Create or inspect a real C++ / Blueprint project

For a new project:

1. Launch the installed Unreal Engine **5.9** Editor and choose **Games → Blank** (or another genuinely suitable gameplay template).
2. Select **C++** when the project requires native gameplay. Choose the actual Windows/Desktop target and quality settings appropriate for your game. Decide whether starter content is needed; generated extras are not automatically production assets.
3. Name and create the game project using an agreed source-controlled location. Record which engine version actually created it.
4. Open the project, select a real playable level, save it under the project's Content hierarchy and establish the intended **Game Default Map**.
5. Verify project settings and source files **before** adding plugins or reimplementing framework classes.

For an existing project, skip creation and inspect its current `.uproject`, target files, source modules, maps, settings and current branch. **Reuse what exists.**

### The `.uproject`, target and module relationship

The `.uproject` identifies the project, registered runtime modules and enabled plugins. Native C++ projects normally include:

- `Source/<GameName>.Target.cs` — **Game target** for a standalone cooked build.
- `Source/<GameName>Editor.Target.cs` — **Editor target** for development in Unreal Editor.
- `Source/<GameName>/<GameName>.Build.cs` — C++ module dependencies, include paths and build rules.

The filenames may differ if a project intentionally uses different target names. Read the actual code first. A target is **not** the same thing as the runtime module; a new gameplay component usually belongs in an existing module instead of another duplicate module.

`TargetType.Game` and `TargetType.Editor` are part of UBT's documented target model. A native gameplay module generally depends on `Core`, `CoreUObject`, `Engine` and any functionality it actually imports. Add dependencies such as `EnhancedInput`, `GameplayAbilities`, `GameplayTags`, `GameplayTasks`, `UMG` or `ModelViewViewModel` **only when the game implements corresponding functionality**. A plugin checkbox alone is not an alternative to compatible C++ module rules and real runtime integration.

### Unreal project files are not the gameplay

Generating IDE files is not compilation. Compiling a C++ Editor target is not authoring a Blueprint. Compiling a Blueprint is not proof that its Actor is in the correct level or that its interaction is reachable. Keep each check separate.

## 4. Author the actual startup map and play mode

Epic's packaging tutorial explicitly requires the **Game Default Map**: without it, a packaged game may open to a black or empty screen.

1. In the Unreal Editor, open **Edit → Project Settings → Maps & Modes**.
2. Choose a saved, authorized level as **Game Default Map**. Verify the map is part of the intended release and can load without Editor-only dependencies.
3. Check the GameMode, PlayerController, pawn/character, world partition settings and any map-specific overrides as needed for real gameplay.
4. Under **Project Settings → Packaging**, inspect Build Configuration, target platform, map/content cooking scope, prerequisites and required non-asset files.
5. Run **Play In Editor** manually and confirm the player sees and can interact with the intended starting level. Save only assets currently owned by the operator.

The real Engine may have a Game Default Map inherited from a template. Verify its asset, not just the presence of a configuration value.

### Content cooking matters

Cooked assets are based on references and configured cook scopes. A mesh or map visible in the Editor's Content Browser is **not guaranteed to be present in the packaged game**. Check maps explicitly included, hard and soft references, loading by asset path, plugins with runtime content, and localization or other required files.

Do not solve missing cooked dependencies by automatically copying the entire Content folder into a release. Diagnose references and configuration first.

## 5. Prepare local PowerShell build execution

The [reusable scripts](../examples/unreal-local-build/README.md) operate on **a real existing C++ project**, regardless of its checkout location. They receive the Engine and project paths through **runtime arguments/environment**, so this public repository contains no hardcoded personal directories.

The scripts are:

| File | Real purpose |
| --- | --- |
| [`Unreal-Local.ps1`](../examples/unreal-local-build/scripts/Unreal-Local.ps1) | Validate OS, project, C++ targets, actual UE 5.9 version and command paths |
| [`Build-Editor.ps1`](../examples/unreal-local-build/scripts/Build-Editor.ps1) | Invoke UBT through the Engine's `Build.bat` for the Editor target |
| [`Package-Windows.ps1`](../examples/unreal-local-build/scripts/Package-Windows.ps1) | Invoke UAT `BuildCookRun` for a fresh Win64 archive and locate the game executable |

Before executing, set the two variables **in your local shell** to your actual project and Engine. Their values are not stored in this repository:

```powershell
# Required caller-provided values:
# $env:UE_PROJECT_FILE — existing .uproject
# $env:UE_5_9_ROOT   — root of the installed Unreal Engine 5.9

$project = (Get-Item -LiteralPath $env:UE_PROJECT_FILE).FullName
$engine = (Get-Item -LiteralPath $env:UE_5_9_ROOT).FullName

pwsh -File ./examples/unreal-local-build/scripts/Build-Editor.ps1 `
  -Project $project -EngineRoot $engine -Configuration Development
```

`Get-Item` fails when the requested input is missing. This is deliberate: **do not fall back silently to a different game or Engine installation**.

The script checks actual `Source/<Target>Editor.Target.cs`, the Engine's `Engine/Build/Build.version` and the presence of `Engine/Build/BatchFiles/Build.bat` and `RunUAT.bat`. The installed Engine version must report major 5, minor 9. A mismatched preview/build, project association or toolchain requires review; the major/minor check alone cannot prove full binary/API compatibility.

For a custom existing target, use `-TargetName` with the real Editor target name.

### What happens during the Editor build

The wrapper invokes the equivalent of:

```text
Build.bat <ExistingEditorTarget> Win64 Development -Project=<ExistingGame.uproject> -WaitMutex -NoHotReloadFromIDE
```

The batch file and arguments are passed as **separate PowerShell arguments**, preserving paths containing spaces. This is a real UBT invocation, not an imagined compiler command.

**Acceptance after this step:** inspect the actual UBT output, error/warning records, module compilation and exit result. If the command exits nonzero, the wrapper throws. If it exits zero, record **Editor build PASS** but leave gameplay/Editor asset/packaged tests **NOT RUN** until executed.

## 6. Run a first real Win64 package

Once the game compiles, the startup map is configured and the Editor-owned assets are saved, run an early **Development** package. It reveals missing cook references, staging errors, configuration issues and external executable problems sooner than a last-minute Shipping build.

```powershell
pwsh -File ./examples/unreal-local-build/scripts/Package-Windows.ps1 `
  -Project $project -EngineRoot $engine -Configuration Development
```

To produce a separate release-candidate archive later:

```powershell
pwsh -File ./examples/unreal-local-build/scripts/Package-Windows.ps1 `
  -Project $project -EngineRoot $engine -Configuration Shipping
```

### What the packaging wrapper executes

It invokes the documented UAT pipeline:

```text
RunUAT.bat BuildCookRun
  -project=<ExistingGame.uproject>
  -noP4
  -platform=Win64
  -clientconfig=Development|Shipping
  -build -cook -stage -pak -package -archive
  -archivedirectory=<UniqueOutputDirectory>
  -unattended -utf8output
```

This project is a **local-first workflow**. No hosted CI, GitHub Actions, push, download or release action is involved.

The script defaults to a **unique directory under the target project's `LocalBuilds/`** and refuses overwrites. It prevents archive roots inside Engine/source/content or generated Unreal directories. It checks for a game-named `.exe` after a successful UAT run and reports where the archive was written.

Add `LocalBuilds/` to the actual game's own `.gitignore`. Do not add binaries or staged content to this playbook.

### Packaged output is not a game test

A package may exist even if:

- The startup level is wrong, blank or incomplete.
- A soft-referenced Blueprint or material was not cooked.
- Required runtime plugins, data files, fonts or media are unavailable.
- Input mappings are missing, UI is nonfunctional or graphics initialization fails.
- An unsaved Editor asset differs from the cooked disk version.
- The executable opens and immediately crashes.

Therefore the **package gate stops at UAT and file presence**. The next operation is a separate **human standalone verification**.

## 7. Launch and test the packaged game outside the Editor

The person authorized to test the game performs this on the archive **actually produced** by the script, not on another old build:

1. Locate the reported packaged executable and check its file timestamp and target.
2. Exit or separate the relevant Unreal Editor session as appropriate; launch the game executable directly.
3. Confirm the real startup map loads, the viewport renders, audio device initializes and the game responds to intended mouse/keyboard/controller inputs.
4. Use an actual interaction or objective, including any persistent state change already present in that game.
5. Exit by the game's supported UI mechanism. Epic notes that a Shipping build normally has no developer console; **an actual Exit/Quit action must work**.
6. Launch again as a fresh process and check New Game, Continue and persistence where implemented.
7. Read the **packaged game's** logs and record errors, actual results and defects. Do not treat the absence of a visible crash as complete functional acceptance.

For an early project with little gameplay, validate the **actual existing behavior only**. Do not claim that unfinished abilities, inventory or save systems passed because the executable started.

### First game build versus final Shipping build

| Development archive | Shipping archive |
| --- | --- |
| Optimized for iteration and debugging | Optimized for distribution |
| May expose developer console/diagnostics | Restricts developer console/tools |
| Useful for early packaging defects | Necessary final release candidate when platform requires it |
| Does not prove final release | Still needs a real full-route gameplay test |

Both should be treated as distinct executable revisions if their contents differ.

## 8. Blueprint-only and Editor UI alternative

The included scripts deliberately target **C++ projects with real target files**. They reject a Blueprint-only game rather than creating empty C++ files or pretending the target exists.

For a Blueprint-only project, use the official Editor workflow:

1. Configure **Game Default Map** and the intended platform/settings.
2. Open the **Platforms** menu, choose **Windows** and the appropriate build configuration.
3. Select **Package Project** and provide an authorized output directory.
4. Inspect the **Output Log** and **Message Log → Packaging Results**, correcting actual errors.
5. Launch the newly packaged `.exe` and manually test the same startup and player behavior.

This option is equally valid for C++ projects when the operator wants direct Editor control, provided results are documented.

## 9. Common failures and non-destructive fixes

| Symptom | Specific inspection | Safe next action |
| --- | --- | --- |
| `-Project` points to nothing | Existing `.uproject`, shell variable and current working directory | Supply actual file; do not create a dummy project |
| No `.Target.cs` | Blueprint-only vs native project; custom target names | Use Editor packaging for Blueprint-only or the real `-TargetName` |
| Engine mismatch | `Build.version`, actual Editor title, project association | Select the installed 5.9 Engine matching the game |
| MSVC/Windows SDK error | UBT compiler error and Epic's current requirement table | Install/repair the appropriate toolchain; rerun the **same** source |
| C++ module/dependency compile error | Module `.Build.cs`, plugin enabled state and offending header | Fix actual dependency or source and rebuild; do not duplicate systems |
| Live Coding / build conflict | Running Editor, hot reload state and open build processes | Stop conflicting compilation safely; preserve unsaved Editor assets |
| Missing cooked map or asset | Packaging log, Game Default Map, references and map inclusion | Fix content reference/cook scope and repackage |
| UAT returns nonzero | Full UAT/UBT log and first meaningful failure | Address root cause; never relabel failed packaging as PASS |
| Archive exists but named game `.exe` missing | Game target name, archive layout and UAT output | Validate the true target and output; do not claim a valid package |
| Executable launches to black screen | Default map, GameMode, cooked assets and runtime log | Correct the real configuration and create a new package |
| Packaged executable crashes | Runtime logs, prerequisites, drivers and asset/plugin dependencies | Reproduce and fix; a successful Editor build does not override runtime failure |
| Packaging changes overwrite prior files | Archive destination ownership and generation scheme | Use unique archives; retain diagnostic output and previous releases |

Never “fix” a broken build by deleting another operator's source or unsaved native packages. Inspect ownership and evidence first.

## 10. Actual verification report

Use [the testing template](../templates/test-plan.md) and record:

- Game project/revision and **installed** Engine build/preview identity.
- PowerShell, Visual Studio/MSVC and Windows SDK versions if relevant to a failure.
- The real `.uproject` and target identities (public-safe, relative names only).
- Commands and actual exit codes for UBT and UAT.
- Individual warnings, error roots, and the archived game executable located.
- Editor/asset save state and the actual Game Default Map.
- Human standalone launch: fresh-process startup, controls, one real interaction and supported exit.
- Actual `PASS`, `FAIL`, `BLOCKED` or `NOT RUN` for each gate, with tester/date.
- Known differences between Development and Shipping, plus outstanding work.

**No sample PASS evidence is fabricated here.** This playbook publishes the tooling and steps, not an assertion that a particular reader's game has compiled, launched or shipped.

## 11. Ownership and work model

**Manual:** a developer creates/inspects the real project, builds it and performs gameplay acceptance.

**AI-assisted:** an AI may review UBT errors, propose a module rule correction or draft a test plan, while the human authorizes each change and executes the real build.

**AI-driven:** an assigned agent with local computer/build-tool access may invoke the scripts and report outputs, but only the authorized file owner may change source, and the sole Editor operator handles `.uasset`/`.umap` changes. Game Director/human playtest and actual package checks remain explicit gates. No model may claim tests it did not perform.

For the related Editor automation setup, see [T01 — Unreal Engine 5.9 MCP](unreal-mcp-5-9.md). Next in the series: **T03 — From Concept to GDD, TDD and Executable SDD**, which defines the actual feature the local build pipeline should deliver.
