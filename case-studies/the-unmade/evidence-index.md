# The Unmade — Evidence Inventory and Publication Boundary

## Private-source categories reviewed for this educational account

The case was grounded in the original game's:

- Human-directed creative discussion, accepted premise and GDD/Story iterations.
- Approved Technical Design and architecture decisions.
- Agent coordination/SDD process and production skill definitions.
- F001–F014 executable specifications and dated status notes.
- F005 native and Editor reports (including visual/cinematic corrections).
- F006 native execution report with individual PASS/FAIL outcomes.
- F007 character pipeline and Unreal integration report.
- Environment V3 production and FBX verification reports.
- F008/F009/F011 Editor implementation/continuation and R3 materials report.
- Competition compliance and audio/VFX design briefs.

This index names **source categories**, not downloadable public mirrors. Their presence in a private history is not permission to publish source bytes or copyrighted assets.

**Publication method:** [T20 — Glass Box, Provenance and Evidence-Based Postmortem](../../tutorials/glass-box-provenance-postmortem.md) explains how this evidence-grade inventory supports honest public reporting and how private records must remain private. Its read-only auditor verifies supplied real files/hashes, not historical claims or missing runtime tests.

## Evidence grades

| Grade | Evidence type | Appropriate claim |
| --- | --- | --- |
| D | Concept, approved GDD/TDD, feature design | The approach was approved or proposed |
| S | Real source files / export report | The agent reported writing the implementation or producing output |
| E | Saved Editor assets, asset compilation or importer output | Specific Editor checks were executed |
| A | Named automated runtime/native tests | Specific automated assertions passed or failed |
| H | Human manual PIE/standalone test record | Observed player behavior for stated conditions |
| R | Verified packaged game and public submission | Recorded released/distributed outcome |

No one grade silently implies another. **Historical F006:** a packaged archive can coexist with a failed authored-camera test and an unrun executable launch.

## Audit procedure before reusing historical material

1. Identify the exact source record, its version/date and author/agent.
2. Check whether later reports supersede its status.
3. Inspect what was actually executed; do not equate process exit zero with all tests PASS.
4. Search text and file metadata for machine paths, usernames, credentials, private repository URLs, tokens and proprietary source.
5. Verify copyright/redistribution permissions for each image, sample file, code excerpt and audio track.
6. Publish a short truthful summary, sanitized relative technical names and a clear evidence grade.
7. Do **not** fabricate public logs, screenshots or hashes when original evidence cannot be shared.

## Current boundary

No original game source, private repo commit history, native `.uasset`/`.umap`, or externally licensed 3D/audio assets have been copied into this playbook. The original jam submission's final status is **not established by the reviewed evidence**, so it is deliberately not claimed.

See [third-party rights](../../THIRD_PARTY.md), [historical timeline](timeline.md) and [known failures](failures-and-fixes.md).