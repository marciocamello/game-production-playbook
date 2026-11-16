#Requires -Version 7.0
<#
.SYNOPSIS
Builds, cooks, stages, packages and archives an existing Unreal 5.9 C++ game for Win64.
.DESCRIPTION
Creates a unique output directory under the project's LocalBuilds folder by default.
Never overwrites an existing archive and never claims successful gameplay launch.
.EXAMPLE
pwsh -File ./examples/unreal-local-build/scripts/Package-Windows.ps1 -Project $env:UE_PROJECT_FILE -Configuration Development
#>
[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [string]$Project,
    [string]$EngineRoot,
    [string]$TargetName,
    [ValidateSet('Development', 'Shipping')]
    [string]$Configuration = 'Development',
    [string]$ArchiveRoot
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

. (Join-Path $PSScriptRoot 'Unreal-Local.ps1')
$context = Get-UnrealLocalContext -Project $Project -EngineRoot $EngineRoot -TargetType Game -TargetName $TargetName

if ([string]::IsNullOrWhiteSpace($ArchiveRoot)) {
    $ArchiveRoot = Join-Path $context.ProjectRoot 'LocalBuilds'
}
$resolvedArchiveRoot = [System.IO.Path]::GetFullPath($ArchiveRoot)

# Never archive into the Engine, project source/content or generated Unreal directories.
$restrictedRoots = @(
    $context.EngineRoot,
    (Join-Path $context.ProjectRoot 'Content'),
    (Join-Path $context.ProjectRoot 'Config'),
    (Join-Path $context.ProjectRoot 'Source'),
    (Join-Path $context.ProjectRoot 'Intermediate'),
    (Join-Path $context.ProjectRoot 'Binaries'),
    (Join-Path $context.ProjectRoot 'Saved')
)
foreach ($restricted in $restrictedRoots) {
    $canonical = [System.IO.Path]::GetFullPath($restricted).TrimEnd([char]'\', [char]'/')
    if ($resolvedArchiveRoot.Equals($canonical, [System.StringComparison]::OrdinalIgnoreCase) -or
        $resolvedArchiveRoot.StartsWith($canonical + [System.IO.Path]::DirectorySeparatorChar, [System.StringComparison]::OrdinalIgnoreCase)) {
        throw 'ArchiveRoot must be outside Engine and authored/temporary Unreal project directories.'
    }
}

if (-not (Test-Path -LiteralPath $resolvedArchiveRoot -PathType Container)) {
    New-Item -ItemType Directory -Path $resolvedArchiveRoot -Force | Out-Null
}

$stamp = Get-Date -Format 'yyyyMMdd-HHmmss-fff'
$runId = [Guid]::NewGuid().ToString('N').Substring(0, 8)
$archiveDirectory = Join-Path $resolvedArchiveRoot "$($context.TargetName)-Win64-$Configuration-$stamp-$runId"

if (Test-Path -LiteralPath $archiveDirectory) {
    throw 'A generated archive directory unexpectedly already exists. No files have been replaced.'
}
New-Item -ItemType Directory -Path $archiveDirectory | Out-Null

Write-Host "Building, cooking and archiving $($context.TargetName) Win64 $Configuration"
Invoke-UnrealLocalBatch -BatchFile $context.UatBatchFile -Arguments @(
    'BuildCookRun',
    "-project=$($context.ProjectFile)",
    '-noP4',
    '-platform=Win64',
    "-clientconfig=$Configuration",
    '-build',
    '-cook',
    '-stage',
    '-pak',
    '-package',
    '-archive',
    "-archivedirectory=$archiveDirectory",
    '-unattended',
    '-utf8output'
)

$gameExe = @(Get-ChildItem -LiteralPath $archiveDirectory -Filter '*.exe' -File -Recurse |
    Where-Object { $_.BaseName -ieq $context.TargetName } |
    Select-Object -First 1)

if ($gameExe.Count -ne 1) {
    throw "UAT exited successfully, but the expected game executable '$($context.TargetName).exe' was not found in the archive. Inspect the output and target configuration."
}

Write-Host "Archive created: $archiveDirectory"
Write-Host "Game executable found: $($gameExe[0].FullName)"
Write-Host 'Not yet validated: process launch, controls, map loading, gameplay route, Save/Load, or release suitability.'
Write-Host 'Next gate: launch the archived game executable outside Unreal Editor and record real results.'
