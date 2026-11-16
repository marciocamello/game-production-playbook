#Requires -Version 7.0
<#
.SYNOPSIS
Reads Git branch, HEAD, staged/unstaged changes and ordinary untracked files.
.DESCRIPTION
Read-only intent: does not stage, commit, reset, clean or switch branches.
Uses --no-optional-locks to avoid Git status index-refresh writes.
Does not inspect ignored files, unsaved Unreal packages, or Unreal Editor state.
.EXAMPLE
pwsh -File ./examples/multi-agent-handoff/scripts/Inspect-GitPreflight.ps1 -Repository .
#>
[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [string]$Repository
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

if (-not (Get-Command git -ErrorAction SilentlyContinue)) {
    throw 'Git is not installed or is not available in PATH.'
}

$checkout = Get-Item -LiteralPath $Repository -ErrorAction Stop
if (-not $checkout.PSIsContainer) {
    throw '-Repository must identify an existing directory.'
}

Push-Location -LiteralPath $checkout.FullName
try {
    $isGit = & git --no-optional-locks rev-parse --is-inside-work-tree
    if ($LASTEXITCODE -ne 0 -or "$isGit".Trim() -ne 'true') {
        throw 'The selected directory is not inside a Git working tree.'
    }

    Write-Output '== Current revision =='
    & git --no-optional-locks rev-parse --verify HEAD
    if ($LASTEXITCODE -ne 0) {
        throw 'Could not read the current HEAD. The repository may have no commits.'
    }

    Write-Output '== Branch or detached HEAD =='
    & git --no-optional-locks branch --show-current
    if ($LASTEXITCODE -ne 0) { throw 'Cannot read current branch.' }

    Write-Output '== Working tree (tracked + ordinary untracked, NOT ignored) =='
    & git --no-optional-locks -c core.quotepath=false status --short --branch --untracked-files=all
    if ($LASTEXITCODE -ne 0) { throw 'Cannot read Git status.' }

    Write-Output '== Unstaged changed paths =='
    & git --no-optional-locks -c core.quotepath=false diff --name-status
    if ($LASTEXITCODE -ne 0) { throw 'Cannot read unstaged changes.' }

    Write-Output '== Staged changed paths =='
    & git --no-optional-locks -c core.quotepath=false diff --cached --name-status
    if ($LASTEXITCODE -ne 0) { throw 'Cannot read staged changes.' }

    Write-Output '== Ordinary untracked paths (ignored paths omitted) =='
    & git --no-optional-locks -c core.quotepath=false ls-files --others --exclude-standard
    if ($LASTEXITCODE -ne 0) { throw 'Cannot read untracked file list.' }

    Write-Output '== Warning =='
    Write-Output 'This is Git-only, read-only evidence. Inspect ignored files, local source assets, and unsaved Unreal Editor packages separately before assigning any writer.'
}
finally {
    Pop-Location
}
