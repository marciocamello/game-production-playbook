# T17 — Reactive Music, Spatial Sound and Audio Rights

**Publication state:** Complete Unreal Engine 5.9 authoring and verification procedure, not an already executed sound pipeline. No SoundWave, MetaSound, attenuation asset, original recorded music, Blueprint audio binding, PIE playback or packaged audio run was created by this publication. Runtime gates are **NOT RUN**.

**Objective:** restore a real landmark and hear its one-time spatial success cue; transition ambience according to the already committed world state; preserve a correct powered ambience after Save/Load without replaying the success cue; prove that all shipped sources are properly licensed.

**Reuse first:** [T07 persistent state](savegame-reload-world-partition.md), [T09 applied audio settings](umg-mvvm-hud-menus-settings-loading.md), [T15 fresh-restoration signal](niagara-chaos-deterministic-restoration.md), [T16 Sequencer](sequencer-gameplay-cinematic-return.md). The existing Run Subsystem owns progress and exposes OnProgressChanged, OnBeaconRestored(BeaconId), and IsBeaconPowered(BeaconId). Audio consumes these; it does not install a competing progression service.

## 1. Approve the audio contract and real sources

| Layer | Trigger/authority | Spatial | Loop |
| --- | --- | --- | --- |
| Unpowered ambience | Current committed unpowered state | Positioned zone or 2D | Yes |
| Powered ambience/music | Current committed powered state | Music usually 2D | Yes |
| Successful fragment pickup | Confirmed collection transaction | At pickup | No |
| Restoration sting | Fresh successful OnBeaconRestored only | At landmark | No |
| UI feedback | Actual menu command outcome | No | No |
| Cinematic accent | Existing T16 sequence, with one owner per sound | Optional | No |

Choose one original or explicitly licensed ambient stem and one original/cleared restoration sound. Real generated audio files, provider licenses and export rights must exist before import. A prompt or synthetic waveform description is **not** an audio asset.

For every actual sound record: stable ID, source author/provider, generation model and relevant prompt if AI-produced, model/version/date, source URL or creation provenance, original file, SHA-256, sample rate, duration, license and attribution obligations, commercial/redistribution rights, source content restrictions, transformed/exported file and hash, authorized Unreal Content path, owner, and recorded import/playback/cook evidence. Reuse the [existing 3D asset/provenance contract](../templates/asset-brief.md) for fields that apply; add audio-specific fields in the project ledger. Do not invent permissions, infer an unrestricted license from an AI subscription, or publish sensitive receipts/tokens.

A missing or ambiguous redistribution grant means **BLOCKED for release**. Replace the sound or obtain the actual rights. Original procedural tones do not require third-party audio clips, but the engine/tool terms and release context still apply.

## 2. Prepare and import actual audio

1. In the chosen DAW, generate/record one real ambient loop, for example 8–20 seconds, and a short restoration transient. Save source WAV/FLAC and the approved export without overwriting source material.
2. Inspect sample rate, channels, peak and audible noise. Prefer a project-defined standard (48 kHz is reasonable); mono often improves positional SFX, stereo is common for music. Verify both loop boundaries by listening across several repetitions.
3. Import the actual audio into the approved Content folder using Unreal Editor. Inspect the real SoundWave metadata, stream behavior, duration and playback. Save and reopen the asset.
4. Use already existing Music/SFX/UI Sound Classes or routing/submix structure. Do not add a second global volume service alongside T09.
5. Create or reuse the actual landmark Actor Audio Components, with Auto Activate disabled where audio must be triggered from gameplay.

A sound that previews in the Content Browser but is never played by the real game is **Imported**, not **Integrated**.

## 3. An original procedural MetaSound one-shot exercise

As a second source option, use an actual Editor-authored MetaSound so the educational example can create an original short tone without copying music:

1. Verify MetaSounds availability in the installed UE 5.9. In an authorized Content namespace create a **MetaSound Source** such as MSS_RestorationTone.
2. Use a supported sine oscillator near 440 Hz routed through an audio-multiply gain stage and a short triggered envelope (fast attack; decay around 0.4–0.8 seconds). Connect the MetaSound On Play trigger to that envelope using the **actual installed node and pin names**.
3. Send the envelope-scaled audio to the correct output. Reduce peak gain to avoid clipping. No non-public-domain tune, imitation or borrowed recording is used.
4. Compile, save and reopen the MetaSound. Listen to the genuine output and check that the short one-shot decays fully without looping indefinitely.
5. Assign the saved MetaSound Source to the real landmark Audio Component; keep Auto Activate disabled. Playback will happen from the committed event below.
6. If the node names or graph behavior differ in this 5.9 installation, inspect installed MetaSound nodes and author the equivalent functional oscillator/envelope graph. Never claim an Editor asset exists because it appears in a written procedure.

## 4. Wire authoritative state to audio, without replay bugs

In the existing landmark Blueprint, or the game's existing audio projection component:

1. Confirm the correct existing BeaconId. Add/reuse **two real looping Audio Components** for unpowered/powered ambience and a third real one-shot Audio Component, all with valid saved sounds.
2. At BeginPlay, bind one listener to the existing GameInstance-owned OnProgressChanged and one to OnBeaconRestored. Store the owner reference; unbind on EndPlay, including World Partition unloads.
3. Read IsBeaconPowered(BeaconId) at BeginPlay and select exactly the current ambience. **Do not** fire the restoration sting during state projection.
4. OnProgressChanged, read the new committed state. If unchanged, leave the playing loop alone; if changed, FadeOut the old component and FadeIn the correct new one, typically within 0.5–2 seconds. Repeated progress events must not restart the loop or create duplicate instances.
5. On matching fresh OnBeaconRestored, play the spatial one-shot **once**. Update ambient bed using the same state-projection method; never award abilities or alter SaveGame from audio.
6. When the Actor streams out, stop/fade its owned local sources and unbind. If the music must persist globally, keep it in the existing persistent audio owner rather than spawning duplicate global music in each streamed region.
7. On fresh-process Continue, immediately select the powered ambience from saved state, **not** the one-time restoration sound. The same applies to cinematic skip, scene re-entry, checkpoint reload and recovery.

Do not let both a T16 Sequencer Audio Track and the landmark event play the same restoration cue. One producer owns each audible one-shot.

## 5. Real attenuation, routing and settings

Create and save a **Sound Attenuation** asset for the spatial sound. Set audible inner radius and falloff relative to the current camera and level dimensions. Test near, medium and far; inspect behavior through walls, hearing with headphones, and the elevated camera's viewpoint. An occlusion trace may help, but do not activate heavy traces without measuring cost.

Use actual Sound Concurrency settings to prevent repeated simultaneous one-shots, not an arbitrary delay that hides event bugs. Configure the project audio routes so **Music**, **SFX** and **UI** volume from T09 are reflected in actual sound output and survive process restart. Test mute=0, intermediate volume, restore from saved settings, focus loss, pause and UI navigation.

Accessibility: provide a visible cue for required interactions; no mandatory progress information should be conveyed by sound alone. For voices, author synchronized subtitles or captions and check actual spoken text, rights and localization.

## 6. Advanced optional music: MetaSound parameters and Quartz

For quantized, tempo-aligned layering, inspect the installed **Quartz** plugin/API and its clock/quantization nodes. Prepare real stems with matching tempo, phase and loop lengths; create a clock at the intended tempo and schedule transitions on measured beat/bar boundaries. Validate both game thread event timing and audible render thread timing in Editor and the package.

Quartz is unnecessary for a simple two-bed crossfade. Prefer a stable real transition to a complex system whose stems, licenses or player-facing effect cannot be verified.

## 7. Verification matrix — 28 checks

| ID | Observable requirement |
| --- | --- |
| A01 | Audited existing audio owners, settings and delegates |
| A02 | Actual sources exist, with clear licenses and hashes |
| A03 | All required attributions and redistribution rights reviewed |
| A04 | No unknown-license audio in the shipping dependency graph |
| A05 | Real duration, sample rate, peaks and loops inspected |
| A06 | Source audio imported into actual saved SoundWave packages |
| A07 | Original MetaSound graph compiled and sounded, if chosen |
| A08 | One-shot Auto Activate disabled |
| A09 | Saved sound attenuation is assigned and effective |
| A10 | Unpowered state starts the correct ambient layer |
| A11 | Powered state starts the correct ambient layer |
| A12 | Fresh donation plays the restoration cue once |
| A13 | Invalid or repeated donation produces no second cue |
| A14 | Save/Load does not replay the cue |
| A15 | Stream-out unsubscribes and stops owned local audio |
| A16 | Stream-in projects correct state, without duplicate music |
| A17 | State unchanged does not restart the loop |
| A18 | Crossfade has no drop, clipping or unintended overlap |
| A19 | Audible distance attenuation meets design |
| A20 | Occlusion, if enabled, behaves as designed |
| A21 | SFX/Music/UI volume changes apply to correct routes |
| A22 | Volume preferences survive a fresh process |
| A23 | Audio correctly handles pause, focus and menus |
| A24 | Cinematic does not double-play restoration cue |
| A25 | Concurrency policy prevents voice overload |
| A26 | Visible alternatives/captions cover gameplay-critical cues |
| A27 | Packaged Windows game audibly plays expected content |
| A28 | Human reviewer accepts sound identity, clarity and mix |

Record **PASS / FAIL / BLOCKED / NOT RUN** for each check, with source references, actual package paths, captured observations and human listening evidence. Generated or imported sound alone cannot satisfy game-level acceptance.

## 8. Failure diagnosis and authoritative references

- Silence: inspect real SoundWave/MetaSound assignment, sound class volume, component activation, one-shot event, attenuation and concurrency.
- Duplicate sounds: find duplicate event bindings, duplicated persistent music instances or Sequencer tracks playing the same cue.
- Music does not match SaveGame: derive the bed from IsBeaconPowered on BeginPlay and OnProgressChanged; do not treat the most recently played audio as state.
- Discontinuity: inspect actual seam, mismatched music stems, sample lengths and crossfade.
- Packaged silence: inspect cooked sound references, runtime audio device/logs and redistributable dependencies; Editor preview is not a release gate.

**Official references:** [Epic MetaSounds](https://dev.epicgames.com/documentation/unreal-engine/metasounds-the-next-generation-sound-sources-in-unreal-engine), [MetaSounds Quick Start](https://dev.epicgames.com/documentation/unreal-engine/metasounds-quick-start), [Quartz Quick Start](https://dev.epicgames.com/documentation/unreal-engine/quartz-quick-start), [Sound Attenuation](https://dev.epicgames.com/documentation/unreal-engine/sound-attenuation-in-unreal-engine), [Audio Modulation](https://dev.epicgames.com/documentation/unreal-engine/audio-modulation-overview-in-unreal-engine). Verify the exact menus, nodes and engine version in the installed UE 5.9 Editor.

**Next:** [T18 — Native Tests, Editor Checks, Human PIE and Profiling](native-tests-editor-pie-profiling.md).
