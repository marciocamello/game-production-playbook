# Template — End-to-End Feature Specification

**Purpose:** write a contract for a **real implemented and integrated behavior**. A filled-out spec is *Planned* until work starts; publishing this template is not a feature implementation.

## Header to fill from evidence

- Feature ID and descriptive name:
- Approved game vision / GDD / TDD references:
- Version or project baseline actually inspected:
- Feature owner and approving director:
- Source/Blueprint/editor/asset/audio/QA owners:
- Current stage and evidence date:

## Player-visible result

Describe a complete user action and observable consequence. Example pattern: *After the player collects a missing part and voluntarily performs the approved sacrifice, the object changes authoritative state, its collision and route update, the player's ability is actually disabled and a valid next objective becomes accessible.*

## Preconditions and source reuse

Document the **existing** APIs, classes, assets, state owners and active editor context. State what will be extended rather than duplicated. List incompatible or missing dependencies explicitly.

## Detailed execution contract

1. Input and target acquisition; distance, focus, line of sight and denial behavior.
2. Authoritative state transition, one-time IDs, ownership and cancellation.
3. World projection: actual meshes, components, collision, effects, UI and objective.
4. Save/load, new game, actor registration/stream-in and restart behavior.
5. Player camera, sound, animation and VFX consequences where required.
6. Failure paths, repeated action handling, regression risks and non-goals.
7. Exact edited paths and authorized operators.
8. Tool handoff format (Blender source, exported mesh, Unreal import destination).
9. Local reproducible build/tests and human acceptance route.
10. Final packaging and release integration, where the feature affects shipping.

## Test matrix to fill after execution

| Check | Required observation | Executor | Evidence | Actual result |
| --- | --- | --- | --- | --- |
| Source build | Target compiles/links | Assigned engineer | Build log | Record PASS/FAIL/NOT RUN |
| Native assertion | Real state/edge-case test | Assigned engineer | Named test + results | Record actual result |
| Editor integration | Real assets exist, compile and save | Sole Editor owner | Editor check/report | Record actual result |
| Human PIE | Player can finish critical behavior | Human reviewer | Playtest note/video | Record actual result |
| Reload/streaming | State is durable and replay-safe | Human/QA | Fresh-process evidence | Record actual result |
| Shipping | Feature survives real cook/package/launch | Release owner | Logs, executable check | Record actual result |

Only report `Human Validated` once the named reviewer accepts the required criteria. Document cuts and deferred checks instead of silently interpreting them as PASS.

## Reporting

Attach changed relative paths, commits if available, exact tool/model versions, failures corrected, unresolved risks, evidence location, handoff recipient and status gate.