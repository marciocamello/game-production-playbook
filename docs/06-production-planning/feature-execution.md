# Executable Feature Workflow — From SDD to Human Acceptance

A Feature Spec describes a **working end-to-end game behavior**, including integration, tests and human acceptance. It must not be reduced to an implementation readiness document unless readiness is explicitly requested.

## Feature contract

| Field | Requirement |
| --- | --- |
| Player outcome | Observable interaction and consequence inside the running game |
| Authority | Existing source of truth for state, abilities, progression and persistence |
| Reuse audit | Existing methods, services, Blueprints and assets already solving part of the problem |
| Scope/cuts | Explicit must-have result; out-of-scope features; dependencies |
| Source ownership | One writer per source path, asset and Editor/MCP session |
| Handoffs | Actual produced paths/formats, receiving operator and import/verification stage |
| Edge cases | Cancellation, missing assets, repeated input, load/retry, stream-in, failures and softlocks |
| Testing | Native, Editor, PIE, standalone and package expectations, each independently recorded |
| Human acceptance | Named reviewer, reproducible steps and criteria |
| Evidence | Source/version references, commands and logs, PASS/FAIL/BLOCKED/NOT RUN, remaining limitations |

## Execution graph

1. **Game Director:** approve behavior and priority.
2. **Spec owner:** inspect current implementation, author one consolidated executable contract, explicitly assign workstreams and owners.
3. **Source agent/engineer:** implement gameplay authority and relevant code tests. Do not claim test success unless executed.
4. **Art/DCC operator (when needed):** produce verified geometry, materials, rig, audio or animations. Preserve source files, license and exact export paths.
5. **Editor owner:** inspect existing loaded assets and dirty state, import/author real Blueprints, input assets, UI, level geometry and presentation. Do not edit binary Unreal assets by arbitrary filesystem modifications.
6. **Automated/Editor validation:** compile, validate, save/reload and record actual checks and failures.
7. **Human:** run manual gameplay test, observe physical traversal, interaction, timing, art and feedback, and accept or reject.
8. **Release owner:** cook/package and test standalone behavior as required.
9. **Documentation owner:** update specs, decisions and public logs from evidence (not from an optimistic agent summary).

The workstreams are **conditional**: SaveGame may need no 3D generator; a model export may need little C++. Parallelism is safe only with compatible interfaces, file ownership and exclusive Editor session handoff.

## Approval gates

**Planned → In Progress → Implemented → Tested → Human Validated → Released**

Report actual test cases independently. “Editor validated” does not satisfy “manual PIE tested”; “package produced” does not satisfy “packaged launch succeeded.” A failed required test blocks the relevant acceptance gate until resolved or explicitly cut by an authorized decision.

## Scope and time strategy

Build one true vertical slice first, then expand dependent systems. **Prioritize Save/Load, player controls, critical route, ending and an early shipping build before optional asset density.** For a game jam, time must be reserved for trailer/gameplay capture, legal credits, external downloads and manual submission.

## Handoff format

Each handoff report should give:

- Task ID, implementation branch/revision, executing agent/operator and owned paths.
- Inspected existing implementation and reused contracts.
- Actual created/changed files and integrated assets (source vs export vs Unreal content).
- Validation command/procedure, results, logs, known missing checks and reasons.
- Defects encountered, cause, correction and preserved dirty/third-party content.
- Current status and a **specific human acceptance route**.

See the reusable [feature specification template](../../templates/feature-specification.md) and [agent handoff template](../../templates/agent-handoff.md).