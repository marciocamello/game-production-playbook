# T12 Example — Read-Only Blender Rig / Rest Pose Audit

[Script: audit_existing_rig.py](scripts/audit_existing_rig.py) inspects **an existing, user-supplied real Blender rig**, not an invented skeleton. It does not create meshes/animations, write FBX, modify or save the input `.blend`.

## Run it in a new Blender background process

```powershell
blender --background "$env:GPP_EXISTING_RIG_BLEND" `
  --python ./examples/blender-rig-audit/scripts/audit_existing_rig.py -- `
  --armature "$env:GPP_EXISTING_ARMATURE_NAME" `
  --mesh "$env:GPP_EXISTING_SKINNED_MESH_NAME" `
  --required-actions Idle Walk `
  --report "$env:GPP_NEW_AUDIT_REPORT"
```

Set all environment variables to real authorized values. The report path must not exist and its parent must exist. Omit `--required-actions` or supply **actual** action names from the file; `Idle` and `Walk` are illustrative. Optional `--compare-armature OtherRigName` compares full hierarchy, rest matrices and parent identities when two actual armatures are loaded in the source file.

## What is inspected

- Bone names, parents, rest matrices, bone lengths and deform flags (not only a bone count).
- Mesh vertex weights and Armature modifiers (including zero-weight vertices).
- Active animation action sampling at beginning, midpoint and end, with root world positions.
- Existing required action names; the tool fails when a required action is missing.
- Non-identity object scales, reportable in metre units for later unit checks.
- If supplied, another rig's hierarchy and rest matrix differences.

The script records **Unreal import, IK Retargeter, animation-only FBX scale and human deformation inspection as NOT RUN**. These require actual Editor imports, proper retarget chains and sampled animation playback. A report showing matching bone names cannot prove compatible skeleton bind poses.

## Limitations and safe use

- Blender Action slots/layered actions can affect which actions are actively evaluated; this script samples **only the armature's active action**, not every action in the file.
- It does not claim a full animation export, mesh deformation, physics asset, AnimGraph or gameplay-ready retarget.
- Parent/bone comparisons are numeric diagnostic evidence; characters with different proportions may appropriately use distinct Skeletons plus IK Retargeter.
- Never publish the report if it reveals private rig names, source file paths or licensing-restricted content.

Read [T12 — Rigging, Animation and Retargeting](../../tutorials/character-rigging-animation-retarget.md). Original source is MIT-designated under [LICENSE-CODE.md](../../LICENSE-CODE.md). The input .blend and its original animations retain their own licenses.
