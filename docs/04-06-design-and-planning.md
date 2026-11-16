# Chapters 04–06 — Game Design, Technical Design and Executable SDD

## 04. Game Design Document

**Hands-on learning path:** [T03 — From Concept to GDD, TDD and Executable SDD](../tutorials/concept-to-gdd-tdd-executable-sdd.md). Reuse the existing [Project Charter](../templates/project-charter.md), [GDD](../templates/game-design-document.md), [TDD](../templates/technical-design.md), [Architecture Decision Record](../templates/architecture-decision-record.md), and [Feature Spec](../templates/feature-specification.md) rather than inventing competing canonical files.

A useful GDD describes **what the player experiences** and what is intentionally out of scope. Keep story and implementation rules separate until the game director approves them.

Minimum sections:

1. Creative vision, theme and emotional progression.
2. Player fantasy, target platform, camera, control scheme and accessibility principles.
3. Core loop: player action → state consequence → world response → next viable action.
4. Mechanics: input, legal/illegal states, costs, reversible and irreversible transitions.
5. Progression: beginning, objectives, obstacles, recovery, finale and optional epilogue.
6. Failure cases: missing pickup, interrupted input, death/restart if applicable, softlock protection.
7. Visual language and sound motifs.
8. Scope: mandatory, optional, explicitly cut, and validated assumptions.

**Example from The Unmade:** a mechanical part repairs a fountain's missing physical mechanism; donating personal Light is a distinct voluntary act; restoration opens a real passage; a different later fragment recovers personal Light without undoing the fountain. These distinctions prevent a visually plausible but mechanically false implementation.

## 05. Technical Design Document

The TDD translates the approved GDD into **ownership of authoritative game state** and integration contracts.

The Unmade's approved UE 5.9 architecture provides an instructive pattern:

| Authority | Role |
| --- | --- |
| Character / PlayerController | Player movement, interaction and input focus |
| GAS / AbilitySystemComponent | Real ability execution and eligibility |
| GameInstanceSubsystem | Durable in-memory progression authority |
| World Actors | Apply authoritative state to visible meshes, collisions and interactions |
| SaveGame | Versioned logical snapshots; no live Actor pointer serialization |
| UMG / MVVM | Read-only player-facing state and commands routed to owning services |
| Niagara / Sequencer / animation | Presentation reacting to committed actions; never progression authority |

**State contract:** validate an interaction and available capability → commit once to the progression owner → reconcile ability and world state → notify UI/VFX/audio. The renderer cannot be permitted to open an unearned route or accidentally undo a donation.

**Streaming contract:** when a World Partition Actor loads, it queries durable logical state and idempotently applies the correct appearance/collision; a one-time transition is played only on a *new* committed event, not on load. Critical paths must remain traversable after capabilities are lost.

**Save contract:** save logical identifiers and legal states, not pointers, transient UI properties or runtime ability handles. The Unmade chose two alternating snapshot slots with generation ordering and validation as an intended resilience strategy; it still requires failure and process-restart tests to prove its implementation.

Do not import a large framework without a concrete consumer. Consider native delegates for local notifications and message buses only where multiple independent subscribers exist.

## 06. From design to executable feature specification

Each feature identifies:

- The exact player-visible outcome and exclusions.
- Source-of-truth references and already implemented classes/assets.
- State transitions, preconditions, rollback and cancellation behavior.
- Owning C++/Blueprint/assets/audio paths and named sole Editor operator.
- Dependent workstreams that can safely run in parallel.
- Exact handoff inputs and exported files.
- Automated tests, Editor checks, manual playtest, packaged checks and evidence.
- Failure cases, performance risks, migration and clean-up requirements.

**Not acceptable:** a stub method, an empty Blueprint graph, a generated 3D file not imported to the level, an Editor compile advertised as human PIE, or a packaged archive advertised as a successful playthrough.

### Recommended feature sequence

Do not fully specify dozens of disconnected features before first play. Define the *complete story and technical dependencies*, then execute the smallest real slice. Run early local packaging; implement persistence and essential controls before adding expansive content. Reassess scope after each human playtest.

For the full task contract and evidence vocabulary: [Feature Execution Workflow](06-production-planning/feature-execution.md).

**Exit evidence:** approved GDD/TDD, one authoritative state model, an implementation-ready first feature, and an explicit production order.