# Getting Started

This playbook supports a solo developer, a conventional team, an AI-assisted team, or an AI-driven team under human direction.

## Decide the production model

**Manual:** assign real people to gameplay, 3D, level, test and release roles. Follow every quality gate normally.

**AI-assisted:** choose specific subtasks for AI (drafting specs, code review, content research, editor scripts) but keep implementation under human operation where appropriate.

**AI-driven:** assign agents real execution scopes and supported tools. Preserve one owner per asset and a single operator for shared editor sessions. The game director approves the actual result, not the agent's claim.

## Minimal process

1. Write the game's audience, promise, platform, constraints and minimum playable loop.
2. Approve narrative/GDD and technical architecture at a level sufficient for a working slice.
3. Select one meaningful feature that exercises player action → persistent game-state change → visible feedback.
4. Inspect existing code/Blueprints/assets. Write an executable feature contract with specific expected behavior and failure cases.
5. Execute implementation, integration and real checks. Record PASS, FAIL, BLOCKED and NOT RUN.
6. Have a human play the actual integrated game. Fix problems before expanding.
7. Package and run outside the editor early; validate progress/state after restart.
8. Complete the smallest full route including onboarding, controls, sound, ending, credits, rights and download.
9. Retrospect: compare estimates and decisions with evidence and record unfinished work honestly.

## Evidence-based status vocabulary

| Status | Meaning |
| --- | --- |
| Planned | Defined but not executed |
| In Progress | Real work has started |
| Implemented | Artifacts exist and are integrated into the intended system; may still be untested |
| Tested | Listed checks actually ran, with real results; scope of testing is explicit |
| Human Validated | Designated human accepted observable game behavior and evidence |
| Released | The deliverable was actually distributed and its download/launch was verified |

**Never infer the next status from the previous one.** A successful compile is not a playtest; a passed cook is not a stable game.

## Suggested project organization

Use only directories with an immediate purpose. A compact game repository can maintain `docs/game/` for GDD/Story, `docs/architecture/` for TDD and decisions, `specs/` for executable features, `scripts/` for local automation, `assets/source/` for external editable assets, `assets/exports/` for processed deliverables, `devlog/` for evidence, and the Engine project in its own folder. Do not create fake artifacts to fill empty folders.

Follow the [197-step roadmap](../production-roadmap.md), the [SDD execution process](../06-production-planning/feature-execution.md), and the [The Unmade study](../../case-studies/the-unmade/README.md).