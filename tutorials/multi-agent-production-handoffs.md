# T04 — Multi-Agent Game Production, Artifact Ownership and Unreal Editor Handoffs

**Publication status:** Published operational tutorial. **No game implementation, Editor session, or local playtest was executed as part of publishing this document.**

**Applies to:** manual teams, AI-assisted teams, and AI-driven production. The reference workflow emerged from documented *The Unmade* production decisions and agent reports (Unreal Engine 5.9, C++, Blueprints, Blender and Unreal MCP). Adapt roles to actual people, tools, engine versions and project needs.

**Practical outcome:** turn one approved, end-to-end Feature Spec into **nonconflicting executable tasks**, run only the workstreams that have real dependencies, transfer source/art outputs to one active Unreal Editor operator, preserve unowned dirty packages, reconcile evidence and submit the real integrated feature for human acceptance.

**Do not** treat the agent's task report, a Git commit, a successful build or an Editor screenshot as proof of playable completion.

## 1. Trust boundaries and authoritative references

This tutorial builds on:

- [T03 — Concept, GDD, TDD and executable SDD](concept-to-gdd-tdd-executable-sdd.md)
- [Feature Execution Workflow](../docs/06-production-planning/feature-execution.md)
- [Existing handoff template](../templates/agent-handoff.md)
- [Artifact ownership matrix template](../templates/artifact-ownership-matrix.md)
- [The Unmade documented failures and fixes](../case-studies/the-unmade/failures-and-fixes.md)
- [Epic Games — Unreal MCP in Unreal Editor (UE 5.9)](https://dev.epicgames.com/documentation/unreal-engine/unreal-mcp-in-unreal-editor?application_version=5.9)
- [Git — git-status porcelain output](https://git-scm.com/docs/git-status)

**Source distinction:** the role boundaries, one-Editor-operator practice and case-study failure handling are production policies and observed report content. Epic documents the **technical MCP execution mechanism**, not an automatic distributed agent lock. Git documents repository state, not Unreal's unsaved Editor packages. The exclusive-owner rules below are **process safeguards**, not built-in transactional guarantees.

### Critical safety rules

1. **One writer per file, Unreal package, Blender source or generated export at a time.** Separate branches do not remove conflicts if both edit the same authored asset.
2. **One active writer/operator for each Unreal Editor/MCP session.** Multiple read-only clients are possible but their access must be carefully scoped. Do not permit a second agent to mutate the map while the first is working.
3. **The human director owns creative acceptance, scope cuts and release.** Agent self-approval cannot replace actual player observation.
4. **The canonical documentation owner** updates GDD, Story, TDD, Feature Specs, backlog and factual development logs from reports. Implementation operators do not independently rewrite them without authorization.
5. **Inspect existing systems first.** Extend working services, Gameplay Abilities, Blueprints, assets and materials rather than adding duplicate managers.
6. **Never directly modify `.uasset` or `.umap` binary bytes.** Use supported Unreal Editor/MCP/Python/Blueprint workflows and verify the actual package was saved.
7. **No destructive repository actions** without explicit authorization: no blind resets/cleans, silent asset replacement, force-push or unrelated repository edits.
8. **No stubs, fake assets, fabricated success, unexecuted tests labelled PASS, or GitHub Actions.**
9. **No private host information** in public instructions/reports: keep path references repository-relative and sanitize logs/screenshots before publication.
10. **Commercial tool/asset use requires authorization and provenance**, even if the work is agent-operated.

## 2. Decide what parallel execution really means

A large feature does not need every possible agent. Decompose it into **real artifacts**: gameplay state and interfaces, approved asset sources, Editor integration, QA, documentation, and release-specific verification.

### Recommended responsibilities (roles, not required brands)

| Responsibility | Default accountable owner | Typical artifacts and limits |
| --- | --- | --- |
| Game Director | Human | Approved scope, design choices, playtest observations, accept/reject, release |
| Documentation / specification owner | Assigned writer or LLM | One canonical GDD, TDD, Feature Spec, decisions and factual status |
| Gameplay engineer | Engineer or code-capable agent | Existing code modules, real tests, public interfaces and build evidence |
| 3D authoring / Blender operator | Artist or authorized agent | Editable meshes, rigged sources, animations, export QA, provenance |
| Optional generator operator | Assigned human/agent | Actual source mesh/texture/audio output with licensing; **not** engine integration |
| Unreal Editor operator | One authorized person/agent at a time | Actual Blueprints, scene, materials, animation, Niagara, UI and saved packages |
| Independent reviewer | Designated person/agent | Read-only audit of diffs, asset lists, logs and explicit issues |
| Human gameplay tester | Game Director or formally delegated tester | Actual PIE/standalone observed player route and acceptance |
| Release operator | Assigned owner | Local package, executable verification, upload performed under authorization |

In the historical *The Unmade* setup, GPT authored canonical SDD and documentation, Codex primarily authored C++, Claude primarily operated Unreal Editor, and Gemini/Antigravity/Blender handled bounded audit or visual work. Those vendor assignments were project-specific; **the transferable rule is capability + ownership + evidence**, not provider loyalty.

### Safe and unsafe parallelism

| Pair of tasks | Parallel? | Why |
| --- | --- | --- |
| C++ state system ↔ isolated Blender mesh source | **Yes, if contracts approved** | Different editable files; agree on interface, scale and asset ID before handoff |
| Different artists on different Blender source files | **Yes** | Ownership and export output paths remain disjoint |
| Code audit ↔ separate code implementation | **Read-only audit only** | Auditor must not edit the same source while implementation is active |
| Blueprint/material authoring ↔ level layout edits in the **same running Editor** | **No** | One operator per Editor session; common package/dirty state |
| Two agents editing one `.umap`, Blueprint or Material Instance | **No** | Git branches do not make binary Unreal conflict merges safe |
| Two agents updating the canonical GDD/Feature Spec | **No** | Conflicting document authority; revisions must be consolidated |
| C++ API change ↔ Blueprint hookup using the **old interface** | **Only after a stable handoff** | The Editor owner needs an actual compiled and documented contract |
| Generator output ↔ Engine integration | **Only after export gate** | A generator request or concept image is not an existing usable mesh |

Do not confuse concurrency with productivity. Parallel execution is beneficial only when dependencies permit it and integration cost is lower than saved time.

## 3. Model the work as a dependency graph, not a provider queue

Consider an **illustrative authorized landmark feature**: collect a mechanism part, perform a voluntary sacrifice, restore a crossing, and see a real, traversable level change.

```text
Approved GDD / TDD / Feature Spec
                 |
       Ownership + reuse preflight
           /             \
  Gameplay authority   Approved art brief
           |             |
  C++ source + tests   Blender source/export QA
           \             /
         Integration contract
                 |
       Sole Unreal Editor owner
         (import, wire, save)
                 |
         Technical verification
                 |
        Human PIE acceptance
                 |
      Local package/standalone
                 |
      Docs + evidence reconciliation
```

The art track exists **only if necessary**. If functional level geometry already exists or Unreal Modeling Mode is more appropriate, skip external generation. If the feature is SaveGame-only, there may be no Blender handoff at all.

### Explicit gate sequence

- **G0 — Design approved:** approved player behavior, failure modes, minimum complete route and priorities.
- **G1 — Ownership assigned:** code/art/Editor/document paths, task owners, dependencies and tool access verified.
- **G2 — Source contracts delivered:** actual C++ interfaces and exported asset files with QA and license records exist.
- **G3 — Editor integrated:** all required Unreal Actors/Blueprints/assets are authored and saved, and related checks have real results.
- **G4 — Human validated:** intended player behavior is demonstrated in real gameplay; any required failure/recovery cases are accepted.
- **G5 — Packaged/released:** separate standalone build and publication checks are executed when needed.

**Status is not a single unconditional checkbox:** after G3, a feature can be Implemented but still have explicit FAILED or NOT RUN runtime checks.

## 4. Prepare the ownership matrix before delegating

Use the [artifact ownership matrix](../templates/artifact-ownership-matrix.md), recording **exact assigned writable paths**, not broad ownership such as “Claude can change everything in Content.”

For each line record:

- Task ID and the **specific player/asset result**.
- Responsible writer and reviewer; **one writer** for each package/file.
- Branch or local work context and actual starting revision.
- Exact owned relative paths, including native Unreal content package identifiers where appropriate.
- Read-only dependencies and excluded paths.
- Output/handoff format: callable C++ API, actual FBX/GLB, `.blend`, reimport report, `.uasset`/Map, QA results.
- Entry/exit gate, recipient, current editor owner and deadline/budget constraints.
- Real status and evidence; unknown state remains **NOT RUN / BLOCKED**, not assumed clean.

### Distinguish the three ownership domains

**Code owner:** an actual module/source set and its automated checks. Does **not** silently edit the same Blueprint the Editor operator is modifying.

**DCC owner:** approved source meshes and exports. Does **not** declare imported Unreal assets completed.

**Editor owner:** the *running Editor process* and the Unreal native asset set for the session. **Must** check existing dirty packages, loaded map, current selection and content changes before touching the scene.

A `Content/` folder may contain externally added, untracked or ignored assets. **Git remote status cannot prove that these do not exist.**

## 5. Preflight: Git is necessary, but insufficient

Before every new ownership assignment, inspect the **actual local checkout**. The companion script in [`examples/multi-agent-handoff/`](../examples/multi-agent-handoff/README.md) performs a **read-only Git preflight**. It does not alter Git state, create branches, write files, or query the Editor.

From the repository containing the real game:

```powershell
pwsh -File ./examples/multi-agent-handoff/scripts/Inspect-GitPreflight.ps1 `
  -Repository .
```

When the inspection script is kept in a separate documentation checkout, pass `-Repository` pointing to the **actual game checkout**.

The script reports:

- Git HEAD and current branch (including detached HEAD when applicable).
- Human-readable `git status --short --branch --untracked-files=all`.
- Staged and unstaged name/status diffs.
- Ordinary untracked files (not ignored files).
- Whether the available Git commands succeeded.

**Separate mandatory manual/Editor preflight:** inspect ignored files, real Engine Content and unsaved packages. Git cannot see an unsaved map edit held only in memory, and ignored assets may not appear in ordinary `git status`. If any ownership is unknown, stop **writes to that asset**, preserve evidence and request the current owner's decision.

### Editor/MCP preflight

Before taking the active Editor session:

1. Confirm the intended **project, Engine build and map**, including multiple-Editor/port identity where relevant.
2. List available MCP tools using **actual discovery schemas**; do not assume a tool exposes package dirty state if it does not.
3. Ask the current operator for a **dirty-package inventory** and recent saved/unsaved actions.
4. Inspect scene Actors, materials, Blueprints and any already created asset with the requested ID.
5. Compare ownership matrix and feature spec with the **actual editor content**, not only file names.
6. Verify other mutating clients are idle; an MCP server running commands serially **does not enforce human task ownership**.

**Stop-for-asset condition:** conflicting edits, unowned dirty packages, unknown deletions, a mismatched Editor project or an unresolved existing implementation. Preserve the current state rather than doing cleanup by assumption.

## 6. Make one consolidated assignment per workstream

An assignment must be **specific enough for an operator with real tool access to implement**. Use the existing [handoff template](../templates/agent-handoff.md) rather than inventing a second format.

### Required task packet

| Field | Required content |
| --- | --- |
| Objective | Observable gameplay state, imported game asset, Editor result or measured quality improvement |
| Approved sources | Exact current Feature Spec, relevant GDD/TDD decisions and baseline revision |
| Existing systems | Functions/classes/Blueprints and known asset IDs already solving part of this task |
| Owner and session | Assigned writer, allowed read-only reviewers and exclusive Editor operator |
| Allowed writes | Exact repository-relative paths/packages, no broad unspecified folder access |
| Exclusions | Owned work of other agents, unrelated private repositories, Engine source, unapproved plugin/config changes |
| Input contracts | APIs, filenames, exports, scale/pivot conventions, required licenses |
| Deliverables | Real source/asset, integration step and receiver; no stubs or fake assets |
| Tests | Which commands/Editor queries/reimport/PIE/packaging checks **must** run and by whom |
| Failure behavior | Stop conditions, preservation, escalation and no-destructive-repair requirement |
| Report | Actual edits, logs, failures, remaining NOT RUN, evidence and precise handoff |

If the spec changes **before execution**, rewrite the **same single consolidated instruction**. If it changes **during execution**, stop affected work, reconcile owner/output contracts, then resume from a deliberately revised baseline rather than stacking contradicting messages.

### Bound context, not exhaustive context

Load the project constitution, approved feature spec, the relevant technical owner files and **only the matching skills**. Loading entire codebases and unrelated projects into every agent risks stale APIs, wasted quotas and unintended source disclosure.

## 7. Execute source and art independently; hand off concrete artifacts

### Gameplay/code operator

The engineer/agent must:

1. Inspect the actual current classes and tests; find the existing service or method to reuse.
2. Implement real player-facing authority, state transition, safe cancellation/repetition and failure behavior in the assigned source only.
3. Run only focused relevant source builds/tests permitted by the project; report exact commands, failures and exit results. A test process exiting zero does not automatically mean all cases passed.
4. Deliver callable interfaces, state semantics, revised source paths and known asset/Blueprint requirements.
5. **Stop before editing Editor-owned packages**, unless ownership is formally transferred.

### Blender/asset operator, when needed

The artist/agent must:

1. Confirm an approved reference and asset brief.
2. Model or repair genuine geometry/rig/animations as required, preserving `.blend` and source provenance.
3. Verify units, pivot, mesh normals, UVs, material slots, skeleton/rest pose and animation export as applicable.
4. Export **real files** to the approved receiver path, recording source/export formats and licensing.
5. Distinguish **source created → export verified → imported → integrated → runtime tested**.

The Editor operator receives **actual existing exports**, not a prompt or a claimed model creation. A source file on disk is not proof that a cooking/import tool will accept it.

### Handoff gate G2

The Editor operator confirms that code contracts are stable and required exports actually exist. If an interface or export is missing, report **BLOCKED** with the precise dependency and return ownership to the responsible operator. Do not create placeholders to move the task to the next column.

## 8. Perform a safe Unreal Editor/MCP session transfer

**Only one person/agent performs writes in the session at a time.** Epic's official Unreal MCP server executes tool calls serially on the Editor game thread; this does **not** automatically prevent two clients from alternating conflicting writes. Treat session ownership as a separate process contract.

### Owner-to-owner transfer protocol

1. **Previous operator stops mutating calls.** Confirm no running import, material compile, asset save, UBT or unrelated editor automation will continue writing.
2. **Current operator lists all dirty packages**, opened level and pending changes. Split them into: owned and ready to save; owned but unfinished; **unknown/unowned**.
3. **Save only approved owned assets** through supported Editor APIs after validation. Do not use global Save All without checking the package inventory.
4. **Preserve unknown dirty assets** untouched; report their identifiers and uncertainty. Do not discard changes, delete/reimport assets or reset them.
5. **Record known source and asset diffs**: exact relative path/package, Actor labels/IDs when relevant, expected behavior and downstream dependency.
6. **Transfer exclusive write authority** to the receiving operator in the ownership matrix, with time/revision and the specific next task.
7. **Receiving operator confirms Editor identity**, current map, toolsets, dirty packages and assigned scope before writing.
8. **Integration owner runs real Editor checks**: import/compile, material assignments, Blueprint calls, actor placement/collision, save/reload of affected assets.
9. **Receiving operator reports results** and returns to the human for manual PIE and to the documentation owner for factual status reconciliation.

If the Editor must restart, preserve and settle known unsaved work **before** restart. A tool option to discard changes is **not a standard recovery procedure**. The active operator has no automatic permission to alter another operator's work.

### File-system and Git protection

Saving a `.umap` or `.uasset` changes binary content. Avoid concurrent tasks on the same package even if Git branches differ. Never “repair” a conflict with binary byte edits. Compare asset IDs, last saved revision, Editor state and ownership; choose a responsible person to reconcile, keeping a backup when required by the real game's storage policy.

## 9. Work through a concrete multi-agent feature (educational scenario)

**Scenario:** a functional fountain must be repaired by installing a real mechanism part, followed by a voluntary Light gift. The authoritative gift removes the player's Light capability and opens a physical route. A later fragment restores personal Light without resetting the fountain.

The following assignment pattern demonstrates **how to execute** a feature **after a real Game Director approves it**. It is an instructional scenario, **not a claim that this playbook has implemented the feature**.

| Task | Writer | Exclusive outputs | Entry gate | Exit evidence |
| --- | --- | --- | --- | --- |
| FLOW-01 | Documentation owner | Existing `specs/FLOW-01/spec.md` | Approved GDD/TDD | One consolidated executable contract and human execution authorization |
| FLOW-02 | Gameplay engineer | Actual game's `Source/<GameModule>/` files owned by task | Approved spec + existing code audit | Real interaction/state code, concrete C++ API, actual focused tests |
| FLOW-03 | Blender operator, **only if required** | Distinct editable source `.blend` and exports under agreed art domain | Approved visual asset brief | QA'd mesh/export with scale, pivot and rights |
| FLOW-04 | Sole Editor operator | Explicit fountain/gate/character packages and authorized level | FLOW-02 API stable + required art present | Real native assets imported, BPs/events wired, level saved and inspected |
| FLOW-05 | Human tester | Manual PIE observation record | FLOW-04 Editor checks | Actual collection/donation/recovery/traversal result |
| FLOW-06 | Release owner | Unique local package destination | Accepted slice and required config | Real cook/package, standalone launch and any remaining defects |
| FLOW-07 | Documentation owner | Canonical spec and factual development log | Reports and human decisions | Correct stage and evidence, no invented tests |

**Parallel permitted:** FLOW-02 and FLOW-03 when source files/IDs are disjoint and approved. **Serialized:** FLOW-04 Editor ownership and FLOW-05 actual human playtest. No generated image, Blueprint subclass or test-only object substitutes for the functional fountain.

The [worked packet and matrix](../examples/multi-agent-handoff/README.md) includes the actual handoff text structure and sample outcome labels without fabricated implementation evidence.

## 10. Verify separately: source, Editor, gameplay and Shipping

| Layer | Required evidence | Not implied |
| --- | --- | --- |
| Source work | Actual source diff, relevant build and named test results | Real in-world placement |
| Blender/asset | Existing source/export files, reimport QA and rights | Engine integration |
| Editor | Authored `.uasset`/`.umap`, compile/save/reopen checks, asset IDs | Human PIE |
| Human PIE | Actual player input, response, collision, camera, safe progress | Packaged runtime |
| Persistence | Save/load including fresh process and streaming behavior | Every corruption/failure mode |
| Package | Real UAT output, executable present and standalone launch tests | All player routes or public download |
| Release | Actual uploaded artifact, permissions and verified download | Unmeasured hardware coverage |

Record **PASS** only for a performed check whose assertions passed. **FAIL** records an observed defect, **BLOCKED** records a prerequisite preventing a check, and **NOT RUN** means nobody performed it. Keep the feature stage `Planned → In Progress → Implemented → Tested → Human Validated → Released` distinct from each test's result.

### Independent audit without conflicting edits

A separate reviewer can inspect source diffs, code paths, ownership matrix, logs or saved game reports **read-only**. Its report identifies exact findings, files and reproduction criteria. It must not silently become a second implementer of the same code/Editor asset. When a reviewer finds an issue, the responsible current owner executes the fix and reruns affected checks.

## 11. Real failure patterns from The Unmade

The reviewed historical reports provide specific examples of why these controls matter:

- **Unknown dirty packages.** During a later environmental material pass, several material/texture packages already had unsaved changes with uncertain ownership. The operator preserved them rather than overwriting them. The mere existence of a dirty package did not authorize save or reset.
- **Unexpected scene changes.** One integration pass found actor deletions outside its scope and excluded them from the edited files. The operator did not fold them into an unrelated environment correction.
- **Editor and compile evidence do not equal gameplay.** An environment correction report described actual material/shader and geometry changes, but clearly marked artistic acceptance, runtime profiling, manual PIE and Windows package **NOT RUN**.
- **Art handoff scale and compatibility.** Character export reports found mismatched skeleton rest poses and animation-only translations imported at an incorrect scale. Export success was insufficient; the Editor owner performed an explicit corrective import/preview review.
- **Toolset gaps.** When a specific AnimGraph authoring capability was unavailable through the installed MCP route, the operator recorded the limit rather than claiming that graph creation succeeded.

These observations are drawn from private production evidence and paraphrased here with **no raw private logs, local machine paths, scene dumps or source assets**. See the published [case-study failures](../case-studies/the-unmade/failures-and-fixes.md). Historical reports are not independently repeated gameplay tests.

## 12. Stop conditions and recovery decisions

| Condition | Correct action |
| --- | --- |
| Wrong project/Editor/server instance | Stop writes; verify identity and connection |
| Two active mutating Editor clients | Suspend one and confirm the exclusive owner |
| Existing unowned dirty packages | Preserve them; escalate to responsible human |
| Source/API changed after handoff | Pause dependent Editor work; reconcile and recompile/retest |
| Missing export/asset or unclear license | Mark BLOCKED; do not fabricate a proxy asset |
| Generated mesh does not import or rig correctly | Return to Blender/export owner with exact failure |
| Failed required native/Editor/PIE test | Keep FAIL; assign fix owner and rerun affected checks |
| Unexpected Git changes or local untracked assets | Inspect and preserve; do not auto-clean or reset |
| Tool capability unavailable | Discover supported fallback, adjust owner/scope, document limitation |
| Feature exceeds deadline or credit budget | Director explicitly cuts/defer nonessential work; preserve critical route |
| Required test unperformed | Mark NOT RUN; do not infer PASS from other evidence |

**Recovery is a controlled handoff, not automatic deletion.** It should never overwrite unknown assets or quietly bypass the Feature Spec's player outcome.

## 13. Human acceptance and documentation closeout

A feature is **Human Validated** only after the authorized reviewer plays the actual integrated behavior and approves the observed result. A compile, verified import, Editor screenshot, asset count or packaged archive cannot self-promote to that status.

Once reports arrive, the documentation owner:

1. Checks source/edit reports against actual files and the approved contract.
2. Distinguishes implemented, automated-tested, Editor-checked, human-tested and standalone-tested outcomes.
3. Records failures and unrun steps with the owner responsible for follow-up.
4. Consolidates approved design changes into the **existing** GDD/TDD/spec.
5. Records model/tool usage, generated asset provenance, rights, decisions and scope cuts where applicable.
6. Publishes only sanitized material authorized for public use; never mirror a private game repository.
7. Asks the director to accept or reject the **real** feature, not a readiness report.

### Completion criteria for T04 as a reader's practice

The T04 exercise is done when a **real approved feature** has an ownership matrix, actual workstream owners, concrete asset/API handoffs, one verified Editor operator, non-destructive treatment of dirty packages, real Editor integration and an evidence report that distinguishes every check. If actual execution was not performed, the exercise remains NOT RUN; the **documentation itself** can still be published and studied.

**Next tutorial:** [T05 in the publication roadmap](README.md) covers gameplay C++, Gameplay Tags and GAS with a genuine input-driven, durable ability-state change.
