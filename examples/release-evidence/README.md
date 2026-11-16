# T19 Example — Read-Only Windows Release Artifact Inspector

**Source:** [Inspect-ReleaseArtifacts.ps1](scripts/Inspect-ReleaseArtifacts.ps1)

**Status:** Real original PowerShell 7 source is published, but has **NOT RUN** on a real Windows archive/video in this publication. It does not create an Unreal project, cook/package a game, upload, execute the game or simulate successful gameplay.

## Purpose and actual operations

Use this script **after** the existing [T02 Package-Windows.ps1](../unreal-local-build/scripts/Package-Windows.ps1) has produced an actual UAT game archive and after genuine gameplay video has been captured/exported:

- Requires an existing nonempty archive directory and exactly one matching TargetName.exe.
- Requires at least one real cooked asset container file (.pak, .utoc or .ucas), appropriate to the standard sample packaging flow.
- Reads file counts, executable size and executable SHA-256.
- When a real video is provided, requires locally installed **ffprobe**, reads encoded video width, height and duration, validates requested minimums/maximums and computes the video's SHA-256.
- Throws on missing inputs, video mismatch or failed probes; does not silently fill gaps or print fake media characteristics.
- Outputs an explicit runtime launch and human acceptance **NOT RUN** reminder; static file inspection cannot certify a playable product.

**Sample PowerShell 7 command (supply actual existing paths):**

    pwsh -File ./examples/release-evidence/scripts/Inspect-ReleaseArtifacts.ps1 -ArchiveDirectory $env:GAME_ARCHIVE_DIRECTORY -TargetName $env:GAME_TARGET -Video $env:GAME_CAPTURE_MP4 -MinWidth 1920 -MinHeight 1080 -MinSeconds 30 -MaxSeconds 60

The numeric video constraints are an **example** of an event-specific requirement; use the actual current platform/event rules. The script never modifies any source, output, package or clip.

## Limitations and required next gates

An EXE and cooked containers present in a folder do not prove runtime launch, maps, content dependencies, SaveGame persistence, audio, camera return or performance. Use [T19's explicit standalone/release checklist](../../tutorials/windows-packaging-gameplay-capture-release.md) and [T18's PIE profiling and human acceptance procedure](../../tutorials/native-tests-editor-pie-profiling.md) for those distinct gates.

This original script is offered under the repository's [MIT source license](../../LICENSE-CODE.md). ffprobe is external software with its own license and must be separately obtained and verified.
