# Blender-to-Game Pipeline — Manual or Agent-Operated

Blender can be operated manually or by an authorized agent such as Antigravity, Codex or Claude with a **working Blender interface, script execution and file permissions**. A generated answer about Blender is not proof that Blender modified a file.

## 01. Source and handoff

A valid asset brief identifies the actual gameplay purpose, approved reference view(s), target dimensions, coordinate conventions, silhouette, model limits, rig/animation requirements, material maps, legal provenance, expected real export format and receiving engine operator.

Keep source models, raw generator downloads, processed exports and target Unreal packages clearly separated. Do not fabricate files or create empty folders simply to satisfy a naming pattern.

**Runnable modeling tutorial:** [T11 — Blender Modeling, Retopology, UV and PBR](../../tutorials/blender-modeling-uv-pbr-export.md) includes a [real three-mesh modular generator and checksum manifest](../../examples/blender-modular-kit/README.md). Its source is published, but Blender execution and actual Unreal import remain NOT RUN until performed.

## 02. Modeling and cleanup

- Audit generated/source mesh: disconnected objects, geometry intersections, normal direction, thin/degenerate surfaces, scale and bounding box.
- Set correct object transforms and coordinate convention; note Blender metres versus Unreal centimetres.
- Use retopology, UV unwrap, material remapping and texture baking as required by the asset rather than routinely.
- Validate PBR map color space: base color/emission generally color data; normals, metallic, roughness and AO generally linear data. Verify the **actual source map encoding**, not only a filename.
- For modular architecture, validate compatible pivots, bounds and one-to-one broken/restored positional alignment.

**T12 rigging/IK retarget tutorial:** [Character Rigging, Animation and Unreal IK Retargeting](../../tutorials/character-rigging-animation-retarget.md) includes an [original read-only Blender rig audit](../../examples/blender-rig-audit/README.md) comparing bone hierarchy/rest pose and sampling real animations before separate Unreal retarget validation.

## 03. Rigging and animation

- Agree on reference pose, axis, bone hierarchy, bone naming and handedness.
- Apply skinning and check problematic shoulders, wrists, pelvis, knees and feet.
- Build only genuine, useful clips (Idle, Walk, Interact, React, Awakening, etc.) and verify them on the real mesh.
- For animation-only exports, confirm root scale, translation units and bind pose in a reimport. Imported motion can appear 100× smaller than the same character mesh.
- Shared bone names do not prove shared skeleton compatibility. Compare **rest poses and local transforms**. Retarget or use separate skeletons where required.
- Do not claim an animation graph or cross-fade exists until its authoring and runtime behavior are verified.

## 04. Real export test

Before exporting a large batch, perform a representative round trip:

1. Export a final mesh/clip in the supported FBX/GLB format.
2. Import into the chosen game engine.
3. Compare geometry counts, bounds, materials, bone mapping, axes, pose and sampled animation frames.
4. Report importer warnings and distinguish benign import warnings from deforming geometry or hidden compatibility problems.
5. Correct the pipeline, re-export and repeat until the actual target artifacts pass.

A batch exporter can store SHA-256 hashes and a manifest, but it does **not** guarantee the generated files are aesthetically or legally acceptable.

## 05. Game integration gate

The designated Unreal operator imports and configures meshes, materials, collision, IK Rig/Retargeter, Animation Blueprint, actor variants and gameplay event bindings. Verify visibility at gameplay scale, runtime animation, streaming, packaging and human acceptance. Imported ≠ Integrated ≠ Tested.

## Agent task contract

A Blender agent must report versions, commands/operations actually run, exact relative paths changed, asset counts, errors, source/export license basis, screenshots where permitted, and real QA results. One editor owner should not overwrite another operator's unsaved `.blend`.

See [The Unmade Heart Kin evidence](../../case-studies/the-unmade/README.md).