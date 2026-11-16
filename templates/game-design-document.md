# Template — Game Design Document (GDD)

**Role:** canonical player-facing design. Implemented source, technical architecture, editor assets and test evidence live in their own documents.

## Control and approval

- Game title, GDD version, date, author and owner:
- Related approved [project charter](project-charter.md):
- Human creative reviewer and approval status:
- Decisions pending explicit review:
- Product/platform/release constraints:

## Creative vision

- One-sentence game fantasy:
- Core theme and player emotion:
- What makes the game original and fun:
- Narrative premise and mandatory conclusion:
- Style identity (visual, animation, music, sound and UI):
- Content restrictions and rating:

## Player model

- Camera, perspective, scale and movement:
- Primary verbs and interactions:
- Default input/actions and remapping requirements:
- Onboarding, first interaction, controls page and discoverability:
- Accessibility needs and readability at gameplay scale:

## Core gameplay loop

Describe a **repeating meaningful player action** with a real cost/reward and a new world consequence. Include progression and how the player recovers, adapts or fails safely.

```text
Explore -> Discover -> Decide/Act -> Pay cost -> World changes -> New route -> Repeat
```

Replace that pattern with **this game's actual behavior**; it is not a mandated genre formula.

## Mechanics and state rules

For each mechanic record:

| Mechanic | Prerequisites | Player action | Gameplay state change | Denial / failure / recovery | Visible/audible feedback |
| --- | --- | --- | --- | --- | --- |
| Name | Real eligibility | Input and target | Persistent/transient | Cancellation, repeat, softlock | Actual player cues |

Be explicit about separate collected physical items, character abilities, spent/donated resources, world states and earned access. Do not silently introduce new systems.

## World, progression and dependency graph

- Areas and landmark connections:
- Entry and starting conditions:
- Mandatory objectives in order:
- Locked routes and unlocking consequences:
- Save/checkpoint/restart and streaming assumptions:
- Irreversible choice and anti-softlock rule:
- Optional exploration versus critical route:
- Ending state and player control afterward:

Supply an actual route/dependency diagram when the design depends on topology.

## Narrative and presentation

- Opening/awakening and player agency:
- Character and world storytelling:
- Short interactive/noninteractive beats:
- Environmental change:
- Finale and ending transition:
- Controls return, skip/retry and accessibility:

## UI and audio

- Real HUD and interaction prompts:
- Main menu, Pause, Continue, Options and Controls:
- Save/Load and genuine error/status presentation:
- Audio identity, dynamic states, music, SFX and rights:

## Scope boundaries and production

| Essential | Optional | Cut / not applicable |
| --- | --- | --- |
| Required route and systems | Safe post-slice additions | Exclusions with rationale |

- Production environment/target and real assets:
- First real vertical slice (complete player outcome):
- Required technical handoffs:
- Questions to test during human PIE:
- Expected packaged-game and public-release criteria:

## Approval record

- What the human approved and when:
- Values requiring gameplay tuning rather than new creative approval:
- Contradictions with earlier versions and resolution:
- Next canonical artifact: [Technical Design](technical-design.md).

**Design approval is not implementation or playtest acceptance.** Record the actual revision used by each Feature Spec.
