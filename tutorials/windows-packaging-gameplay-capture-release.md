# T19 — Windows Packaging, Real Gameplay Capture and Release: From Editor Project to Downloadable Game

**Publication status:** Full Unreal Engine 5.9 release process, reusing the already published [T02 PowerShell BuildCookRun wrapper](../examples/unreal-local-build/README.md), plus a [new read-only archive/video inspector](../examples/release-evidence/README.md). No build, cooking, archive, genuine gameplay capture, video encoding, download, upload, installation or player test was executed for this publication. **All real delivery gates remain NOT RUN.**

**Goal:** produce a real, self-contained Windows game archive; launch it **outside Unreal Editor**; play the complete critical path; record actual 1080p-or-higher gameplay where required; verify capture metadata, rights and checksum; publish an approved release that a new user can download and run.

This is an actionable release workflow, not a suggestion to prepare for a future implementation. The actual project, its validated gameplay and an authorized release operator are indispensable.

## 1. Freeze one exact candidate

Before packaging, identify the game/project revision and C++ branch, installed UE build, exact game target, default map, input contexts, settings, plugins and content owners. Inspect the working tree and Editor dirty packages. **Do not build from an unexplained mixture of unsaved Editor assets and a different source commit.**

The candidate must have a complete playable route or any known missing features documented as real release blockers/cuts. Verify T06 interaction, T07 save/load if included, T08 camera, T09 UI, T15 reconstruction, T16 skip/return, T17 audio and T18 QA at the intended scope. Exclude editor-only dependencies from the runtime game.

Use [T02 bootstrap and local build](unreal-project-bootstrap-local-build.md) rather than creating a second competing packaging script or adding GitHub Actions.

## 2. Real local Windows build and cook

From PowerShell 7 on the Windows machine with a supported **installed UE 5.9**, use the existing scripts. A real project path is a required input: do not substitute a demo path and count it as execution.

    pwsh -File ./examples/unreal-local-build/scripts/Build-Editor.ps1 -Project $env:UE_PROJECT_FILE -Configuration Development

    pwsh -File ./examples/unreal-local-build/scripts/Package-Windows.ps1 -Project $env:UE_PROJECT_FILE -Configuration Shipping

**Notes on this example:**

- The existing T02 script requires an actual Project path; EngineRoot, TargetName and ArchiveRoot are optional **only if** its real project/engine discovery can resolve them. Supply them explicitly where discovery is ambiguous.
- Package-Windows.ps1 calls installed **RunUAT.bat BuildCookRun** with build/cook/stage/pak/package/archive and uses a **new unique archive directory**; it does not overwrite a prior archive.
- The script verifies that the archive contains the expected game EXE, but **does not launch it, test assets or prove gameplay**. Inspect the complete UAT log and capture failed cook references, material or Blueprint errors even if a wrapper produced an output folder.
- Choose Development for diagnosis and Shipping for an actual public candidate after verifying the project's plugin/build compatibility, packaging settings and user-facing Quit/controls. A Shipping build can differ in available console/logging behavior.
- Use the project's actual Game Default Map, included maps, platform texture formats, localization, sound and cooked asset references. An Editor-successful asset excluded by cooking is not shipped.
- Avoid publishing developer logs, crash traces, private save data, build paths or API credentials with the archive. Preserve private forensic logs separately for QA.

If the build fails, record FAIL and fix the real cause; never publish an empty archive or an Editor executable as the shipped game.

## 3. Validate the **actual archived executable** in a new process

1. Locate the packaged game EXE from the actual output directory. Close the editor if relevant and launch the archived executable directly from File Explorer or a controlled PowerShell process. Record full target/revision, OS and initial process result.
2. Start New Game from the real main menu. Verify window, resolution, display settings, audio settings, input focus, controls and no unexpected editor content.
3. Follow the **entire critical gameplay route** with actual user input, not console teleport or prerecorded camera flythrough. Test cancellation and confirmed interactions, changed capabilities, gates and world collision, camera occlusion, story beats, Skip action, UI/HUD, sound and finale.
4. Save at a meaningful mid-route checkpoint, **exit the game process completely**, reopen the same archived executable and use actual Continue. Verify safe player location, collected IDs, restored gates, gained/lost abilities and no one-shot VFX/audio replays.
5. Repeat the end-to-end smoke on a second reasonably clean compatible Windows machine/account if available; test install/extraction into a path with spaces and non-ASCII characters when appropriate. Document whether runtime redistributables or external prerequisites are required.
6. Measure frame times and memory on the **packaged build**, not only PIE, using T18's repeatable route/settings. Confirm loading screens and graphics settings do not cause black screen/softlock.
7. Check that the actual public archive contains needed cooked content, license notices and instructions, but no private sources, secrets or unnecessary binaries. Do not delete unknown third-party files automatically; investigate provenance.

A game executable existing or launching briefly is **not** evidence of a working game.

## 4. Record authentic gameplay footage

The capture must show actual interactive gameplay **from the candidate build**, not AI-generated trailer frames passed off as gameplay or a noninteractive Editor preview.

1. Review the **current platform/event rules** for actual accepted formats, duration, resolution, video language, audio, required gameplay percentage and deadline. Record which rules/version were checked. A previous jam's requirements are not generic rules for future releases.
2. Launch the actual packaged game and configure approved graphics/resolution. Use OBS Studio or another trusted **real capture tool**. Set capture canvas and output to at least the required resolution. For an example 1920×1080/30–60-second jam requirement, configure that target explicitly.
3. Capture at the actual game resolution. Prefer Game Capture when it works; use Window/Display Capture if the game hook cannot attach and verify the output contains only permitted content. Avoid displaying tokens, private notifications and unrelated desktops.
4. Record a representative honest sequence: player movement/exploration, real fragment pickup, real voluntary restoration and functioning gate, distinctive visuals/audio and a final traversal/cinematic if implemented. Preserve a continuous segment or make editorial cuts **without misrepresenting real game behavior**.
5. Record native game audio and ensure voice/music source permissions allow publication. Avoid copyrighted commercial tracks without the right to redistribute. Check original clip for dropped frames, incorrect cropping, muted audio, flashing capture overlay or unexpected personal information.
6. Render/export a real MP4/H.264 or the platform-required format. Export at the actual resolution/duration, maintain audio synchronization and view the whole rendered file before submission. Do not treat the capture project's timeline length as the verified encoded duration.
7. Retain original recording, exported file, version, hashes and editing decisions in the private release evidence folder. The public Glass Box may publish a redacted and license-safe reference.

**Editing distinction:** ordinary editorial cutting is allowed if rules permit it; never represent a concept render, Unreal Sequencer offline render, synthetic cutscene or AI-generated promotional shot as captured interactive footage.

## 5. Verify archive and footage metadata with real input files

After T02 has created a real archive and a real video has been exported, run the new [read-only T19 inspector](../examples/release-evidence/scripts/Inspect-ReleaseArtifacts.ps1).

For a specific release that genuinely requires 1920×1080 and 30–60 seconds, the command is:

    pwsh -File ./examples/release-evidence/scripts/Inspect-ReleaseArtifacts.ps1 -ArchiveDirectory $env:GAME_ARCHIVE_DIRECTORY -TargetName $env:GAME_TARGET -Video $env:GAME_CAPTURE_MP4 -MinWidth 1920 -MinHeight 1080 -MinSeconds 30 -MaxSeconds 60

The inspector:

- Refuses nonexistent/empty archives, requires **exactly one target-named game EXE** and identifies real cooked .pak/.utoc/.ucas containers.
- Computes SHA-256 of the actual game EXE and captured video; reports real bytes and file count.
- Runs an **installed ffprobe** against a supplied video and refuses wrong/absent stream, undersized resolution, out-of-range duration or probe errors.
- **Never launches or modifies** the game, uploads the archive or claims a human gameplay PASS. A successful inspector result is **metadata evidence only**, not a full release.

Video is optional for a preliminary local package-only inspection. ffprobe is a third-party executable: verify its installation/source; do not install or download any software automatically during this script. If the game's chosen packaging format deliberately uses loose cooked assets rather than these containers, inspect and adapt the verification policy with evidence rather than pretending a failed check is PASS.

## 6. Publish only after distribution rights and human acceptance

Use the existing [release checklist](../templates/release-checklist.md) as the canonical acceptance record. Verify license/attribution for art, music, AI-generated source, plugins and fonts; game title/age rating/languages; controls/help; privacy/analytics disclosures if applicable; screenshots; credits; size; download and extraction experience.

The authorized human release owner performs the actual store/itch.io submission or release upload. Check the final public page from an unauthenticated browsing context when possible, download the published archive independently and compare its hash/contents against the approved candidate. A local archive and draft store page do not prove public availability.

Never silently overwrite an existing public release, push unauthorized changes to a game's repository, create GitHub Actions or promise delayed submission. If publication fails, record the exact failure and preserve the previously working archive.

## 7. Release go/no-go: 28 independent observations

| ID | Measured/observed release requirement |
| --- | --- |
| R01 | Exact source commit, target platform, Engine and plugin versions identified |
| R02 | Git state and dirty Editor assets reconciled with candidate |
| R03 | Correct Game Default Map and startup flow configured |
| R04 | UHT/UBT build actually passes |
| R05 | Cook/stage/package/archive actually completes without errors |
| R06 | Correct standalone EXE and cooked containers found |
| R07 | External archive launch succeeds |
| R08 | Main menu, New Game, Quit work outside Editor |
| R09 | Character movement, camera and input function |
| R10 | Fragment pickup and rejection conditions work |
| R11 | Voluntary donation changes actual ability and gate |
| R12 | Recovery and anti-softlock route work |
| R13 | Niagara/optional Chaos assets work as actually integrated |
| R14 | Cinematic returns gameplay on normal finish and Skip |
| R15 | Actual audio is audible, spatial and correctly licensed |
| R16 | UMG menus, options and settings persist |
| R17 | Fresh-process Save/Continue restores correct world |
| R18 | Full critical path and ending actually playable |
| R19 | No release-blocking crash, invisible geometry or missing content |
| R20 | Measured packaged performance meets approved target or documented limitation |
| R21 | Game archive contains no secrets/unrelated private material |
| R22 | Rights, credits, third-party inventory and accessibility checked |
| R23 | Real gameplay recorded from the candidate build |
| R24 | Exported video duration/resolution/codec checked from actual media |
| R25 | Captured gameplay and audio accurately represent shipped content |
| R26 | Authorized human accepts the final candidate and media |
| R27 | Upload/submission really succeeds and page is publicly reachable |
| R28 | Independently downloaded package matches the approved release |

Use PASS only when executed with actual evidence; unexecuted checks remain NOT RUN. Some projects deliberately omit a feature such as Chaos or SaveGame; mark the scope **NOT APPLICABLE** explicitly with approved reasoning rather than claim a passing test.

## 8. Failure recovery and references

- **Cook fails:** inspect first relevant error, missing map/assets/plugins and true cooked dependency graph; rebuild after fixing source.
- **EXE runs but black screen:** inspect default map, GameMode, startup UI and cooked references in actual game logs.
- **Missing VFX/sound/cinematic:** inspect saved Niagara/MetaSound/Sequence asset references and streaming dependencies; PIE working is insufficient.
- **SaveGame works only in PIE:** investigate slots, packaged writable user location, schema version, map/streaming restore and actual fresh-process launch.
- **Video missing frames/sound:** inspect original capture format, OBS input/output setup, codec and render metadata; recapture if game evidence is misleading.
- **Public download differs:** stop submission, preserve both artifacts, compare source of archive/hash and republish only with authorization.

**Official references:** [Epic Packaging Your Project](https://dev.epicgames.com/documentation/unreal-engine/packaging-your-project), [Epic Build Operations](https://dev.epicgames.com/documentation/unreal-engine/build-operations-cooking-packaging-deploying-and-running-projects-in-unreal-engine), [Epic Project Launcher](https://dev.epicgames.com/documentation/unreal-engine/using-the-project-launcher-in-unreal-engine), [OBS Studio knowledge base](https://obsproject.com/kb), [FFmpeg ffprobe documentation](https://ffmpeg.org/ffprobe.html), [T02 local build](unreal-project-bootstrap-local-build.md), [T18 QA](native-tests-editor-pie-profiling.md). Consult the actual current submission platform/event rules.

**Next:** [T20 — Glass Box, provenance and postmortem](glass-box-provenance-postmortem.md).
