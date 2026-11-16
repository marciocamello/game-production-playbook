# Chapters 13–14 — UI, Audio, VFX and Cinematics

## Player interface and MVVM

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

## Cinematic handoff

Define explicit states for camera takeover, gameplay input lock, actor movement, animation event, sound/VFX and restoration of control. Avoid long mandatory sequences; test interruption, load-after-event, and disabled presentation. For a finale, verify the player regains the correct camera, HUD and movement in the intended post-finale area.

**Exit evidence:** complete player-facing interface, audible/reactive sound, reliable FX and cinematics, accessibility checks, and packaged-game verification.