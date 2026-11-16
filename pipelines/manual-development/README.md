# Manual Development Path

The manual path has **no AI dependency**. The same project stages, artifact contracts and acceptance criteria apply to solo developers and multi-person teams.

## Reference roles

A solo developer may own every task sequentially; on a team, assign design, gameplay, level, technical art, animation, sound, QA and release roles as capacity allows. Each changed file or asset still has one current writer; merge reviewed work without erasing other contributors' changes.

## Working sequence

1. Define audience, target platform, one-sentence game promise and smallest complete route.
2. Approve compact GDD and technical decisions; choose technologies that match team skills.
3. Create a small executable feature spec describing player behavior, failure modes, files and tests.
4. Implement the authoritative game system and connect it to the scene and UI.
5. Create required art in Blender, the Engine or approved third-party tools; preserve rights and editable source.
6. Run code tests, Editor validation, manual playtest and early standalone packaging as separate gates.
7. Iterate on actual player feedback; only then add the next level/feature.
8. Finish controls, save/load if required, complete ending, audio, credits, performance and distribution.

## Deliverables

The project needs a game that runs, not speculative architecture. A small completed map and one meaningful state-changing interaction are better early evidence than a large empty asset library. Use the same [Feature Execution Workflow](../../docs/06-production-planning/feature-execution.md) as the AI-driven path, replacing agent handoffs with human peer handoffs.

## Benefits and limitations

Manual production offers direct creative control and fewer automation/editor-tool constraints but may require longer authoring time. Avoid treating that as a measured speed claim without actual comparable time logs.