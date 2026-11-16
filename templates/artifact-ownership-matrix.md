# Template — Artifact Ownership Matrix and Editor Session Lease

**Use this for a real authorized production task.** The template records ownership decisions; it does **not** lock Git, Unreal, Blender or any MCP server. Human/operator coordination enforces the lease.

## Session header

- Game/Feature and approved spec revision:
- Current source branch/commit and local inspection date:
- Assigned Game Director and human acceptance owner:
- Documentation/Spec owner:
- Active Unreal Editor session (project name, Engine version, opened level):
- Current **sole** Editor write operator:
- Session lease began / next handoff recipient:
- Existing dirty or unsaved packages and known owner:
- Unknown ownership conflicts (stop writes to affected assets):

## Artifact and task matrix

| Task ID | Real outcome | Sole writer | Exact owned relative source/asset paths | Read-only dependencies / forbidden writes | Required output + recipient | Verification evidence | Status |
| --- | --- | --- | --- | --- | --- | --- | --- |
| Assigned ID | Player/asset deliverable | Named person/agent | Existing/approved actual paths | State/other owners | Real file/export/API | Named checks and logs | Planned / In Progress / actual result |

Do not invent a target path solely to fill the table. Audit the existing project; use actual paths when the task is authorized. Distinguish `Content/` disk files from Unreal package identifiers `/Game/...`, and distinguish source meshes from exported/imported assets.

## Exclusive Editor session lease

- Operator and tool/client identity:
- Correct Engine/project/map and MCP server identity verified by:
- Current map and selected Actors:
- Dirty packages owned and safe to save:
- Dirty packages **not owned** and preservation decision:
- Active long-running calls/imports/compiles:
- Allowed native Unreal assets and path namespaces:
- Explicitly forbidden packages and project settings:
- Receiver, date, and preconditions for transferring write permission:

**The Editor lease is a process convention, not an automated mutex.** Epic's MCP server serializes tool execution but does not resolve conflicting edits from two clients or safely merge binary Unreal packages.

## Handoff gate

1. Previous writer stops mutating calls.
2. Actual files/exports/interfaces exist and have been inspected.
3. Only owned validated packages are saved; unowned dirty packages remain untouched.
4. Owner provides exact changed paths, tool results, failures and remaining checks.
5. Receiver confirms project/session identity, baseline and exact accepted outputs.
6. Matrix transfers ownership **before** receiver starts any writes.
7. Director accepts, rejects or authorizes correction after real gameplay.

## Decisions and evidence

- Confirmed reused systems/assets:
- Dependency changes or blockers:
- Source/Editor/Blender tests actually run:
- Human PIE outcome: PASS / FAIL / BLOCKED / NOT RUN:
- Packaged-game outcome: PASS / FAIL / BLOCKED / NOT RUN:
- Known rights/licensing constraints:
- Current feature stage and evidence:
- Director decision and actual date:

The matrix belongs alongside the [canonical executable Feature Spec](feature-specification.md) and [single consolidated task handoff](agent-handoff.md). It does not replace either document.
