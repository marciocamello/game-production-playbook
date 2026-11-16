#Requires -Version 7.0
Set-StrictMode -Version Latest

function Get-UnrealLocalContext {
    [CmdletBinding()]
    param(
        [Parameter(Mandatory = $true)]
        [string]$Project,
        [string]$EngineRoot,
        [ValidateSet('Editor', 'Game')]
        [string]$TargetType = 'Editor',
        [string]$TargetName
    )

    if (-not $IsWindows) {
        throw 'This example supports Windows and PowerShell 7 or later.'
    }

    $projectItem = Get-Item -LiteralPath $Project -ErrorAction Stop
    if ($projectItem.PSIsContainer -or $projectItem.Extension -ine '.uproject') {
        throw '-Project must name an existing .uproject file.'
    }

    $projectFile = $projectItem.FullName
    $projectRoot = $projectItem.DirectoryName
    $projectName = $projectItem.BaseName

    $projectManifest = Get-Content -LiteralPath $projectFile -Raw | ConvertFrom-Json
    if (-not $projectManifest.Modules -or @($projectManifest.Modules).Count -eq 0) {
        throw 'These scripts require an existing C++ Unreal project. For Blueprint-only projects, use Editor packaging.'
    }

    if ([string]::IsNullOrWhiteSpace($TargetName)) {
        $TargetName = if ($TargetType -eq 'Editor') { "$($projectName)Editor" } else { $projectName }
    }
    if ($TargetName -notmatch '^[A-Za-z][A-Za-z0-9_]*$') {
        throw "Unexpected target name '$TargetName'. Pass the exact existing C++ target name."
    }

    $targetFile = Join-Path $projectRoot "Source/$TargetName.Target.cs"
    if (-not (Test-Path -LiteralPath $targetFile -PathType Leaf)) {
        throw "Missing real $TargetType target: Source/$TargetName.Target.cs. Use -TargetName for a nondefault target."
    }

    if ([string]::IsNullOrWhiteSpace($EngineRoot)) {
        $EngineRoot = [Environment]::GetEnvironmentVariable('UE_5_9_ROOT')
    }
    if ([string]::IsNullOrWhiteSpace($EngineRoot)) {
        throw 'Pass -EngineRoot or set UE_5_9_ROOT to the root of the installed Unreal Engine 5.9.'
    }

    $engineItem = Get-Item -LiteralPath ([Environment]::ExpandEnvironmentVariables($EngineRoot)) -ErrorAction Stop
    if (-not $engineItem.PSIsContainer) {
        throw '-EngineRoot must name an existing Unreal Engine installation directory.'
    }

    $enginePath = $engineItem.FullName
    $buildBatchFile = Join-Path $enginePath 'Engine/Build/BatchFiles/Build.bat'
    $uatBatchFile = Join-Path $enginePath 'Engine/Build/BatchFiles/RunUAT.bat'
    $buildVersionFile = Join-Path $enginePath 'Engine/Build/Build.version'

    foreach ($file in @($buildBatchFile, $uatBatchFile, $buildVersionFile)) {
        if (-not (Test-Path -LiteralPath $file -PathType Leaf)) {
            throw "Installed Unreal Engine file is missing: $file"
        }
    }

    $version = Get-Content -LiteralPath $buildVersionFile -Raw | ConvertFrom-Json
    if ([int]$version.MajorVersion -ne 5 -or [int]$version.MinorVersion -ne 9) {
        throw "Expected Unreal Engine 5.9, found $($version.MajorVersion).$($version.MinorVersion). Refusing a mismatched build."
    }

    return [pscustomobject]@{
        ProjectFile    = $projectFile
        ProjectRoot    = $projectRoot
        ProjectName    = $projectName
        TargetName     = $TargetName
        TargetFile     = $targetFile
        TargetType     = $TargetType
        EngineRoot     = $enginePath
        BuildBatchFile = $buildBatchFile
        UatBatchFile   = $uatBatchFile
    }
}

function Invoke-UnrealLocalBatch {
    [CmdletBinding()]
    param(
        [Parameter(Mandatory = $true)]
        [string]$BatchFile,
        [Parameter(Mandatory = $true)]
        [string[]]$Arguments
    )

    if (-not (Test-Path -LiteralPath $BatchFile -PathType Leaf)) {
        throw "Executable Unreal batch file not found: $BatchFile"
    }

    Write-Host ("Running Unreal command: " + [System.IO.Path]::GetFileName($BatchFile))
    & $BatchFile @Arguments
    $code = $LASTEXITCODE

    if ($null -eq $code -or $code -ne 0) {
        throw "Unreal command failed (exit code: $code). Inspect UBT/UAT output before retrying."
    }

    Write-Host 'Unreal command completed with exit code 0. Gameplay/runtime validation is still separate.'
}
