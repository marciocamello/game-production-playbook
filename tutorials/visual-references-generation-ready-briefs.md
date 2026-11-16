# T10 — Visual References, Style Bibles and Generation-Ready Art Briefs

**Publication state:** A complete, actionable tutorial and original example brief have been published. **No reference images, AI assets, meshes, Unreal packages or Blender projects were generated, imported or reviewed in a live tool during this publication.** Any production output remains NOT CREATED / NOT RUN until performed.

**Audience:** art directors, solo developers, technical artists, AI visual producers and Blender/Unreal operators. **Target:** Blender-to-Unreal Engine 5.9 stylized 3D production, but the principles work with manual and AI-assisted pipelines.

**Deliverable:** a human-approved visual production contract covering one playable landmark and a modular asset family, not a gallery of attractive pictures.

## 1. Inspect the existing game before proposing visuals

Read the approved [charter/GDD/TDD](concept-to-gdd-tdd-executable-sdd.md), the real playable critical route, camera settings, target renderer, authored assets, environment modular scale and license records. Open the **actual** Editor project or request a factual owner report. Do not assume remote Git lists locally ignored `.uasset` files or unsaved Blender scenes.

Record all of the following:

- Visual objective: what emotion, readable game action and gameplay consequence does the scene express?
- Camera: actual elevation/FOV, average player-to-prop distance, thumbnail size and intended occlusion.
- Required states: before/after (broken/restored, inactive/active) that must produce **functional changes**, not merely an image swap.
- Existing approved palette, shape language, characters and current source meshes; reuse first.
- Geometry budget, material budget, runtime target and relevant export constraints.
- Allowed artist/generator tools, plan rights, reference redistribution restrictions and one writer per editable file.

A scene concept approved for **mood** is not automatically accepted as source geometry, rigging, material maps or playable traversal.

## 2. Define an art-direction grammar, not just a prompt

A useful style bible answers five questions at a **gameplay camera scale**:

| Dimension | Define | Test at runtime |
| --- | --- | --- |
| Silhouette and recurring motifs | Repeated shape vocabulary and deliberately missing elements | Landmark readable at gameplay zoom without labels |
| Value structure | Dark/light grouping, maximum simultaneous highlights and focus hierarchy | Player, interaction target and route remain distinguishable |
| Color roles | Base materials, local emissive source, state-change accent, accessible alternative cues | Broken/restored states separable without color alone |
| Architecture/materials | Scale, modular joints, trim language, roughness/metal families and weathering logic | No seam, 100× import discrepancy or unlicensed texture |
| Motion/sound implication | How a repaired object moves or illuminates; what requires animation | A functional game-system event drives final visuals |

Separate **approved facts**, **proposals** and **playtest tuning**. Never phrase a generator's interpretation as already approved art direction. Store actual numeric choices only once in the canonical style bible.

## 3. Build a useful reference set

Do not substitute a single scenic illustration for every required view. Record a reference index with four distinct types:

1. **Whole-world direction:** sky, palette, staging, architecture density, weather, lighting and mood.
2. **Landmark establishing view:** approach, elevation, player route, occluders, before/after state.
3. **Single-asset clean orthographic views:** front, side, back and top where applicable, scale cues, unobstructed silhouette, no scenic background touching the object.
4. **Material and animation details:** stone/metal/wood families, motifs, damage, pivots, skeleton pose and transform expectations.

For procedural or AI 3D services, a clean subject image is often a better modeling input than a picturesque environment composition. A coherent multi-view character sheet must match in head size, hands, feet, costume and pose; inconsistent views are not solved by a “more detailed” prompt.

**Reference rights:** Record who made each image, where it came from, allowed use, whether prompt context may legally contain it, storage restrictions and whether it can appear publicly. A mood-board screenshot found on the Internet is not automatically redistributable or permitted as generator input.

## 4. Bridge 2D direction to a real 3D specification

Use the existing [asset brief template](../templates/asset-brief.md); do not create a duplicate document competing with that canonical form.

A generation-ready brief includes:

- Stable ID, asset category and **real intended game role**.
- Approved reference IDs and preferred angles, with *excluded* motifs and rejected styles.
- Dimensions **in Blender metres** and corresponding **Unreal centimetres** (1 m = 100 cm), forward/up axes and pivot.
- Assembly contract: tile/grid dimensions, mating edges, material slots and broken/restored variants.
- Technical source and export: real `.blend`, `.fbx` or `.glb`, UV sets, material maps, lod/collision needs.
- For characters: required skeleton hierarchy, rest pose, named clips, root motion, deform-only bones and neutral arms.
- Performance limits chosen from actual project profiling rather than asserted universal triangle counts.
- Legal provenance fields, license and generated output rights; approval/QA owner.
- Reimport and human-acceptance criteria at the **real gameplay camera**.

A “prompt” is only a tool-facing rendering instruction within this bigger contract. Never claim prompt text equals geometry or a final asset.

## 5. Worked educational example: a two-state modular lantern pedestal

The complete [worked brief](../examples/visual-art-brief/README.md) describes a **hypothetical** small level landmark: a cracked stone lantern pedestal that, once repaired by actual gameplay, lights a corridor. Its purpose is to connect shape/palette/scale with collision, material identity, art rights and source/export requirements.

The example is **not** a claim that this asset already exists or that anyone approved it for a game. It is an instructional specification for an authorized operator to adapt to a real level.

Example asset-family properties in the brief:

| Field | Proposed exercise value | How to verify |
| --- | --- | --- |
| Grid | 2 m modular spacing; 200 Unreal units | Actual bounding boxes and adjacent placement |
| Origin | Local pivot at base center | Blender origin and UE rotation/placement |
| Visual states | Damaged / Powered (same physical base) | Separate game state drives a light/visual change |
| Mesh | A modest readable stone/metal assembly, authored source retained | Inspect real geometry, silhouette, normals, UVs and material assignments |
| Collision | Solid base, no invisible overhang barrier | Actual Player capsule walks around the asset |
| Source/export | Original `.blend` and verified `.fbx` per required mesh | File existence, import report and measured bounds |
| Provenance | Generator or original-author license and output rights | Explicit evidence, not inferred permission |

## 6. Producer handoff to Blender, Tripo, Meshy or other tools

Choose the smallest suitable tool for the approved geometry:

- **Manual modeling / Blender Python:** strong for modular dimensions, pivots, topology repairs, UV editing and repeatable parameterized assemblies.
- **Generation service:** useful for original organic candidate meshes, but demands review of source/export rights, topology, thickness, UV and rigging.
- **Unreal Modeling Mode:** sometimes faster for final functional blockout or static structural pieces than round-tripping a DCC.
- **Reference image generator:** useful for selected concept sheets but **not** a source of editable mesh geometry by itself.

The operator owns **exact source files** and reports actual tool/model versions, input reference rights, prompts/settings where relevant, source/export paths, measurements, warnings, evidence and license. A different person owns Unreal imports and scene integration. No operator may treat uncreated files as complete.

Do not send an art agent unrelated private project paths, personal reference images or unapproved source materials. One combined approved brief should govern every attempt; revisions update that brief, not detach an incompatible secondary prompt.

## 7. Evaluate candidate concepts before generating a whole asset batch

For each genuine candidate, compare:

1. **Silhouette at intended camera distance.**
2. Gameplay readability: visual state, target focus and actual route.
3. Compatibility with approved style grammar across adjacent architecture.
4. Reusable module boundaries and geometry demand.
5. Likely rig/retopology/import effort, available time and tool budget.
6. Licensing and publication eligibility.

Reject alternatives with a reason. Keep only the human-selected direction as the production authority. A composition can win for the art book but lose as a gameplay landmark if interaction and collision become unreadable.

### Example revision contract

A valid revision record might say: *“Proposal: move decorative spires outward so the Character capsule can pass; retain the original 2 m base, palette and pivot. Approval required.”* This is a **design request**, not evidence that a Blender export or Unreal level was edited.

## 8. Actual production acceptance matrix

| ID | Required evidence | Accept only when |
| --- | --- | --- |
| ART-01 | Real project's current GDD/level/camera context | Relevant implementation and scale assumptions inspected |
| ART-02 | Approved style bible and references | Human confirms silhouette, palette, material and world logic |
| ART-03 | Original/sourced reference provenance | Use and publication rights explicitly checked |
| ART-04 | Whole-world concept and landmark view | Same location and player route are depicted coherently |
| ART-05 | Clean single-asset views | Subject isolated and technically consistent |
| ART-06 | Broken/restored visual contract | Functional changes and state-color cues defined |
| ART-07 | Approved asset brief | Real owners, IDs, size, pivot, format, performance and rights |
| ART-08 | Actual generator/Blender source file | File exists, not merely described |
| ART-09 | Export geometry QA | Normals, UV, bounds, scale, materials and source provenance checked |
| ART-10 | Unreal import/reimport | Asset actually imported with correct slot/scale/collision |
| ART-11 | Level integration at actual camera | Silhouette/readability and traversal verified in PIE |
| ART-12 | Final license/evidence record | No unauthorized asset redistribution |

**T10 publication is a documentation deliverable.** ART-08 through ART-12 remain NOT RUN/NOT CREATED in this repository. Mark per-asset status as Concept → Source Created → Export Verified → Imported → Integrated → Editor Checked → Runtime Tested → Human Validated.

## 9. Frequent failures and recovery

| Failure | Correct response |
| --- | --- |
| Scenic concept produces fused multi-object mesh | Produce a clean single-object reference with consistent scale/angles |
| Every new asset changes architectural language | Reuse bible and compare candidate at the same camera/framing |
| Regeneration makes new “identical” items | Stable IDs, source revisions and explicit human selection |
| No measurable pivot/scale | Return brief before spending export/import time |
| Texture rights are unknown | Block redistribution and production import until authorized |
| Imported mesh differs 100× in size | Inspect source units, exporter scaling and actual Engine bounds |
| Broken/restored materials look different but collision stays blocked | Correct the gameplay authority and collision integration |
| An AI-generated render is called a “finished asset” | Reclassify as Reference Only until source/export/import/gameplay exist |
| Agent changes unrelated geometry or dirty package | Preserve changes; return to exact file/Editor owner and report conflict |

## 10. Next

Continue with [T11 — Blender Modeling, Retopology, UV and PBR](README.md) for real source geometry and an export/QA workflow. For character motion and rest-pose auditing, follow [T12](README.md); for actual native Unreal imports, [T13](README.md); for playable assembled level geometry, [T14](README.md).

**Authoritative reading:** [Blender manual](https://docs.blender.org/manual/en/latest/), [Epic Modeling and Geometry Scripting](https://dev.epicgames.com/documentation/unreal-engine/modeling-and-geometry-scripting-in-unreal-engine), [asset provenance template](../templates/asset-brief.md), [third-party rights](../THIRD_PARTY.md).
