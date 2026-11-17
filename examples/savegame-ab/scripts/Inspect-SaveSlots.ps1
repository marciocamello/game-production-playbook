#Requires -Version 7.0
<#
.SYNOPSIS
Reports file-level evidence for Unreal SaveGame slot files without altering them.
.DESCRIPTION
Computes size, modification timestamp and SHA-256 for the chosen named slots.
This is a file inspection tool, NOT an Unreal deserializer or save-validity test.
The caller must specify the actual save directory for the correct project/process.
.EXAMPLE
pwsh -File ./examples/savegame-ab/scripts/Inspect-SaveSlots.ps1 -SaveDirectory './Saved/SaveGames'
.EXAMPLE
pwsh -File ./examples/savegame-ab/scripts/Inspect-SaveSlots.ps1 -SaveDirectory './Saved/SaveGames' -OutputJson './artifacts/save-slots.json'
#>
[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [string]$SaveDirectory,

    [ValidateNotNullOrEmpty()]
    [string[]]$SlotNames = @('GPP_Run_A', 'GPP_Run_B'),

    [string]$OutputJson
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

if (-not (Test-Path -LiteralPath $SaveDirectory -PathType Container)) {
    throw "Save directory does not exist: $SaveDirectory"
}

$directory = (Resolve-Path -LiteralPath $SaveDirectory).ProviderPath
$slots = [System.Collections.Generic.HashSet[string]]::new([System.StringComparer]::OrdinalIgnoreCase)
$results = @(
    foreach ($slotName in $SlotNames) {
        if ([string]::IsNullOrWhiteSpace($slotName) -or
            $slotName.IndexOfAny([System.IO.Path]::GetInvalidFileNameChars()) -ge 0 -or
            $slotName.Contains('/') -or $slotName.Contains('\') -or
            $slotName -eq '.' -or $slotName -eq '..' -or
            -not $slots.Add($slotName)) {
            throw "Each slot must be a unique, safe file base name: $slotName"
        }

        $savePath = Join-Path -Path $directory -ChildPath "$slotName.sav"
        if (-not (Test-Path -LiteralPath $savePath -PathType Leaf)) {
            [pscustomobject]@{
                Slot             = $slotName
                Status           = 'Missing'
                Bytes            = $null
                ModifiedUtc      = $null
                SHA256           = $null
            }
            continue
        }

        $file = Get-Item -LiteralPath $savePath -ErrorAction Stop
        $hash = (Get-FileHash -LiteralPath $savePath -Algorithm SHA256 -ErrorAction Stop).Hash

        [pscustomobject]@{
            Slot             = $slotName
            Status           = 'Found'
            Bytes            = $file.Length
            ModifiedUtc      = $file.LastWriteTimeUtc.ToString('o')
            SHA256           = $hash
        }
    }
)

$results | Format-Table -AutoSize | Out-Host

if (-not [string]::IsNullOrWhiteSpace($OutputJson)) {
    $outPath = [System.IO.Path]::GetFullPath($OutputJson)
    $outParent = [System.IO.Path]::GetDirectoryName($outPath)

    if (-not (Test-Path -LiteralPath $outParent -PathType Container)) {
        throw "Output parent directory does not exist: $outParent"
    }
    # Reports must not be created inside the game's SaveGames directory.
    # This also protects the .sav namespace from accidental report placement.
    $safeSaveRoot = [System.IO.Path]::GetFullPath($directory).TrimEnd([char]'\\', [char]'/')
    if ($outPath.Equals($safeSaveRoot, [System.StringComparison]::OrdinalIgnoreCase) -or
        $outPath.StartsWith($safeSaveRoot + [System.IO.Path]::DirectorySeparatorChar, [System.StringComparison]::OrdinalIgnoreCase)) {
        throw 'OutputJson must be outside the SaveGame directory.'
    }
    $payload = [pscustomobject]@{
        CapturedUtc = [DateTime]::UtcNow.ToString('o')
        SlotEvidence = $results
    }

    # FileMode.CreateNew atomically refuses an existing report. Set-Content
    # would silently overwrite it and violates the read-only evidence contract.
    $json = $payload | ConvertTo-Json -Depth 6
    $stream = [System.IO.File]::Open($outPath, [System.IO.FileMode]::CreateNew,
        [System.IO.FileAccess]::Write, [System.IO.FileShare]::None)
    try {
        $writer = [System.IO.StreamWriter]::new(
            $stream, [System.Text.UTF8Encoding]::new($false))
        try {
            $writer.Write($json)
            $writer.Flush()
        }
        finally {
            $writer.Dispose()
        }
    }
    finally {
        $stream.Dispose()
    }
    Write-Host "Evidence report created without overwrite: $outPath"
}

Write-Host 'Only file-level evidence was inspected; gameplay consistency must be tested in Unreal.'
