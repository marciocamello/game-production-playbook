# T20 — Glass Box, Provenance and Evidence-Based Postmortem: Publish What Actually Happened

**Publication status:** This guide establishes an operational, evidence-grounded public production log and a [working source-level, read-only manifest verification script](../examples/glass-box-audit/README.md). Neither an actual production evidence manifest nor files from a private game have been audited through the script during publication. No fresh runtime test, original asset, license grant, store upload or human final approval is invented. **Runtime and real-evidence execution remain NOT RUN.**

**Objective:** produce a truthful, judge/reviewer-accessible report of human decisions, AI tools and prompts, ownership, original/third-party assets, rights, implementation, testing, failures, scope cuts, release and lessons learned, with **traceable artifacts** and clear unknowns. The Glass Box must not reveal secrets, unrelated private repositories or personal machine details.

**Reuse:** [T03 GDD/TDD/SDD](concept-to-gdd-tdd-executable-sdd.md), [T04 ownership/handoffs](multi-agent-production-handoffs.md), [T10 asset briefs](visual-references-generation-ready-briefs.md), [T18 test evidence](native-tests-editor-pie-profiling.md), [T19 release candidate/footage](windows-packaging-gameplay-capture-release.md), the [existing case-study evidence inventory](../case-studies/the-unmade/evidence-index.md), and [existing production templates](../templates/README.md). Do not create a competing second feature status ledger if a canonical one already exists.

## 1. Understand the reporting contract

A **Glass Box** explains how actual work happened and which pieces remain uncertain; it is not a promotional timeline that upgrades unverified AI statements to human playtest proof. Publication requirements may vary by competition, employer or store. Check the **current official rules** for required contents, deadlines, public access and AI disclosure before release.

| Evidence class | What it can truthfully establish | What it cannot establish alone |
| --- | --- | --- |
| Decision/spec | Intent and authorized design | Built gameplay |
| Agent/tool output | Reported source changes, export attempts or actions | Valid Editor integration |
| Git commit | Repository source artifact at that revision | Working Unreal package |
| Imported Editor content | Authored saved asset exists | Correct runtime behavior |
| Native/automated test report | Named checks ran with outcomes | Human game feel and full route |
| Manual PIE record | Player-observed behavior in Editor | Standalone release |
| Packaged game hash and launch | Known executable tested externally | Public availability |
| Public download and human acceptance | Distribution/acceptance observed | All hypothetical edge cases |

Each claim must point to a **real, inspectable artifact** with an outcome and context. When two historical reports conflict, preserve dates and explain which one is later or more authoritative; do not rewrite history to look cleaner.

## 2. Use one canonical decision and execution ledger

Maintain a project-owned log organized by date/revision, not by whichever agent wrote the newest chat response. Every substantive row should identify:

- Stable record ID, timestamp with timezone, project/game version and source commit.
- Decision maker (human/role), event category, what was approved or rejected, reason, scope impact and superseded decision if any.
- Task/spec ID and actual owner; source paths or Editor asset package names.
- AI involvement: provider/model/version where known, role, relevant prompt/context summary, tool/MCP action logs and actual outputs. Describe whether output was narrative, source code, 3D model, animation, audio, test or integrated gameplay.
- Rights/provenance source, commercial/redistribution grant, license/credits, provider terms/version/date, third-party attribution and any special restrictions.
- What was **tried**, what succeeded/failed, concrete test ID and environment, evidence reference/hash and **human acceptance** status.
- Explicit cut/deferred work, defect severity, rollback/mitigation, remaining risks and public publication boundary.

Do not infer model identity/version when the provider did not expose it; state **Unknown**. Do not list every exploratory prompt as a completed implementation. A large prompt count is not a measurable productivity benefit.

### Stage vocabulary

Use **Planned → In Progress → Implemented → Tested → Human Validated → Released**, but never skip a gate because of confidence or a successful LLM answer. A created mesh export is not an imported StaticMesh, a BP compile is not PIE, and a package is not a human-reviewed complete route. For each required check use the canonical **PASS / FAIL / BLOCKED / NOT RUN** vocabulary and a separate approved **NOT APPLICABLE** only with a real scope decision.

## 3. Reconstruct actual chronology from the strongest sources

Start from repository/Editor evidence, not an idealized schedule:

1. Record the real initial constraints, platform, rule set, deadline and initial decisions from dated source records.
2. Identify approval points for story, core loop, art bible, GDD, TDD, feature specs, project structure and critical route.
3. Tie meaningful changes to actual commit SHAs and explicit agent/human ownership. Inspect diffs and later reports rather than treating commit titles as a complete description.
4. Identify source/export/import stages for every significant 3D, animation, material, VFX and audio asset. Keep provider, Blender/Unreal artifact paths and rights separately; image references are **not** 3D integration.
5. Record actual tests at each stage, with both positive and negative cases, failures and unrun checks. Preserve original failing reports and later fix/retest evidence.
6. Record actual gameplay demonstrations, fresh-process SaveGame, standalone executable identity and real published release URLs when verified.
7. Explain deliberate scope cuts, altered mechanics, abandoned techniques and tradeoffs. Do not erase decisions simply because the final game changed.
8. Publish a condensed chronology with direct links only to authorized public files. Private reports may be summarized with rights-safe statements and evidence grades without mirroring their source.

The public [The Unmade case study](../case-studies/the-unmade/README.md) already separates historical decisions, failures, scope cuts, timelines, features and evidence grades. Treat it as an **educational record**, not permission to expose its original private files or assume every historical test passed.

## 4. Distinguish AI generation, asset integration and game execution

Every AI-assisted artifact needs a **provenance chain** with meaningful stages:

    Idea/approved prompt
      -> actual provider generation or authored source file
      -> origin/licensing checked
      -> actual import/export or native source build
      -> Editor integration into gameplay
      -> tested in the relevant live engine state
      -> independently observed player outcome
      -> released, if applicable

A screenshot of a generated object is not proof of Blender topology/rigging; an FBX does not establish a saved/working Unreal asset; a Niagara emitter on disk is not evidence of a restoration transaction; sound licensed as a preview may not be distributable; a full T18 test report does not itself validate a T19 downloaded game.

For failures, log not just the observed crash but also the smallest reproduction, exact version, root-cause confidence, what changed to fix it, re-run results and remaining known risk. Do not backfill nonexistent success screenshots.

## 5. Protect privacy and legal rights before publishing

Classify evidence into **public**, **private retained**, **redacted summary**, and **excluded**. Before making a public GitHub/jam/website log:

- Remove personal absolute paths, OS usernames, private addresses, access tokens, API keys, session cookies, model-provider credentials, private issue URLs, account/transaction details and unrelated private repository names or source files.
- Avoid copying copyrighted third-party meshes, audio, concept images, fonts or AI-generated content when the redistribution grant is not verified. A public link to original license/credit is not a substitute for actual license compliance.
- Confirm whether prompts contain sensitive internal code or proprietary reference images; publish only permitted prompts/excerpts, with a reasoned omission note where relevant.
- Avoid attaching oversized logs with private stack traces or personally identifying test screenshots. Sanitize evidence copies while preserving originals privately and their own hashes.
- Review public links while signed out or from a separate browsing context to confirm judge access; do not force a private repo to become public merely for submission.

Never post unrelated project internals into the Glass Box. General techniques can be described without revealing proprietary source.

## 6. Audit real files with a reproducible, read-only integrity check

The [T20 Python manifest auditor](../examples/glass-box-audit/scripts/audit_evidence_manifest.py) verifies **existing files and hashes** without creating evidence or changing any file. It uses Python 3.11+ standard-library modules, resolves paths under an approved evidence root, refuses path traversal and duplicate IDs, validates SHA-256, and checks that each test record has an explicit outcome. It **cannot** certify that the content of a log is truthful or that rights are legally valid; those remain human audits.

Prepare a **real** JSON manifest containing:

| Field | Type | Required behavior |
| --- | --- | --- |
| schema_version | integer | Exactly 1 |
| artifacts | nonempty array | Real asset/source/package evidence records |
| artifacts[].id | unique nonempty string | Unique across all artifact and test IDs |
| artifacts[].path | relative path | Existing file under the specified evidence root |
| artifacts[].sha256 | 64 hex characters | Must equal actual file contents |
| artifacts[].source_ref | nonempty string | The real originating decision/commit/provider record |
| artifacts[].rights_or_license | nonempty string | Auditable rights or license reference, **not** legal approval by script |
| tests | nonempty array | Actual named test case records |
| tests[].id | unique nonempty string | Real check ID, never invented execution |
| tests[].status | enumerated string | PASS, FAIL, BLOCKED, NOT RUN or NOT APPLICABLE |
| tests[].reason | nonempty string | Required for FAIL/BLOCKED/NOT RUN/NOT APPLICABLE |
| tests[].evidence_path | relative path | Required for PASS and FAIL; optional otherwise |
| tests[].evidence_sha256 | 64 hex characters | Required whenever evidence_path is present |

The manifest is **not** a place to invent placeholders. Fill it with real file paths and hashes only after evidence exists. For the planned game-level verification matrix, include unexecuted cases as NOT RUN with truthful reasons, rather than omitting them.

Run locally against an **existing** manifest and authorized evidence directory:

    python ./examples/glass-box-audit/scripts/audit_evidence_manifest.py --root ./evidence --manifest ./evidence/manifest.json

The code's exit semantics are intentional:

- **0**: syntactically valid manifest, verified referenced file hashes and no FAIL/BLOCKED/NOT RUN cases. This is **integrity success**, not proof of runtime authenticity or human acceptance.
- **1**: invalid manifest, missing/unsafe file, incorrect hash or structurally insufficient record.
- **2**: files/structure validate, but at least one honest FAIL/BLOCKED/NOT RUN case remains; release cannot silently claim all tests passed.

A file may have a valid hash and still contain a **false** claim. The script cannot interpret gameplay video, grant commercial rights, inspect the game UI or substitute for authorized reviewers.

## 7. Public report layout

Use one readable report with explicit references to existing canonical data:

1. **What we intended:** rules, scope, premise, approved audience/engine and design.
2. **Who did what:** human direction; model/provider roles; exact tool operations; one-writer artifact ownership.
3. **How artifacts were made:** code systems, generated/Blender models, imported assets, animation, VFX, UI, audio, rights and credits.
4. **What actually ran:** compile, native tests, Editor checks, PIE, packaged game, real recorded gameplay, with PASS/FAIL/BLOCKED/NOT RUN distribution.
5. **Failures and repairs:** meaningful regressions, tool limitations, fixes and retest evidence. Include unresolved defects plainly.
6. **Cuts and decisions:** what was intentionally removed/deferred and the reason, particularly for a jam deadline.
7. **Final distribution:** released build identity, approved public destination, checksum, real capture and who accepted it, if actually verified.
8. **Postmortem:** what improved the result, what was costly, measured rather than imagined benefits, next improvements and owner.

Do not inflate a 20-tutorial **publication** milestone into a claim that all tutorial exercises ran in Unreal, Blender or packaged Windows. Documentation publication and real project validation remain different states.

## 8. Postmortem with engineering evidence, not vanity numbers

Ask which decisions improved fun, scope or player clarity; which system handoffs failed; where asset rigging/export, Unreal camera collision, World Partition or UI integration consumed time; whether first packaging occurred early enough; whether the team made deliberate cuts; and which agent/tool actually saved measured time.

When comparing AI/manual workflows, measure **comparable** task complexity, human correction time, integration failures, license checks, rework and final runtime quality. Avoid invented hours saved, arbitrary agent leaderboards or unsupported claims of 100% automated quality. The human remains accountable for acceptance and public claims.

## 9. Verification matrix — 27 publication checks

| ID | Observable evidence |
| --- | --- |
| G01 | Current event/platform reporting rules reviewed |
| G02 | Canonical decisions and dates traced to source |
| G03 | Model/tool ownership and prompt scope recorded |
| G04 | File/asset owner handoffs recorded without conflicts |
| G05 | Approvals separated from implementation |
| G06 | Implementation separated from Editor integration |
| G07 | Editor integration separated from human PIE |
| G08 | Packaged launch separated from public distribution |
| G09 | Every cited critical gameplay feature has source/evidence |
| G10 | Native test cases individually reported |
| G11 | Failed and blocked tests preserved with reasons |
| G12 | Unrun verification honestly listed |
| G13 | All published external assets have traceable source |
| G14 | Licenses, AI-provider terms and attribution checked |
| G15 | All generated art/audio explicitly distinguished from imported files |
| G16 | Public timeline faithfully reconciles conflicting dated records |
| G17 | Scope cuts and unfinished systems are transparent |
| G18 | Real captured gameplay tied to the released candidate |
| G19 | Published build identity and distribution verified |
| G20 | Public material excludes tokens, private paths and sensitive details |
| G21 | Unrelated private project source/identifiers absent |
| G22 | Evidence links work without privileged/private repository access |
| G23 | Actual evidence manifest exists and uses unique IDs |
| G24 | Real local hashes validate and files do not escape root |
| G25 | Integrity audit does not mislabel open gates as all-pass |
| G26 | Human director reviews and authorizes public Glass Box |
| G27 | Postmortem includes concrete failures, measured outcomes and next owners |

Use PASS/FAIL/BLOCKED/NOT RUN for each. A publication may have an open gate while still being an honest useful report; do not silently change its status to satisfy the auditor.

## 10. Failure diagnosis, stewardship and references

- **A source claim exceeds evidence:** downgrade the claim, retain source link and request actual runtime proof rather than invent screenshots.
- **Hash mismatch:** determine whether file contents changed, manifest is stale or the wrong version was cited. Never overwrite the expected hash to hide a discrepancy without inspecting the real artifact.
- **Missing asset license:** exclude it from public release until verified and record the scope cut or replacement.
- **Conflicting agent reports:** retain both with date, exact artifacts and known later superseding evidence.
- **Private details leak:** stop publishing the affected artifact, revoke exposed credentials when applicable and publish a sanitized replacement only with approval; a Git commit containing secrets can remain in history.
- **Public download unavailable:** record the release as not verified/public until an independent fetch works.

**Authoritative references:** [T03 executable SDD](concept-to-gdd-tdd-executable-sdd.md), [T04 ownership](multi-agent-production-handoffs.md), [T18 QA](native-tests-editor-pie-profiling.md), [T19 packaged release](windows-packaging-gameplay-capture-release.md), [Glass Box case-study evidence index](../case-studies/the-unmade/evidence-index.md), [existing postmortem chapter](../docs/17-18-postmortem-and-maintenance.md), [License/third-party notices](../THIRD_PARTY.md). Verify specific competition/platform requirements from the current organizer's official rules; this general playbook is not a legal determination.

**Series closeout:** T01–T20 constitute the complete **published educational tutorial series**. The accompanying scripts are real source examples with independent runtime validation obligations. A downloadable game is validated only through its project's own integration, QA, human acceptance and shipping evidence.
