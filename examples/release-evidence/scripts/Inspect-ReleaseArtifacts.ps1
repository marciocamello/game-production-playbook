#Requires -Version 7.0
<#
.SYNOPSIS
Read-only verification of an EXISTING Windows game archive and optional gameplay capture.
.DESCRIPTION
Checks exactly one named game executable, cooked file presence, hashes and real
ffprobe video metadata. Never launches, edits, deletes or publishes any files.
#>
[CmdletBinding()]
param(
    [Parameter(Mandatory=$true)][string]$ArchiveDirectory,
    [Parameter(Mandatory=$true)][string]$TargetName,
    [string]$Video,
    [ValidateRange(0,8192)][int]$MinWidth=0,
    [ValidateRange(0,8192)][int]$MinHeight=0,
    [ValidateRange(0,86400)][double]$MinSeconds=0,
    [ValidateRange(0,86400)][double]$MaxSeconds=0
)
Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'
if (-not (Test-Path -LiteralPath $ArchiveDirectory -PathType Container)) {
    throw "Existing archive directory not found: $ArchiveDirectory"
}
if ([string]::IsNullOrWhiteSpace($TargetName) -or
    $TargetName.IndexOfAny([System.IO.Path]::GetInvalidFileNameChars()) -ge 0) {
    throw 'TargetName must be an executable basename, without a path.'
}
if ($MaxSeconds -gt 0 -and $MinSeconds -gt $MaxSeconds) {
    throw 'MinSeconds cannot exceed MaxSeconds.'
}
$root = (Resolve-Path -LiteralPath $ArchiveDirectory).Path
$files = @(Get-ChildItem -LiteralPath $root -File -Recurse)
if ($files.Count -eq 0) { throw 'Archive is empty.' }
$exes = @($files | Where-Object {
    $_.Extension -ieq '.exe' -and $_.BaseName -ieq $TargetName
})
if ($exes.Count -ne 1) {
    throw "Expected exactly one matching game executable; found $($exes.Count)."
}
$cooked = @($files | Where-Object {
    $_.Extension -in @('.pak', '.utoc', '.ucas')
})
if ($cooked.Count -eq 0) {
    throw 'No cooked .pak/.utoc/.ucas containers found; inspect actual packaging.'
}
$videoReport = $null
if (-not [string]::IsNullOrWhiteSpace($Video)) {
    if (-not (Test-Path -LiteralPath $Video -PathType Leaf)) {
        throw "Video file missing: $Video"
    }
    $ffprobe = Get-Command ffprobe -ErrorAction SilentlyContinue
    if (-not $ffprobe) {
        throw 'ffprobe is required for measured video metadata.'
    }
    $probeJson = & $ffprobe.Source -v error -select_streams 'v:0' -show_entries 'stream=width,height' -show_entries 'format=duration' -of json $Video
    if ($LASTEXITCODE -ne 0) { throw 'ffprobe failed; video remains unverified.' }
    $probe = ($probeJson -join [Environment]::NewLine) | ConvertFrom-Json
    if (-not $probe.streams -or $probe.streams.Count -lt 1) {
        throw 'No video stream found.'
    }
    $width = [int]$probe.streams[0].width
    $height = [int]$probe.streams[0].height
    $seconds = [double]::Parse([string]$probe.format.duration, [System.Globalization.CultureInfo]::InvariantCulture)
    if ($width -lt $MinWidth -or $height -lt $MinHeight) {
        throw "Video resolution below minimum: $width x $height."
    }
    if ($seconds -lt $MinSeconds -or ($MaxSeconds -gt 0 -and $seconds -gt $MaxSeconds)) {
        throw "Video duration outside requested bounds: $seconds seconds."
    }
    $videoReport = [pscustomobject]@{
        Path = (Resolve-Path -LiteralPath $Video).Path
        Width = $width
        Height = $height
        DurationSeconds = $seconds
        SHA256 = (Get-FileHash -LiteralPath $Video -Algorithm SHA256).Hash
    }
}
$exe = $exes[0]
[pscustomobject]@{
    ArchiveDirectory = $root
    FileCount = $files.Count
    CookedContainerCount = $cooked.Count
    GameExe = $exe.FullName
    GameExeBytes = $exe.Length
    GameExeSHA256 = (Get-FileHash -LiteralPath $exe.FullName -Algorithm SHA256).Hash
    Video = $videoReport
    RuntimeLaunch = 'NOT RUN - launch the archived executable and verify gameplay'
    HumanAcceptance = 'NOT RUN - requires an observed human playtest'
} | Format-List
