#Requires -Version 7.0
<#
.SYNOPSIS
Builds an existing Unreal Engine 5.9 C++ Editor target on Windows.
.DESCRIPTION
Uses the existing project target and selected installed Engine.
Does not create a project, run PIE, or validate gameplay.
.EXAMPLE
pwsh -File ./examples/unreal-local-build/scripts/Build-Editor.ps1 -Project $env:UE_PROJECT_FILE
#>
[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [string]$Project,
    [string]$EngineRoot,
    [string]$TargetName,
    [ValidateSet('Development', 'DebugGame')]
    [string]$Configuration = 'Development'
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

. (Join-Path $PSScriptRoot 'Unreal-Local.ps1')

$context = Get-UnrealLocalContext -Project $Project -EngineRoot $EngineRoot -TargetType Editor -TargetName $TargetName
Write-Host "Building $($context.TargetName) Win64 $Configuration for the selected Unreal 5.9 project."

Invoke-UnrealLocalBatch -BatchFile $context.BuildBatchFile -Arguments @(
    $context.TargetName,
    'Win64',
    $Configuration,
    "-Project=$($context.ProjectFile)",
    '-WaitMutex',
    '-NoHotReloadFromIDE'
)

Write-Host 'Editor target build completed. Next: open the project, inspect assets, and run real tests.'
