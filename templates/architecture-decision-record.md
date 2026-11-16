# Template — Architecture Decision Record (ADR)

Use an ADR only for a material, genuinely evaluated engineering choice. Do not turn unverified ideas into “approved” decisions.

## ADR identity

- ID and descriptive title:
- Date and decision owner:
- Status: Proposed / Approved / Superseded:
- Relevant approved GDD/TDD/spec revisions:

## Context and problem

What actual implementation, runtime, maintenance, performance, safety or delivery issue requires a choice? Document observed constraints and evidence.

## Options actually considered

| Option | Advantages | Costs/risks | Compatibility with existing game |
| --- | --- | --- | --- |
| Option name | Actual benefits | Actual downside | Reuse, migration and Editor implications |

Do not fabricate benchmarking or comparison results. If an option was not tested, say so.

## Decision and rationale

Which approach did the responsible human approve, and why? What invariants must the implementation preserve?

## Consequences and implementation impact

- Affected authority, lifetime and state transitions:
- Source/Blueprint/Editor asset responsibilities:
- Integration, data migration, SaveGame/streaming or UI effects:
- Test cases, packaging and rollback/cut conditions:
- Known unresolved limitations:

## Evidence and follow-up

- Design approval record/date:
- Linked TDD and executable Feature Spec:
- Actual implementation evidence, if any:
- Real verification results and outstanding NOT RUN checks:

**Decision accepted** is separate from **implementation verified**. If the project changes course, mark this ADR Superseded and link the replacement rather than silently rewriting its history.
