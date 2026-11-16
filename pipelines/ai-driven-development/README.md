# AI-Driven Game Production — Agent Orchestration

AI-driven means **tools and agents do actual production work**, not that AI suggestions alone count as implementation. The human remains the creative and product authority and reviews the real result.

## Reference role split

| Workstream | Example owner | Explicit boundary |
| --- | --- | --- |
| Direction, scope, gamefeel and acceptance | Human game director | Final authority; playtest and release |
| Story, GDD, TDD, SDD, Glass Box, backlog | GPT or equivalent | Publishes approved documentation and evidence, not fictional tests |
| Gameplay C++ / code tests | Codex, Claude or engineer | Modifies assigned source only; preserves existing APIs |
| Unreal Editor, Blueprints, levels, UI and VFX | One assigned Editor/MCP operator | Exclusive session and asset ownership; report exact authored content |
| Blender, rigs, animation, texture/mesh cleanup | Antigravity or another agent with real Blender access | Actual DCC operations, export QA and source provenance |
| Source asset generation | Tripo, Meshy or authorized provider | Asset produced, rights checked; never “integrated” by default |
| Independent audit | Different model/engineer when justified | Examines evidence and concrete code, not a second competing implementation |

Provider choice depends on capability, cost and available connector tools. Do not add a provider to the workflow simply because it exists.

## Essential controls

### One writer, one Editor owner

Before a task, inspect branch, working tree, existing code, Unreal content and unsaved editor packages. Assign one owner to each file/asset and only one operator to a shared Unreal Editor/MCP session. A second agent may work on a **separate** Blender source file while the C++ owner implements a non-conflicting game system; the Editor owner integrates at the handoff point.

When interrupted, preserve dirty packages. Unknown asset ownership is a **stop-for-that-asset** condition; do not reset, overwrite, delete or “repair” it without review. Avoid destructive Git actions.

### Minimum context

Agents load the constitution, task specification and only relevant skill/doc context; do not dump the entire repo into every prompt. Inspect real methods and plugins before suggesting new code. Identify each task's target engine version, tool capabilities and acceptance criteria.

### Consolidated prompts

When reviewing a task prompt before execution, revise the **single canonical prompt** instead of stacking conflicting addenda. Include source-of-truth files, exact scope, required behavior, owner/handoff, tests, limits and reporting.

### Reality gates

- Code review is not a build.
- A passing compile does not validate a Blueprint or a player's path.
- A generated mesh is not rigged or imported merely because it has a filename.
- An Editor screenshot is not a runtime camera capture.
- An Unreal cook/archive is not proof that the game starts or plays successfully.
- Only documented checks get PASS; unperformed checks stay NOT RUN.

## Anti-patterns

- Multiple agents simultaneously editing the same map/Blueprint.
- New generic frameworks where the engine already supplies the needed behavior.
- Agents modifying GDD/specs while another owner independently edits the same canonical docs.
- Publishing machine-specific command arguments or copied private logs.
- Marking a feature “done” because a prompt was written.
- Rebuilding already working gameplay to solve a visual-only problem.
- Spending external generation credits before visual references and asset briefs are approved.

## Alternate models

[Manual](../manual-development/README.md) and [AI-assisted](../ai-assisted-development/README.md) routes use the same evidence gates and design authority.

Related: [Feature Execution Workflow](../../docs/06-production-planning/feature-execution.md).