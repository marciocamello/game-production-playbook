# Chapters 01–03 — Discovery, Research and Preproduction

**Purpose:** turn an uncertain idea and constrained resources into an approved, achievable production target. This phase is useful for both deadline-driven game jams and longer commercial projects.

## 01. Project discovery

Start with a short project charter: player fantasy, audience, intended session length, platform, one-sentence differentiator, must-have mechanic, deadline, team, budget, and key exclusions. Collect several candidate ideas, then compare each against the **actual production limits**: engine familiarity, asset requirements, novelty, gameplay readability, integration cost and time remaining for a real release.

Evaluation must be tied to a playable action. “A beautiful city changes” is an art vision; “the player deliberately gives up a capability, causing a route to become permanently traversable” describes a testable gameplay event. Record alternatives you rejected and why. Do not treat a concept illustration as an implemented prototype.

**Approval gate:** the director confirms one game concept, its primary loop, audience, boundaries and at least one likely viable vertical slice.

## 02. Competition and legal research

For a jam, review the current **official rules**, permitted preparation, eligibility, assets, team composition, deadlines in the correct timezone, content rating, required text/audio language, gameplay-video resolution and duration, and responsibility for submitting. Create a compliance checklist linking each criterion to an actual source. A summary from an earlier event is not authoritative for a new event.

Before writing gameplay, distinguish permitted pre-event preparation (tools, planning, prompts, configuration, depending on the rules) from content that must be newly produced during the competition. Never hide earlier development or misdate source files.

For a non-jam project, replace the competition gate with store policies, accessibility targets, privacy, licensing and commercial-use rights. A downloaded asset is not necessarily redistributable. Record third-party sources before they are mixed into authored game content.

## 03. Tool and architecture selection

Build a capability matrix rather than choosing every available tool. For each software or agent, record:

| Dimension | Questions to answer |
| --- | --- |
| Real ability | Can it edit native project files, only suggest changes, or merely generate references? |
| Integration | Which APIs, import formats, editor sessions or connector permissions are actually available? |
| Cost | Subscription, credits, generation failure rate, storage, compute and human review time |
| Legal | Rights to generated/source material; compatibility of imported content |
| Quality | Scale, topology, rigging, visual consistency, reproducibility and release readiness |
| Fallback | What manual/native workflow works when this tool is unavailable? |

For a typical Unreal project, choose whether each feature needs C++, Blueprints, Editor Python, Modeling Mode, an external DCC or a generator. **Inspect the installed editor and API signatures; do not invent MCP tool names or engine functions.**

## 04. Repository and operating model

Set the game repository's ignore rules before producing binary content; preserve versioned source, authored assets, docs, explicit local packaging scripts and factual dev logs. Ignore build intermediates, caches, credentials and external engine-source junctions. Never copy the engine source into an educational repository.

Establish a canonical documentation owner, stable feature IDs, task ownership, agent handoff rules, one writer per file, one shared-editor operator and a way to record test results. The human remains accountable for game-design acceptance and release.

**Exit evidence:** approved charter, rules matrix, licensing policy, engine/tool decision record, clean repository baseline and initial risk register.

Related: [project roadmap](production-roadmap.md) · [feature execution](06-production-planning/feature-execution.md).