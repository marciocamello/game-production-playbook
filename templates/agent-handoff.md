# Template — Contributor / Agent Handoff

Use this for a real editor, programming, art or QA task. It is not an autonomous authority to change arbitrary files.

## Required task definition

- Task and requested player-visible or artist-visible outcome.
- Game Engine/tool versions actually installed or verified.
- Source-of-truth GDD/TDD/spec and specific current files to inspect.
- Single owner of each file, model, asset or package.
- Exclusive Unreal Editor/MCP session owner, if relevant, and the recipient of the next handoff.
- Exact **repository-relative** paths permitted for reading and writing.
- Actions explicitly disallowed (unrelated edits, binary asset byte modifications, destructive Git, unspecified scope expansion).
- Expected concrete outputs and acceptance checks.
- Approval to use commercial credits or external services, if applicable.

## Execution report required from owner

1. Starting branch/revision and preflight, including existing dirty packages.
2. What existing functionality was **reused**.
3. Actual created or changed code/assets and their path.
4. Exports/imports and comparison/reimport results.
5. Commands/tests executed, real outcomes and matching log paths.
6. Failures, blockers, tool limitations and fixes; never hide test failures behind an exit code.
7. Human PIE steps to run and unresolved runtime checks.
8. Preserved user work and unsaved Editor/Blender packages.
9. Status: Planned / In Progress / Implemented / Tested / Human Validated / Released, with evidence.
10. Document owner/reviewer responsible for recording the handoff.

## Conflict and interruption

If another agent owns the same asset or the Editor contains unowned unsaved changes, **stop edits to that asset**, preserve the state and report the conflict. Do not overwrite, recreate, reset, force-push or claim a clean workspace without inspection.

A revised instruction is delivered as **one consolidated task**, not scattered contradictory fragments.