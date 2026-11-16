# Example — Local Unreal Engine 5.9 C++ Build and Windows Package

The scripts in [scripts/](scripts/) are **real build command wrappers**, not stubs or dummy projects. They invoke the installed Unreal Engine 5.9 tools against **an existing C++ Unreal project** supplied by the operator.

**Verification boundary:** Authored from Epic's documented UBT/UAT workflow and the historical production records. **Not executed against a running Windows Unreal 5.9 installation by this public repository.** The exact Engine build, SDK, plugins and game configuration must be verified in the user's environment.

## Requirements

- Windows with PowerShell 7 or later (`pwsh`).
- An installed Unreal Engine 5.9 and matching project association.
- A real `.uproject` with runtime C++ module and actual `Source/<Target>.Target.cs` / `Source/<Target>Editor.Target.cs`.
- A compatible Visual Studio C++ toolchain and Windows SDK.
- Packaging configuration, especially a **Game Default Map** which actually exists.

## Execute against an existing project

From a checkout containing these scripts, configure your own environment variables; their values are **never written into this repository**:

```powershell
# UE_PROJECT_FILE: the existing game .uproject.
# UE_5_9_ROOT: the installed Unreal Engine 5.9 root directory.
$project = (Get-Item -LiteralPath $env:UE_PROJECT_FILE).FullName
$engine = (Get-Item -LiteralPath $env:UE_5_9_ROOT).FullName

pwsh -File ./examples/unreal-local-build/scripts/Build-Editor.ps1 `
    -Project $project -EngineRoot $engine -Configuration Development

pwsh -File ./examples/unreal-local-build/scripts/Package-Windows.ps1 `
    -Project $project -EngineRoot $engine -Configuration Development
```

The scripts run against the specified real `.uproject`; they do **not** require the game project to be stored inside this playbook repository. Use `-TargetName` when the existing C++ target differs from the project filename. The default Editor target adds `Editor` to that filename.

## Safeguards

- Fail on unsupported OS/PowerShell, missing project, missing C++ target, missing Engine tools and wrong Engine version.
- Invoke `Build.bat` for an actual Editor target and `RunUAT.bat BuildCookRun` for Win64.
- Keep archives outside the Engine and project authored/generated directories.
- Generate unique archive destinations; never delete or overwrite another build.
- Report nonzero build process exit codes and require the expected game executable in the archive.
- Do **not** claim PIE, human gameplay, fresh packaged launch or public release success.
- Create **no GitHub Actions** or hosted workflows.

By default, archives are written into **`LocalBuilds/` under the target game's project directory**. Add `LocalBuilds/` to that game's `.gitignore`. This is **not** an instruction to add the game into the playbook repository.

## Verification record

| Stage | Expected evidence | What it cannot establish |
| --- | --- | --- |
| Preflight | Existing `.uproject`, target and matching UE 5.9 | Compilation |
| Editor build | UBT command result and compile log | Blueprint behavior or PIE |
| Packaging | UAT build/cook/stage/package result, executable in new archive | Actual executable launch |
| Human standalone | Fresh process opens correct map, controls work and exits safely | Other hardware or full route |
| Human game acceptance | Entire intended route, Save/Load where applicable and meaningful interaction | Untested conditions |

See [T02 — Project Bootstrap & Local Build](../../tutorials/unreal-project-bootstrap-local-build.md) for the complete exercise, troubleshooting and acceptance checks.

## Licensing

These original educational scripts are specifically licensed under [MIT](../../LICENSE-CODE.md); Unreal Engine and any game project they invoke are governed by their own licenses.
