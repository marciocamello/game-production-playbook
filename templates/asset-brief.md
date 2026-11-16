# Template — Production Asset Brief and Provenance

**Use only for an asset with approved purpose.** A reference sheet is not an exported model, and an exported model is not a game-integrated asset.

## Brief contents

- Stable asset ID and player-visible role.
- Approved concept/reference version; silhouette and style constraints.
- Which state variants are needed (e.g. broken/restored).
- Approximate dimensions, units, orientation, pivot and gameplay camera viewing distance.
- Structural/decorative/interactive classification and collision expectations.
- Expected topology, materials/PBR maps, UV layout, LOD/Nanite choice and performance limits.
- If animated: bone hierarchy, rest pose, export convention and required named clips.
- Primary authoring method (manual Blender, agent-operated Blender, generator or Editor Modeling Mode).
- Source file and working export formats; receiving Unreal content location.
- Quality gates, reimport comparison and human acceptance.

## Provenance record after real production

| Field | Required evidence |
| --- | --- |
| Source | Author/provider, license, download or original generation date |
| AI tooling if used | Product/model/version, prompt reference and rights basis |
| Editable master | Existing source file and revision |
| Export | Actual FBX/GLB or sound/texture file and format |
| Technical checks | Dimensions, normals, UV, materials, rig/reimport validation |
| Engine | Destination package, importer warnings, version and real integration stage |
| Runtime | Visibility, animation, gameplay relationship, performance and packaging |
| Credit/reuse | Attribution, commercial permissions and redistribution status |

If an entry does not yet exist, mark it **NOT CREATED / NOT RUN** rather than fabricating filenames or hashes.

## Acceptance

Confirm that the object can actually be used in the approved scene, has valid rights, meets the established visual language, and does not break traversal, camera or gameplay.