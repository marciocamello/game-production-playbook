# Chapters 13–14 — UI, Audio, VFX and Cinematics

## Player interface and MVVM

**Hands-on tutorial:** [T09 — UMG/MVVM, HUD, Menus, Settings and Loading Boundaries](../tutorials/umg-mvvm-hud-menus-settings-loading.md), with a real source-level [HUD ViewModel and operational UI Controller example](../examples/umg-mvvm-demo/README.md) and an explicit checklist for required Editor-authored widgets, persistent settings and truthful Save/Continue outcomes.

The UI communicates **real** state; it must not own progression truth.

- Main menu: New Game, Continue (only if a validated saved session exists), Options, Controls and Quit as appropriate.
- HUD: current objective, ability availability, contextual input prompt, actual interaction cost, hold progress and operation feedback.
- Pause and settings: real display/graphics/audio/game settings, input rebinding, conflict resolution and confirm/revert of risky display changes.
- Loading: show real start/load/save status and errors, never success before durable completion.
- End flow: an explicit completed-run state, readable end/credits, and return navigation.
- Accessibility baseline: legible scale, keyboard/gamepad focus as supported, remappable essential actions, input cues not expressed by color alone.

UMG ViewModels can project state using field notifications while command handlers invoke the relevant gameplay or settings authority. A ViewModel existing in C++ is not a working WBP: the real widget and bindings must be authored, integrated and manually tested.

## Audio

Define a small original music/ambience identity associated with player and world changes. Prepare sources separately from export-ready files. Check loop boundaries, spatialization, volume routes, sound classes/mixes or current equivalent, and persistent audio settings. Use silence deliberately when narrative impact requires it.

Record tool/provider, license, source file, export file, version and actual in-game usage. An audio prompt is not a sound asset; an imported sound is not an audible, event-driven feature.

## VFX and reconstruction

Use Niagara for bounded particles, materials/light parameters for durable visual state, and Sequencer/Blueprint transforms for deterministic large-piece motions. Use Chaos only when the demonstrated impact outweighs complexity, caching and packaging risks.

**Critical rule:** visual reconstruction may decorate a committed restoration, but **does not grant abilities, change save state or open passage collision**. When presentation is skipped, interrupted or reloaded, the already committed gameplay state must still be correct.

**T15 practical tutorial:** [Niagara, Chaos and deterministic restoration](../tutorials/niagara-chaos-deterministic-restoration.md), plus [original C++ presenter source](../examples/restoration-vfx/README.md), separates a fresh committed donation from restored-state projection after load/streaming. The visual actor animates nonblocking real meshes, triggers an Editor-authored Niagara System, and never controls the durable gate. Optional Chaos decoration has its own separate verification gate.

**T17 hands-on audio:** [Reactive Music, Spatial Sound and Audio Rights](../tutorials/reactive-music-spatial-audio-rights.md) reuses the authoritative T07 events for a spatial one-shot and persistent state-projected ambiences; actual MetaSound/SoundWave authoring, audibility, audio routing and licenses require verified Editor/packaged execution.

## Cinematic handoff

**T16 practical cinematic:** [Sequencer, Gameplay Transitions and Cinematic Return](../tutorials/sequencer-gameplay-cinematic-return.md) uses the existing T15 fresh-commit event and project input/camera authority for actual authored Level Sequences, normal/skip/interrupted return, and 26 explicitly separate QA gates. No real sequence is created just by publishing the tutorial.



Define explicit states for camera takeover, gameplay input lock, actor movement, animation event, sound/VFX and restoration of control. Avoid long mandatory sequences; test interruption, load-after-event, and disabled presentation. For a finale, verify the player regains the correct camera, HUD and movement in the intended post-finale area.

**Exit evidence:** complete player-facing interface, audible/reactive sound, reliable FX and cinematics, accessibility checks, and packaged-game verification.