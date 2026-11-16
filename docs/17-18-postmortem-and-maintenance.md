# Chapters 17–18 — Postmortem, Maintenance and Reuse

A useful postmortem distinguishes **intentions from evidence**, including work cut because it was no longer feasible.

**Hands-on provenance and Glass Box:** [T20 — Evidence-Based Glass Box and Postmortem](../tutorials/glass-box-provenance-postmortem.md) provides the real reporting contract and a [read-only Python SHA-256/status audit](../examples/glass-box-audit/README.md) for actual local evidence files. A valid manifest hash is not proof that a game passed human acceptance.

## Historical reconstruction

Maintain a dated timeline containing: initial constraints, concept choices, GDD/TDD approvals, feature specification creation, actual editor/source execution, owner handoffs, known failures, playtest observations, commits, packaged builds, release events and omissions. A design document created on a given date cannot by itself prove implementation happened that day.

Historical specs and later evidence reports may disagree because status fields were not updated. Retain both records and reconcile their time relationship explicitly. The strongest accurate formulation is often “the initial spec was Planned; a later agent report records Editor integration; human runtime validation was not evidenced.”

## What to analyze

- Which creative decisions improved the player experience?
- Which integrations created unexpected cost (camera, FBX, skeleton, materials, streaming, UI)?
- Where did task boundaries, locked assets or unsaved Editor changes cause conflicts?
- Was the build packaged early enough to expose missing dependencies?
- Which tools actually saved time after accounting for fixes, license checks and integration?
- Which optional features consumed time without helping complete the core game?
- What did the human actually play and approve?

Avoid ungrounded monetary savings, time productivity claims or model rankings. If no comparable measurements exist for manual vs AI-driven production, state that.

## Maintenance

Maintain compatibility notes by **verified tool version**. Keep examples runnable or clearly labelled as *patterns*, not production scripts. For every new engine release, review APIs, plugin maturity, export/import behavior and packaging rather than assuming earlier success transfers automatically. Preserve provenance and rights when changing generation providers.

**Exit evidence:** a factual public learning record with explicit unknowns, failures, action items and future ownership.