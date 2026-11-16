# Template — Technical Design Document (TDD)

**Role:** engineering/integration authority for the **approved GDD**. Do not invent available APIs, plugins, classes or local assets; inspect the actual project before filling this document.

## Source, version and approval

- Project, Engine version and actual installed build:
- Approved [GDD](game-design-document.md) reference/revision:
- Repository/branch/revision audited:
- Design/technical owners and human approver:
- TDD state: Draft / In Review / Approved / Superseded:
- Known locally uninspected content, unsaved Editor assets and missing tool access:

## Existing project audit

Inventory what **already exists** and can be reused:

| Existing subsystem or asset | Actual source/asset (relative path) | Present and verified? | Extension or reuse |
| --- | --- | --- | --- |
| Input/Player | | | |
| Gameplay state and events | | | |
| World Actors and collision | | | |
| Save/load | | | |
| UI and user settings | | | |
| Editor maps and Blueprints | | | |
| Build/test tooling | | | |

Remote source listing alone does not establish the absence of untracked local Editor content.

## Architecture and authority

- Real Engine target/modules and plugin dependencies:
- Source-of-truth for progression, abilities, inventory and settings:
- Actors/components/interfaces and their lifetime:
- Data ownership, read/write boundaries and event contracts:
- Input, interaction and camera rules:
- Gameplay-state diagram and legal transitions:
- Synchronous commit/rollback, retries, cancellation and duplicate prevention:
- Threading/async constraints where applicable:
- Error reporting and restart behavior:

## Persistence and streaming

- Which logical IDs and state are serialized:
- Save schema version, file/slot ownership and actual failure handling:
- Actor/World Partition registration, stream-in/out behavior:
- Idempotent reconciliation versus one-time presentation:
- Fresh process, upgrade/migration and corrupted/missing snapshot behavior:
- Softlock prevention and checkpoint recovery:

## Presentation boundaries

- Gameplay authority to UI/UMG/MVVM:
- Materials, animation, sound, Niagara and cinematic trigger events:
- Distinct concept, 3D source/export and Engine import stages:
- Asset provenance and redistribution constraints:
- Input focus and camera return after cutscenes:

## Build and packaging integration

- Actual Engine build target and tools:
- Local source compile commands and checks:
- Editor asset validation and human PIE:
- Platform cooking, dependencies and startup map:
- Packaged executable and fresh-process acceptance:

## Architectural decisions

Document only meaningful trade-offs in an [ADR](architecture-decision-record.md). Note selected option, alternatives, costs, limitations, migration effects and reason.

## Feature interfaces and order

| Feature | Actual required system/asset | Dependency | Owner | Acceptance impact |
| --- | --- | --- | --- | --- |
| Identifier | Existing/required | Other approved work | One writer/operator | Player-visible check |

Define one responsible owner per file/asset and a sole Unreal Editor/MCP operator per session where applicable. Documentation and agent reports have separate owners.

## Verification and approval

- Required test layers and who executes them:
- Actual baseline tests/builds already run (cite evidence; else NOT RUN):
- Technical risks, unknowns and planned checks:
- Human technical decision/approval and date:
- Next artifact: [Executable Feature Spec](feature-specification.md).

**Technical design approval does not prove implementation, compiling, Editor validation, playable PIE or standalone Shipping.**
