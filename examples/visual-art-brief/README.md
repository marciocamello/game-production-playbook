# Worked Art Brief — Two-State Modular Lantern Pedestal

**Type:** Educational, original fictional specification. **Status:** Design example only. **No reference image, source mesh, export or Unreal asset exists as part of this T10 example.** It has not been approved for use in any particular game.

Use [T10](../../tutorials/visual-references-generation-ready-briefs.md) and the [canonical asset brief](../../templates/asset-brief.md) to adapt this example to a real project's approved feature.

## Player-facing purpose

A stone-and-metal lamp pedestal is restored by a **real gameplay transaction**. Its light reveals the next readable path, while world state, passage collision and any character costs remain owned by the game's progression system. This brief controls **visual design**, not gameplay authority.

## Proposed art grammar

- Silhouette: short broken Gothic stone prism, offset metal upper ring, one deliberately absent decorative segment in Damaged state.
- Material families: rough dark stone, desaturated aged metal and very small warm emitter. Two-state transformation must remain readable without color alone.
- Damaged state: no active emission, visually cracked stone, missing segment not physically obstructing passage.
- Powered state: stable reassembled segment, enabled fixture and warm bloom under actual game exposure, with no change to primary physical footprint.
- Exclude: excessive spikes blocking movement, glowing outlines in the inactive state, floating supportless rubble, unapproved photo textures.
- Camera validation: equivalent fixed elevated camera in both before/after captures; object readable when the player stands several metres away.

## Proposed dimensional contract (must be approved)

| Item | Value |
| --- | --- |
| Module footprint | 2.0 m × 2.0 m (200 × 200 UE centimetres) |
| Base height | 0.6 m |
| Total height | At most 3.0 m; confirm camera clearance |
| Origin | Base center at local Z=0 |
| Modular attachment | 2.0 m grid, bounding footprint must not drift between states |
| Facing | Author in source and record Blender-to-Unreal forward axis |
| Materials | Named slots for stone, metal and emitter (actual materials created later) |
| Collision | Solid stone base only; physical capsule cannot pass through it; decorative cap cannot create an invisible wall |
| Variants | `Damaged` and `Powered` share stable spatial identity |
| Export | Original `.blend`, then real `.fbx`; Unreal operator verifies import |

## Reference production

Prepare **four distinct reference artifacts** only after real approval:

1. A broad environment/landmark composition that communicates atmosphere and path.
2. A close three-quarter view showing material/shape detail.
3. Single-asset orthographic front, side and top views on a plain backdrop.
4. A controlled before/after comparison with matched position, camera, scale and exposure.

Each reference record must include author/source, date, license/permission, whether its original is redistributable and whether a 3D generator may process it.

## Generation or manual modeling assignment

An approved operator receives the current brief plus **only rights-cleared references**. They must deliver actual editable source geometry, separate export files, a geometry QA record, corrected UVs, consistent material slots and an import handoff. They must not invent a private source link, use an unapproved logo, or mark the asset integrated.

If the approved tool is Blender, T11 supplies an original modular-mesh exercise. If a generator is used, document its real model/version, prompt history, output rights, topology/UV corrections, and the receiving source/export IDs.

## Ownership and status record

| Stage | Owner | What must actually exist | Status at publication |
| --- | --- | --- | --- |
| Creative approval | Human game director | Approved real-game brief | NOT APPROVED |
| References | Concept artist or agent | Rights-cleared reference images | NOT CREATED |
| Mesh source | Blender/operator | Editable real mesh with UV/materials | NOT CREATED |
| Export QA | Mesh owner | Real FBX/GLB and measured geometry | NOT RUN |
| Unreal import | Sole Editor operator | Saved Static Mesh and material assets | NOT RUN |
| PIE | Human tester | Real route and two-state visual correctness | NOT RUN |
| Packaged check | Release operator | New built executable and observed behavior | NOT RUN |

These explicit gaps prevent an illustrative brief from being mistaken for a shipped art asset.
