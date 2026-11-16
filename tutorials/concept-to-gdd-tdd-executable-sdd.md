# T03 — From Game Concept to GDD, Technical Design and Executable SDD

**Status:** Published educational workflow. This tutorial helps a reader author and approve **real production documents** that direct actual implementation; publishing the tutorial does **not** mean an illustrative feature was built or playtested.

**Applicability:** Manual development, AI-assisted production and AI-driven execution. Engine-agnostic method; the worked example refers to Unreal Engine 5.9 and the historically approved design of *The Unmade*.

**Outcome:** transform a candidate concept into a constrained **project charter**, approved **Game Design Document (GDD)**, coherent **Technical Design Document (TDD)** with architecture decisions, and one **executable Feature Specification (SDD)** whose required delivery includes gameplay implementation, real Editor integration, tested state transitions, human playtest and packaging where applicable.

**Prerequisite:** an authorized project owner, a real game idea, and enough knowledge of the project repository/engine state to identify existing functionality. Do not start a feature implementation merely because an assistant drafted a design.

## 1. The four separate authorities

| Artifact | Owns which questions? | What approval establishes | What it does **not** establish |
| --- | --- | --- | --- |
| **Project charter** | Who plays, why this game, platform, deadline, budget, constraints, scope boundaries | One project concept has been selected within actual limits | That the game loop is enjoyable |
| **GDD** | What the player does, feels, learns and experiences; progression and fail states | The player-facing behavior and creative direction | That the systems have been implemented |
| **TDD / decisions** | Which systems own state, how they interact, when data survives and which tools execute them | A consistent engineering/integration contract | That APIs, assets, builds or runtime behavior have passed |
| **Executable Feature Spec** | Who implements the approved behavior, with which assets/interfaces, edge cases, tests, and acceptance | Authorization-ready **end-to-end execution contract**, once the director explicitly approves work | That an agent has already written code, authored assets or completed manual PIE |

The **canonical files must have one current owner**. When player behavior changes, revise the approved GDD, reconcile TDD consequences, then update the *existing* Feature Spec before sending any implementation instruction. Do not create competing versions or split an active task into contradictory prompt fragments.

The word *specification* is **not** synonymous with *readiness*. A Feature Spec describes the real playable result and all necessary workstreams; it is only **Planned** until real execution starts.

## 2. Begin with constraints, not a list of technologies

Before choosing GAS, Blender, a generative model or a level tool, establish a genuine project brief:

1. **Audience and promise:** What should a first-time player recognize as compelling in the opening two minutes?
2. **Core interaction:** What can the player do that transforms their next decision?
3. **Target:** Windows, consoles, mobile, browser or another real destination; supported input and performance goals.
4. **Experience boundary:** solo/multiplayer, session length, narrative scope, accessibility baseline, content/rating requirements.
5. **Hard limits:** release date, production hours, engine/tool access, subscriptions, external asset rights, team composition and deployment rules.
6. **Differentiation:** compare alternative ideas using actual novelty, gameplay clarity, asset requirements and cost.
7. **Acceptance owner:** The human responsible for concept selection, scope cuts and final game acceptance.

A feasible concept is one that can be **demonstrated with a playable vertical slice**. “Restore a city” is a world premise; “the player gives up a working capability to repair a fountain and thereby opens a traversable path” is a testable interaction.

Use [Project Charter](../templates/project-charter.md) for the approved decision. Reject or defer other concepts with their reasons; avoid recording speculation as human approval.

### Jam versus non-jam constraints

For a competition, cite the **current official rules** and note what may be prepared before the start, when game creation is permitted, video and submission requirements, the age rating and human submission responsibility. For a non-jam game, replace those checks with store, privacy, localization, accessibility, platform and commercial-license obligations. Historical competition rules do **not** automatically apply to future events.

**Gate C0:** the project owner has approved the premise, target, hard limits and minimum playable loop. This is a design approval, not a game implementation.

## 3. Author a useful GDD without inventing an entire feature universe

The [GDD template](../templates/game-design-document.md) is a **living creative contract**. Focus on the parts that make the game playable and complete:

- One-sentence vision, theme and intended emotional transformation.
- Player perspective, verbs, input/camera assumptions and controls/help.
- **Core loop:** explore → choose/act → state cost or reward → world consequence → next viable goal.
- System rules: eligibility, repeated actions, losses, recovery and irreversible decisions.
- Narrative start, turning points, ending, and the minimum complete player route.
- World/level regions and their dependency graph; where the player can go after each choice.
- Visual/audio identity and interaction readability at the real gameplay camera.
- Required UI, Settings, Save/Load, accessibility and onboarding.
- **Must-have / optional / cut** categorization and explicit anti-softlock requirements.
- Open design decisions versus approved behavior; expected playtest questions.

A GDD is not a one-time marketing description. It should distinguish an **approved mechanic** from a **tuning value awaiting a playtest**. It should also specify how optional content can be cut without invalidating the ending.

### Historical example: The Unmade

The source game's original GDD (approved November 12, 2026) established a small incomplete creature, an elevated camera and a Gothic city restored through meaningful sacrifice. Its minimum loop was **find a physical missing part → voluntarily give a personal capacity → restore a functional landmark → gain access or adapt**.

Three facts are **distinct gameplay rules**, not interchangeable prose:

1. The fountain requires a separate mechanical part **before** the Light donation.
2. Personal Light is donated voluntarily, becomes unavailable to the character, and **continues powering the restored fountain**.
3. A later **different recovery fragment** returns personal Light **without undoing** the fountain or closing the route.

The human-approved game direction also contained a subsequent Impulse donation/crossing, a final heart gift, revival and a brief playable epilogue. Combat, runtime LLMs, procedural city generation and many extra mechanics were outside the compact core.

These are **historical design decisions**, not assertions that all the described scenes were built or human-validated.

### Express the player's dependency graph

A compact dependency chain for that example:

```text
Move/inspect
  -> Find missing mechanism part
  -> Install part in fountain
  -> Confirm voluntary Light donation
  -> Commit permanent fountain restoration + remove personal Light
  -> Open previously blocked, traversable route
  -> Find distinct Light recovery fragment
  -> Recover personal Light; fountain remains restored
  -> Reach next objective
```

For every edge ask: What happens after input cancellation? On repeat interaction? After a reload or streamed Actor recreation? What if the player has already donated their needed ability? If these questions have no safe answer, the design is incomplete.

**Gate C1:** human approval of core loop, complete minimum narrative route, scope cuts, required interactions, recovery and softlock safeguards. Mark unknown tuning values for testing.

## 4. Translate the approved GDD into a real TDD

The [Technical Design template](../templates/technical-design.md) focuses on *authority and lifecycle*, not class-name decoration.

Begin with **a reuse audit of the real repository**:

- Source modules, existing classes, gameplay systems, tests and plugin dependencies.
- Blueprints and asset content actually present in the Editor or disk; remote Git cannot establish locally ignored assets.
- Active maps, streaming architecture, input assets, C++ / Blueprint boundaries, dirty packages, existing SaveGame and UI.
- Engine/SDK/tool versions; read relevant installed headers or public docs rather than guessing reflected API signatures.
- Existing local build/package scripts and previous verification results.

Do not propose a new `Manager` if an existing subsystem, component or Engine facility already owns the needed responsibility.

### TDD example: authoritative state versus presentation

The Unmade approved Unreal technical design had these responsibilities:

| Responsibility | Example authority | Contract |
| --- | --- | --- |
| Player input and possession | Character/PlayerController | Identifies action and safe interaction request |
| Gameplay eligibility and capabilities | Character-owned GAS ASC and Gameplay Tags | Executes a usable ability; reports capability state truthfully |
| Durable in-memory progression | GameInstance-level Run Subsystem | Sole owner of donated capacity, landmark restoration and once-only IDs |
| World projection | Fountain/gate/recovery Actors | Applies progression to mesh, collision, availability and objective |
| Persistent snapshots | Versioned SaveGame records | Saves logical IDs/states; no live streaming Actor pointers |
| UI and feedback | UMG/MVVM, audio, Niagara, animation, Sequencer | Observes committed state; **never independently decides progress** |

Do **not** automatically impose this exact architecture on every engine or game. Choose the minimum reliable design for the actual behavior and existing project, then record the trade-off.

### Design the state transition explicitly

The approved Light example separates:

```text
Personal Light: Owned -> Donated -> Recovered
Fountain:       Broken -> Repaired -> Restored
Route:          Blocked -> Traversable
Mechanism part: Available -> Collected -> Installed
Recovery item:  Available -> Consumed
```

The successful transaction must commit **once** to the authoritative progression owner. Only after committing should runtime Actors reconcile appearance and collision, GAS availability change, UI refresh and one-time effects play. Incorrect or duplicated interactions must not partially restore the world or consume two items.

For a streaming world, Actor initialization reads **current logical state** and applies it idempotently. A restored fountain remains restored after reload; **loading a map must not replay a one-time donation animation or award a recovery fragment again**.

Avoid hardcoding invented private code symbols into a generic tutorial: the exact class implementation is an engineering choice validated against the reader's repository and target engine.

### Record decisions as ADRs

Architecture Decision Records are useful where a real trade-off exists. Use [the ADR template](../templates/architecture-decision-record.md) for:

- Owning player capabilities in GAS versus a minimal custom model.
- Persistent progression in a subsystem versus per-level Actor state.
- One World Partition world versus multiple levels.
- UMG/MVVM versus direct widget bindings.
- Native source versus Blueprint authority.
- A custom plugin versus Engine-provided functionality.
- Save schema, failure recovery and platform packaging.

An ADR records context, considered alternatives, decision and consequences; **it is not evidence of implementing its chosen solution**. Avoid writing ADRs for every uncontroversial property or invented choice.

**Gate C2:** approved TDD and decisions identify the **single authority for each state**, lifetime, event/cancellation rules, Save/Load contract, Blueprint/Editor ownership and build/test integration. Dependencies and actual missing assets are explicit.

## 5. Decompose into an executable SDD Feature Spec

After C0–C2, create **one canonical Feature Spec per real player outcome** using the [existing feature template](../templates/feature-specification.md). The spec is not a preparation checklist; it describes the complete work to create, integrate, test and obtain acceptance for the feature.

### Mandatory contract

| Field | What must be written |
| --- | --- |
| Player-visible behavior | Given a starting world, describe the exact action and committed, observable consequence |
| Scope | Core requirements, explicit exclusions, relevant level and dependency order |
| Reuse | Real classes, methods, Blueprints, assets, plugin versions and any local inspection gaps |
| Authority and state | Inputs, eligibility, transactions, idempotence, rollback/cancellation, recovery and failure |
| Integrations | True actor placement, collisions, visuals, UI, animation, audio, persistence and startup configuration |
| Assets and rights | Which assets must actually exist; source/export/import, model/author, licensing and engine destination |
| Owners | One writable owner per file/asset and one active Editor/MCP operator; explicit handoff |
| Test evidence | Native, Editor, human PIE, streaming/save, standalone and packaging **as distinct layers** |
| Human acceptance | Actual reviewer, complete player route, success conditions and reject/repair loop |
| Outputs | Implemented source and assets, imported level, logs, build/package, change report and documented limits |

### Select workstreams conditionally

The smallest practical example may require **one C++ developer and one Editor operator**, not a mesh generator or a second framework. If an approved 3D asset already exists and is correctly licensed, reusing it may be better than creating a new one. If no art is needed, the generator/Blender columns are marked **Not Required** with reasons.

When AI tools perform production, assigning a role must mean **real tool access and actual work**, not a model describing what it would do. Distinct editor sessions cannot safely write the same map in parallel.

### One consolidated implementation handoff

Do **not** send scattered follow-up prompts that redefine the feature after its operator starts. Revise the **same canonical spec** and issue a **single consolidated task instruction** containing the version, exact owned paths, outcomes, failure cases, selected workstreams and reporting contract. Reassignment requires a deliberate handoff; never overwrite unsaved editor data to resolve a conflict.

**Gate C3:** the director approves **execution of the specific feature**, including scope, dependencies, responsible owners, real tool availability and acceptance. C3 authorizes the work; it does not mark it Implemented.

## 6. Worked traceability: an actual design, an illustrative execution contract

The following derives its **behavioral goals** from the historically approved The Unmade fountain design. It demonstrates what a production-ready spec must contract; it is **not** a new The Unmade implementation or proof of completed gameplay.

| Trace | Approved design rule | TDD implication | Implementation and acceptance requirement |
| --- | --- | --- | --- |
| FOUNTAIN-01 | Find missing mechanical part | Unique part ID and collected/installed state | The actual pickup disappears only after valid collection; repeat collection fails safely |
| FOUNTAIN-02 | Repair first, donate second | Distinct interaction phases and eligibility | Donation unavailable until installation; UI displays the real reason |
| FOUNTAIN-03 | Donate personal Light voluntarily | Single once-only transaction; GAS capability follows progression authority | Hold/confirm performs one donation, early release performs none; personal Light action actually ceases to work |
| FOUNTAIN-04 | Permanently power fountain | Restored world state owned outside VFX | Water, material and the newly traversable route reflect committed restoration; collision really changes |
| FOUNTAIN-05 | Separate fragment returns personal Light | Distinct recovery item ID; GAS state becomes Recovered | The Light action works again while the fountain stays on and the route remains open |
| FOUNTAIN-06 | No irreversible softlock | Safe traversal graph, required interactions independent of donated ability | Player can cross to recovery without personal Light or hidden alternate controls |
| FOUNTAIN-07 | Persist and stream safely | Snapshot logical state; idempotent Actor registration | Fresh process/load and stream-in reproduce correct states without duplicates or replay |
| FOUNTAIN-08 | Player can actually finish the feature | Editor assets, input, functional route, UI and audio feedback | Human plays the whole encounter; packaging is verified independently at its required gate |

### Given / When / Then scenarios (requirements, not reported test passes)

- **AC-01 — valid collection:** Given an uncollected part in range and unobstructed focus, when the player interacts, then the item becomes owned exactly once and the world pickup no longer grants a duplicate.
- **AC-02 — premature donation:** Given a broken fountain with no installed mechanism, when the player requests donation, then no personal ability is lost and the fountain does not restore.
- **AC-03 — canceled confirmation:** Given a repaired fountain and usable Light, when the player releases the confirm action early, then the action cancels with no fragment/ability/state mutation.
- **AC-04 — completed gift:** Given a repaired fountain, owned Light and a valid sustained confirmation, when donation completes, then the authoritative event occurs exactly once, the fountain is restored, the passage becomes physically traversable, and personal Light cannot be used.
- **AC-05 — repeated requests:** Given a restored fountain, when the player interacts again or reloads the same stream cell, then progress and rewards are not applied twice.
- **AC-06 — recovery:** Given a separately reachable recovery fragment after donation, when the player collects it, then personal Light works again while the fountain remains restored.
- **AC-07 — replay durability:** Given a completed encounter, when the player saves and starts a fresh process/load, then the world, pickup, ability and route states match the committed logical state.
- **AC-08 — human route:** Given the actual integrated map and controls, when the human tester plays from entrance to next objective, then the route is understandable and completable with no unauthorized ability requirement.

**Tuning parameters** (interaction reach, exact hold duration, camera angle, light intensity) are not asserted as globally valid values. Define concrete defaults in the **real game spec** and measure/adjust them through a playtest. Spec authors may propose defaults, but cannot record human approval before it occurs.

### Feature state reporting

After the work is authorized, each operator must report its real outputs and exact test cases. The human and documentation owner reconcile them without promoting a PASS from one layer into another.

| Stage | Evidence required |
| --- | --- |
| Planned | Approved contract and owners; implementation not begun |
| In Progress | Actual source/art/Editor work started |
| Implemented | Necessary source and native assets exist and are integrated into the intended gameplay system |
| Tested | Listed tests **actually executed**; required failures are visible and resolved or a recorded blocker remains |
| Human Validated | Named human reviewer observed the real required gameplay and accepted it |
| Released | Relevant packaged executable was distributed and the actual public/downloaded artifact verified |

A failed required test **blocks the corresponding acceptance**. A compile pass, a model export or a UE package alone never proves human validation. Use **PASS**, **FAIL**, **BLOCKED** or **NOT RUN** at case level.

## 7. Execute the complete feature, not an implementation simulation

The production order for a feature with native gameplay and Editor assets can be:

1. **Preflight:** inspect the actual code, Editor packages, existing assets, tool capabilities and saved/dirty state; reconfirm ownership.
2. **Gameplay authority:** implement C++/Blueprint logic, collision rules, transactions and native tests. Reuse existing systems.
3. **Conditional source art:** produce approved source/export meshes, textures and audio, recording licenses and import/rig requirements.
4. **Editor integration:** as the sole Editor operator, create/configure real Blueprints, Input Actions, UMG, map Actors, collision, VFX and actor-level wiring using supported APIs. Save and reopen relevant native packages.
5. **Local technical checks:** compile, run named tests, validate Editor assets and capture actual failures.
6. **Human gameplay:** complete the **real** encounter in PIE, checking appearance, interaction, camera, route, cancellation, recovery and softlocks.
7. **Durability and packaged checks:** fresh-process Save/Load and streaming, Win64 cook/package, executable launch and relevant gameplay smoke test when this is part of the approved milestone.
8. **Document actual status:** separate performed tests from unrun checks, record defects and human acceptance, update the **canonical** GDD/TDD/spec only when behavior or evidence requires it.

Parallel work is appropriate only for different files/assets and approved interfaces. One operator must hold the active Unreal Editor/MCP session.

The [T02 Unreal local build pipeline](unreal-project-bootstrap-local-build.md) describes local UBT/UAT commands. The [T01 MCP guide](unreal-mcp-5-9.md) describes safe Editor connection, schema discovery and authoring.

## 8. Common mistakes and corrective decisions

| Failure | Consequence | Corrective action |
| --- | --- | --- |
| Approve a mood board as a rigged character | The playable feature still lacks usable geometry and animation | Separate visual approval, source mesh export, rig QA, import and integration |
| Write all systems before a playable slice | Large, unverified dependencies and little time to ship | Define the full story but execute the minimum real vertical slice first |
| Treat every generator as required | Cost and integration grow without user-visible value | Select only needed workstreams; reuse real assets and tools |
| Put permanent world state in Niagara/Sequencer | Saving, skipping and streaming can undo or replay progress | Commit to a durable authority, then play transient presentation |
| Add a second state manager | Inconsistent ability, save and UI state | Audit existing owners and extend one authority |
| Save streaming Actor pointers | Load/World Partition lifecycle breaks object references | Serialize IDs and logical states; reconcile Actors on registration |
| Use multiple concurrent Editor agents | Dirty assets are overwritten or map ownership becomes unclear | One Editor operator; explicit save/handoff |
| Deliver source code without level assets | Compilation passes but no playable outcome exists | Make Editor Blueprint/map integration part of the same feature |
| Label a package as a passed game | Startup, input, objective or required cooked content may fail | Launch and play the built executable separately |
| Keep conflicting spec prompt revisions | Different agents implement incompatible interpretations | Consolidate changes into the existing canonical spec |
| Guess exact Unreal symbols before inspection | Unbuildable dependencies, duplicate classes | Verify installed Engine and actual project source first |
| Claim a test not performed | Misleading technical and release record | Mark NOT RUN or BLOCKED with the reason |

## 9. Acceptance of this tutorial as an educational exercise

The T03 document-production exercise is complete for a reader **only if**:

1. The authorized director selected a genuinely feasible project concept.
2. A charter states target, hard constraints, scope and intended playable loop.
3. The actual GDD defines player action, complete critical route, failure/recovery rules and optional cuts, with recorded human approval.
4. The actual TDD records inspected existing systems, one authority per durable state, module/asset interfaces and relevant architecture trade-offs, with recorded approval.
5. One canonical Feature Spec defines a real end-to-end behavior, integration workstreams, owner/handoff, concrete Given/When/Then checks and human acceptance.
6. Execution is **explicitly authorized** before developers/agents write game content; test outcomes stay NOT RUN until actually performed.
7. The author updates only the canonical documents, records decisions/unknowns, and protects any private input/reference materials.

This is an **education/documentation gate**. A reader cannot mark the feature **Implemented**, **Tested** or **Human Validated** until the corresponding game execution and checks happen.

### A compact task transition

```text
Candidate concept
  -> Approved project charter           [C0]
  -> Approved GDD + critical route       [C1]
  -> Approved TDD + decisions            [C2]
  -> Approved executable Feature Spec   [C3]
  -> Real implementation and integration
  -> Automated / Editor checks
  -> Human playtest and acceptance
  -> Standalone build and release checks
  -> Evidence-based documentation
```

## 10. Reusable documents and related learning

- [Project charter template](../templates/project-charter.md)
- [Game Design Document template](../templates/game-design-document.md)
- [Technical Design Document template](../templates/technical-design.md)
- [Architecture Decision Record template](../templates/architecture-decision-record.md)
- [Executable Feature Specification template](../templates/feature-specification.md)
- [Actual Feature Execution Workflow](../docs/06-production-planning/feature-execution.md)
- [The Unmade design/architecture decisions](../case-studies/the-unmade/decisions.md)
- [The Unmade F001–F014 historical inventory](../case-studies/the-unmade/features/README.md)
- [The Unmade failures and evidence limits](../case-studies/the-unmade/failures-and-fixes.md)

**Next:** T04 — multi-agent production and handoffs, focusing on real work partitioning, dirty Editor packages, explicit inputs/outputs and conflict resolution.
