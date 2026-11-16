# Reusable Production Templates

These are **instructional formats**, not partially implemented features. Copy only a template that serves an actual deliverable and fill it with real evidence. Do not create fake game files, empty classes, dummy assets or proof of tests that were not executed.

| Template | When to use |
| --- | --- |
| [Project Charter](project-charter.md) | Select a feasible game concept and approve platform, team, deadline and constraints |
| [Game Design Document](game-design-document.md) | Define approved player experience, full critical route, mechanics and intentional cuts |
| [Technical Design Document](technical-design.md) | Define verified existing systems, authority, data lifecycle and actual Engine integration |
| [Architecture Decision Record](architecture-decision-record.md) | Record the rationale, alternatives and consequences of a meaningful technical choice |
| [Executable Feature Spec](feature-specification.md) | Before assigning a real end-to-end gameplay or content feature |
| [Agent / contributor handoff](agent-handoff.md) | When transferring files, Blender output or sole Editor ownership |
| [Artifact Ownership Matrix](artifact-ownership-matrix.md) | Assign one writer per file/asset, track the exclusive Editor session and prevent destructive conflicts |
| [3D Asset Brief and Provenance](asset-brief.md) | Before spending generation credits, modeling or importing assets |
| [Test and Acceptance Plan](test-plan.md) | When separating native, Editor, PIE and packaged checks |
| [Release Checklist](release-checklist.md) | Before shipping a downloadable game |

**Verification companions:** The [T18 native test example](../examples/native-validation-tests/README.md), [T19 release archive/video inspector](../examples/release-evidence/README.md) and [T20 Glass Box evidence audit](../examples/glass-box-audit/README.md) are working source examples with runtime validation still required; they are not blank feature templates.

Every completed task must have one authoritative artifact path, current owner, known dependencies, objective checks, failures, and an acceptance decision. For a comprehensive task list, see [197-step roadmap](../docs/production-roadmap.md).