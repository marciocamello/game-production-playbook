# The Unmade — Scope Decisions, Deferred Work and Explicit Unknowns

Game jams require different decisions from unconstrained production. A polished complete path is more valuable than large collections of unrelated systems.

## Approved compact experience

The core GDD specified a single-player, no-combat game in one continuous World Partition level: awakening; fountain/Light gift; Light recovery; Gear Court/Impulse and permanent crossing; garden clue; final heart gift; revival and a short playable epilogue. The expected target duration was a **design estimate**, not a measured completion time.

Essential technologies selected included C++/Blueprint, GAS, Gameplay Tags, UMG/MVVM, real Save/Load and basic configuration. The player could not permanently lose walking or become trapped after a donation.

## Not in the initial critical path

- **Combat and enemies:** intentionally excluded from the approved game direction.
- **Echo and multiple optional abilities:** investigated but cut from the compact mandatory puzzle arc.
- **Procedural city and huge open world:** unnecessary; authored readable world and modular geometry took precedence.
- **Extra biome, extended garden and branching puzzle tree:** deferred to protect core progression.
- **PCG, Mesh Terrain and large-scale Chaos simulation:** optional production tools, not mandatory mechanics.
- **Additional UI frameworks/Common plugins:** reduced to what provided actual benefit; custom infrastructure avoided.
- **Runtime LLM/companion:** pre-theme research hypothesis, not selected for The Unmade's mandatory gameplay.
- **Additional character types, dense decoration and advanced graphical settings:** subordinate to route, ending, input, audio, stability and packaging.

## Distinguish cuts from unverified implementation

“Out of core scope” means an explicit design exclusion; “Deferred” means planned or proposed work postponed; “Not Verified” means there is insufficient evidence to claim runtime acceptance. They are not interchangeable. Later asset work, additional Kin instances or visual polish may exist even if they were optional in the initial design.

## A general release cut line

If the game cannot be completed and launched outside the Editor, cut optional visual density before cutting critical state changes, basic controls, Save/Load, player feedback, the finale or a required release video. Reassess after measured playtests and known packaging risks.