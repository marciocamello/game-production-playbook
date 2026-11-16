# Chapters 15–16 — Testing, Optimization, Packaging and Release

## Separate verification layers

| Layer | What it can establish | What it **cannot** establish |
| --- | --- | --- |
| Native compile | Source and link are compatible with target | Functional gameplay, UI visibility or scene traversal |
| Automated tests | Specific assertions passed under the recorded fixture | All player behavior, camera and art |
| Editor/MCP checks | Assets exist, compile/save/reload and pass queried validators | Human playability, sounds in packaged runtime |
| Manual PIE | Human-observed input, gameplay and visual outcome inside Editor | Cooked dependencies and fresh packaged process |
| Cook/package | Required content and build stages completed | Launch, full route, save durability |
| Packaged launch and play | Actual standalone game behavior in the tested environment | Every hardware, controller and accessibility combination |
| Public upload/download | Artifact is retrievable from the stated platform | Long-term quality or zero defects |

Always report exact test version, command/procedure, assertions, environment, failures, caveats and evidence. **Process exit code 0 is not necessarily a full test-suite PASS**: read the test-case results. Report NOT RUN instead of silently dropping checks.

## Test matrix for a narrative puzzle game

1. Fresh New Game: controls, camera, objective, interaction distance and prompt.
2. Pickup or reveal: collect exactly once; cancel/lose focus without duplicates.
3. Donation: tap vs hold, early release, repeated request, denial reasons, actual loss of ability.
4. World restoration: object materials, animation, collision and newly traversable route.
5. State recovery: revisit old area, stream relevant cells out/in, verify no duplicated pickup, effects or closed gates.
6. Save and load: separate process, checkpoint transform, ability grants, world Actors, valid/invalid snapshots and UI result.
7. Controls: remapped movement, interaction, menu focus, pause/unpause and settings persistence.
8. Presentation: camera obstruction, audio routes, finale input lock, sequence skipping/failure and return to control.
9. Full route: no irreversible softlock, correct ending and epilogue where approved.
10. Shipping: fresh archive, standalone executable, logs, graphics settings, audio, performance and downloads.

## Prioritize delivery

Produce an early, playable **packaged slice** and rerun after major content changes. Freeze scope when bug fixing and submission media would be threatened. Cut decorative extras before critical interactions, Save/Load, controls, ending and packaging. Scope cuts are documented decisions, not defects to hide.

## Performance

Measure actual game scenes, not only Editor shader statistics. Record target hardware or class, resolution, renderer settings, frame timings, memory, streaming behavior and reproducibility. Profile if draw calls, particle counts, world streaming, mesh density, material cost, animation instances or editor-only dependencies are suspected. Optimize measured bottlenecks; recheck visual quality and functionality after changes.

## Release package

The release owner confirms: target build config, output contents, required redistributables per platform, executable launch outside Editor, new-game and continue behavior, rights/credits, input help, store page, screenshots and actual gameplay video, file sizes/hash when useful, download access, and final submitter.

For time-limited jams, check the specific event's rules for allowed development period, video duration/resolution, submission language, age rating, team and required dev-log access. The **current official rules supersede any previous case-study summary**.

**Exit evidence:** passing recorded checks, a human-playtested game, an externally launched package and verifiable publication.