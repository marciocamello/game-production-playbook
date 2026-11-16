# AI-Assisted Development Path

Human contributors remain the direct operators of game code, DCC and Engine assets. AI provides targeted research, drafting, code suggestions, focused debugging, asset concepts or second opinions **only where helpful**.

## Practical division

| Work | Human owns | AI may assist with |
| --- | --- | --- |
| Narrative and GDD | Approval and consistency | Ideation, revision, option comparison |
| Architecture and code | Design decisions, file writes, build and verification | API research, implementation proposals, review, focused debugging |
| Visual art and Blender | Source file, modeling/rigging and acceptance | References, Python snippet review, topology/checklist suggestions |
| Unreal Editor | Blueprints/maps/collision, actual PIE | Technical steps, suspected failure causes, test cases |
| QA and release | Real game execution, human observations, upload | Test planning, evidence summaries, regression analysis |

AI suggestions must be checked against the **installed tool version** and the actual repository. Never paste speculative Engine API calls as if verified.

## Workflow

Start with the current source and a focused question. Ask for one consolidated, implementable answer, inspect it, apply the changes, execute real checks and report failures back with scoped evidence. Maintain a single authoritative spec; do not accumulate contradictory prompt amendments or duplicate classes. Reuse working functions and existing assets before adding new abstractions.

## Documentation and provenance

Record the human-created implementation, AI assistance used, prompt reference when relevant, verified output, rights basis and test result. No special credit is given to unvalidated AI output. The human remains accountable for accuracy and playable acceptance.

See [the 197-step roadmap](../../docs/production-roadmap.md) and [the AI-driven path](../ai-driven-development/README.md) for full delegated-editor execution.