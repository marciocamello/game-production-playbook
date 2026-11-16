# T12 — Character Rigging, Animation and Unreal Engine IK Retargeting

**Publication state:** Complete real-asset procedure with an original [read-only Blender rig audit tool](../examples/blender-rig-audit/README.md). **No rig, animation, FBX export, Unreal Skeletal Mesh, IK Rig, Retargeter or AnimBP was created or tested in a live environment by this publication.** The script audits the user's genuine existing source only when executed.

**Target:** Blender rigged characters and Unreal Engine 5.9, with Source/Target Skeletal Meshes, IK Rig Assets and IK Retargeter. Prefer the actual installed Engine's behavior over assumptions based on older documentation.

## 1. Why identical bone names are insufficient

Two characters can share 80 bone names and still have different **parent hierarchies**, **rest matrices**, **bone orientations**, root scale or pelvis placement. Reusing one Unreal Skeleton on incompatible bind poses can distort a mesh. Retargeting is intended to handle differences, **not** to make separate rest poses magically identical.

A historical production export had animation-only FBX translations roughly 100× smaller than the mesh import; a visually collapsing character was observed until the import scale was corrected. This is evidence that *animation-only export settings need separate QA*, not proof that a global scale of 100 fixes every file.

## 2. Inspect actual source and animation contracts

Before rigging: read the approved character reference and real game movement/camera requirements; inspect existing armatures, source mesh, required clips, skeleton bindings, vertex groups, root motion usage and current Unreal animation assets.

- Establish neutral **rest pose** and consistent arm/leg spread suitable for rigging.
- Record actual root, pelvis, spine, limbs and optional IK/twist bones; no fixed universal skeleton is required.
- Compare **parents and matrices**, not just names.
- Define which clips are needed by gameplay: Idle, Walk, Interact, Gift, Recovery or others actually approved.
- Record whether motion is in-place or root-motion and whether turn/foot contact matters.
- Confirm ownership of the Blender file; avoid destructive resets or copying a separate private project's rig.

## 3. Run a real, read-only rig audit

The [original Python audit](../examples/blender-rig-audit/scripts/audit_existing_rig.py) reads the existing `.blend` in an isolated Blender process. It validates that the named Armature and Mesh are real, records bone parents/rest matrices/weights and samples the armature's **active** Action at three frames. It optionally compares a second real Armature.

```powershell
blender --background "$env:GPP_EXISTING_RIG_BLEND" `
  --python ./examples/blender-rig-audit/scripts/audit_existing_rig.py -- `
  --armature "$env:GPP_EXISTING_ARMATURE_NAME" `
  --mesh "$env:GPP_EXISTING_SKINNED_MESH_NAME" `
  --report "$env:GPP_NEW_AUDIT_REPORT"
```

**Non-destructive:** report file must not already exist; the process saves no Blender source. The script cannot guarantee that every Animation Action drives that rig, that rest poses are artistically good or that Unreal will accept the export. No fabricated animation clips are supplied.

## 4. Retopology, weights and deformation

Use a genuine skinned mesh. In Blender:

1. Inspect non-manifold geometry, disconnected islands and normals.
2. Check vertex groups against actual deform bones; locate zero-weight vertices and unintended bone influences.
3. Weight-paint joint bends, shoulders, knees, spine and fingers; examine sampled extreme rotations, not just neutral pose.
4. Confirm Armature modifier points to the correct rig, neutral pose matches reference, skin binding is intact.
5. Check mesh and armature object scales; choose one consistent export convention and document it.
6. If multiple characters use a common animation family, verify bone hierarchy/orientation and proportions. Use retargeting rather than force-sharing a Skeleton when incompatible.
7. Keep a real original `.blend` source under the approved owner and mark each action as only as far validated as actually observed.

## 5. Export one real representative mesh first

The source owner exports an existing rigged mesh **and its actual Armature** using supported Blender FBX settings. A starting convention for an Unreal import is forward `-Y`, up `Z`, selected objects and disabled leaf bones, but the actual importer/settings may require adjustment.

Verify file bytes, file names, FBX exporter logs, rest pose, units, root orientation, material slots and bind weights. The owner must distinguish source mesh, source action, export FBX, imported Skeletal Mesh and Skeleton assets.

Do **not** overwrite another operator's existing Skeleton asset or use “Update Skeleton Reference Pose” without inspecting consequences. First import to an owned test namespace and compare the actual Skeletal Mesh preview.

## 6. Export animation-only FBX separately

For an existing authored clip:

- Select the **correct existing armature** with its active animation and confirmed action slot/layer.
- Verify actual frame range and sampling (start, mid, end), root/pelvis translation values, action fps and loop seam.
- Export a dedicated animation FBX with actual armature bones and baked animation; explicitly exclude unrelated mesh/objects if that is the chosen import path.
- Import onto the intended **compatible** Unreal Skeleton asset. If an animation-only import indicates an implausible pelvis/root displacement, compare measured source and imported translations before changing scale.
- A corrective 100× importer adjustment may be appropriate for a particular bad file, but only after numeric evidence and a repeat import. Never apply it to all characters by default.
- Validate Idle and Walk separately from mesh export. A mesh that imports at correct height does **not** prove action translation has correct units.

## 7. Build real IK Rig and Retargeter assets

For characters with different Skeleton assets, use Unreal's **IK Rig** and **IK Retargeter** workflow, with version-specific settings:

1. Import the actual Source and Target Skeletal Meshes as separate assets with legitimate Skeletons.
2. Create and save **one IK Rig for each appropriate Skeleton**, select retarget roots and define real bone chains: pelvis/root, spine, arms, legs and other required limbs.
3. Create a valid IK Retargeter referencing both existing IK Rigs; map chains deliberately and inspect unmapped joints.
4. Set comparable base poses and address orientation/rest differences, rather than merely matching name strings.
5. Preview an actual source Idle/Walk/Gift clip on the target; inspect feet contact, stride, pelvis height and root trajectory.
6. If beneficial and available in UE 5.9, evaluate the **Retargeting Operation Stack** and 5.9 spatially aware operations. These are enhancements, **not mandatory** to declare a simple rigged character valid.
7. Export/duplicate retargeted animations through the real Editor when required by the approved game and verify them after saving/reopening.
8. Wire the actual Animation Blueprint/state transitions to the real Character/Gameplay system and check blending (T16 covers cinematic integration separately).

A Retargeter asset existing in Content Browser does not prove that its mappings, reference pose and animations work.

## 8. Validate deformation and playback at the game camera

Require sampled before/after evidence in the actual skeletal preview and gameplay:

- Stand/idle neutral: no collapsed upper body, inverted hands or missing mesh sections.
- Walk: root/pelvis motion sensible, foot contact and no extreme sliding; correct game velocity.
- Interaction: arms reach the object at actual scale and center-of-mass/feet remain stable.
- Gift/recovery: animation ends and gameplay input regains control; lost ability stays unavailable until gameplay recovery.
- Retargeted variants: same event timing with acceptable clip-specific adjustments.
- Game camera: small silhouette and hands readable even if the rig passes close-up QA.

Animation quality must be inspected with **active motion**, not one screenshot of a neutral pose.

## 9. Evidence cases

| ID | Required observation | Authority |
| --- | --- | --- |
| RIG-01 | Existing .blend and actual meshes/armature inspected | Blender owner |
| RIG-02 | Real bone hierarchy/parents/matrices report generated | Blender owner |
| RIG-03 | Rest poses compared, differences resolved or documented | Technical animator |
| RIG-04 | Mesh weights/non-manifold/shading corrected | DCC owner |
| RIG-05 | Required authored Actions exist and frame ranges inspected | Animator |
| RIG-06 | Idle/Walk extremes sampled in Blender | Animator |
| RIG-07 | Skeletal mesh FBX exported with real armature | Export owner |
| RIG-08 | Skeletal Mesh/Skeleton imported and saved in Unreal | Editor owner |
| RIG-09 | Animation-only FBX separately exported and measured | Animator |
| RIG-10 | Animation-only asset imported; root/pelvis scale correct | Editor owner |
| RIG-11 | Source/Target IK Rig assets saved | Editor owner |
| RIG-12 | IK Retargeter maps actual chains and base pose | Editor owner |
| RIG-13 | Retargeted clips preview without collapse/foot failure | Animator/Editor |
| RIG-14 | AnimBP state changes/blending implemented if required | Editor owner |
| RIG-15 | Human PIE sees actual animation gameplay and camera | Human reviewer |
| RIG-16 | Windows packaged launch plays intended clips | Release operator |

Each is **NOT RUN** in this publication. A read-only JSON audit can establish only source inspection, not RIG-07..RIG-16.

## 10. Failure matrix

| Observed symptom | Root-cause investigation |
| --- | --- |
| “80 matching bones” but distorted mesh | Parent graph, rest matrices and binding differ |
| Character collapses after animation import | Source-vs-imported root/pelvis translation and FBX unit mismatch |
| Mesh floats above floor | Root motion, pelvis retarget root, floor alignment |
| Mesh imports but animations missing | Wrong FBX action/bake config or missing animation import |
| Foot slide despite correct pose | Stride, target proportions, speed/root motion mismatch |
| Hands twist | Joint orientation, retarget chain/base pose mismatch |
| Retargeter looks correct but idle/walk snaps | Transition/Animation Blueprint graph not actually authored |
| Editor import success but game uses static pose | Wrong mesh Anim Class, asset binding or animation state |
| MCP cannot author AnimGraph | Use supported Editor/Control Rig/API route and report limitation |

**Next:** [T13 — Unreal Asset Import and Actor/Material Integration](README.md). For production process and current role boundaries, see [T04](multi-agent-production-handoffs.md).

**Official references:** [Epic UE 5.9 release notes](https://dev.epicgames.com/documentation/en-us/unreal-engine/unreal-engine-5-9-release-notes), [Epic IK Rig/retargeting](https://dev.epicgames.com/documentation/unreal-engine/ik-rig-animation-retargeting-in-unreal-engine), [Epic 5.9 Python IK Rig](https://dev.epicgames.com/documentation/unreal-engine/using-python-to-create-and-edit-ik-rigs-in-unreal-engine), [FBX animation import](https://dev.epicgames.com/documentation/unreal-engine/fbx-animation-pipeline-in-unreal-engine).
