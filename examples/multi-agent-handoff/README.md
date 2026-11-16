# Practical Example — Multi-Agent Task Ownership and Safe Editor Handoff

This example accompanies [T04 — Multi-Agent Game Production](../../tutorials/multi-agent-production-handoffs.md).

**What is real here:** the read-only Git preflight script in [scripts/](scripts/) executes real Git commands on an explicitly selected existing checkout, without mutating the repository.

**What is illustrative:** the role split and text below demonstrate how to authorize and transfer tasks for a possible game feature. They are **not** an implemented game or a record of tests run. Actual classes, asset paths, ownership and acceptance can only be filled after inspecting an actual game project.

## 1. Inspect the actual local repository

From a directory containing the Playbook's example script, run:

```powershell
pwsh -File ./examples/multi-agent-handoff/scripts/Inspect-GitPreflight.ps1 -Repository .
```

If your game is in another repository, set `-Repository` to that checkout's path **only when you have read access to it**. No location is hardcoded in this example.

The script uses Git's documented `--no-optional-locks` mode to avoid incidental index-refresh locking and shows the current checkout's HEAD, branch, porcelain status, staged/unstaged changes and ordinary untracked files. It **does not inspect ignored files, unsaved Unreal packages, non-Git source assets, Editor identity or MCP tool capabilities**. The sole Editor operator must inspect those separately.

## 2. Assign a real, bounded feature

The example feature is a landmark restoration with a real progression consequence. Do not send this task to an agent unless the game's own approved GDD/TDD/Feature Spec supports it.

| Workstream | Assigned owner | Allowed writes | Explicitly excluded |
| --- | --- | --- | --- |
| Design/SDD | Documentation owner | **Existing** canonical spec and approved decisions | Game source and native Editor assets |
| Gameplay authority | Source engineer | Exact existing gameplay source and relevant tests named in the approved task | Level, Blueprint and art source files |
| Asset preparation (optional) | Blender operator | Approved model source/export IDs only | Unreal binary assets and gameplay code |
| Level integration | **Sole** Unreal Editor operator | Approved gameplay Actors, packages and level only | Unknown dirty content and other owners' files |
| Validation | Independent reviewer (read-only) + human tester | Reports/evidence only | Modifying the target during another owner's lease |
| Documentation closeout | Documentation owner | Canonical spec/log after actual reports | Inventing missing tests or publishing private source |

## 3. One consolidated implementation assignment

Use the existing [handoff template](../../templates/agent-handoff.md) with actual files and tools discovered in the target game. The following wording is an instructional **pattern**, not an autonomous assignment to any connected tool:

> Implement the approved player-visible behavior described in the current canonical Feature Spec. Read the current source and inspect existing implementation before creating any class or asset. You are the sole writer of the exact paths assigned to your workstream; do not change any other operator's files or native Unreal packages. Report actual source changes and callable interfaces, any source/export artifacts, commands and tests executed, failures, and precisely what the next owner needs. No stubs, fake assets, speculative API names, destructive Git operations or GitHub Actions. The active Editor/MCP session is reserved for the assigned Editor operator, not for concurrent agents. Hand off only actual existing artifacts, and mark any unperformed checks NOT RUN.

For a real run, replace every abstract role/path with inspected names and the **existing approved task's specific acceptance criteria**. Do not split later changes into detached prompt fragments; update the same task packet.

## 4. Transfer the session

The current Editor writer finishes or deliberately stops a bounded operation, inventories dirty packages, saves **only approved owned packages** and documents the loaded map. A new writer first confirms the correct Editor, Engine version, current map, actual toolset schemas and the status of unsaved assets. Unknown assets are preserved, not reset.

**No lock is acquired by this Markdown example:** explicit human/operator handoff is required.

## 5. Report the actual outcome

Record factual results separately for C++ build, Blender export, Unreal imports/Blueprints, manual PIE, fresh-process Save/Load and packaged game. The source operator cannot mark the feature Human Validated merely because C++ tests passed. The editor operator cannot mark the feature complete because a screenshot looks correct.

Use [Artifact Ownership Matrix](../../templates/artifact-ownership-matrix.md), [Agent Handoff](../../templates/agent-handoff.md), and [Test/Acceptance Plan](../../templates/test-plan.md).

## References

- [Official Unreal MCP 5.9 documentation](https://dev.epicgames.com/documentation/unreal-engine/unreal-mcp-in-unreal-editor?application_version=5.9)
- [Git status documentation](https://git-scm.com/docs/git-status)
- [Git global options and `--no-optional-locks`](https://git-scm.com/docs/git)
- [The Unmade observed engineering failure patterns](../../case-studies/the-unmade/failures-and-fixes.md)

No third-party code or Unreal assets are redistributed by this example. The read-only script is original educational material under [MIT](../../LICENSE-CODE.md).
