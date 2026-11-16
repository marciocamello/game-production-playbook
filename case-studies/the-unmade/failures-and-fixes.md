# The Unmade — Engineering Failures, Corrections and Evidence Limits

These examples were selected because they teach **failure containment and evidence quality**, not because every related feature was ultimately human-validated.

## 1. Camera collision and constant framing

The early follow-camera implementation could call its Spring Arm's inherited collision update with retraction enabled. The camera contracted near walls, violating an intended stable elevated framing. A design-only change to interpolation limits would not solve the observed root cause. The subsequent dedicated camera work addressed stable framing and added more modes, but still required authored camera assets and human PIE.

**Lesson:** verify the real inherited Engine behavior and test with actual occluders, stairs and character collision. A native camera component does not by itself constitute a configured gameplay camera.

## 2. A single missing Camera Asset invalidated suite completeness

F006's executor recorded successful Editor/Development/Shipping targets and a successful cook/package, alongside **34 passing tests of 35**. The failed case was the real authored-camera/input integration check because the required Camera Asset was null. The UAT/process exit alone would have incorrectly suggested all tests passed. The final archive was reported as not launched.

**Lesson:** inspect individual test results; treat a missing asset as a real failure, never auto-substitute an empty test asset or declare final gameplay success based on an archive.

## 3. Blender-to-Unreal animation translations collapsed

The Heart Kin workflow produced rigs with 80 bone names, yet their different rest poses prevented simply forcing all characters onto one Skeleton. Animation-only FBX exports initially imported bone translation values roughly **100× too small**, causing an apparent collapse. The Editor operator corrected import scale and verified sampled deformation/preview, with bone/pelvis handling during retargeting.

**Lesson:** verify axis and unit conventions **for animation exports separately from meshes**. Shared names are insufficient; check parent graph, bind/rest pose and sampled transforms.

## 4. AnimGraph authoring was not available through the chosen MCP DSL route

The Editor integration report states that the operator could not create the required AnimGraph nodes through the available toolset. Clip switching was implemented without the intended cross-fade; no invented graph was claimed.

**Lesson:** tool discovery must precede task planning; limitations require a supported fallback or clearly recorded functional compromise.

## 5. Unsaved Editor packages and ownership conflict

During visual/material integration, three pre-existing dirty material/texture packages had uncertain ownership. A later operator deliberately preserved them rather than saving over them. A Blueprint edit rollback briefly affected loaded Editor enumeration, and reloading a saved map restored it. Another environment pass identified tree-Actor deletions outside its owned changes and explicitly excluded them from its commit.

**Lesson:** “file exists” does not imply “safe to overwrite.” Record loaded map, dirty packages, sole editor owner and precise asset diffs. Preserve unknown work.

## 6. Material correctness versus actual visual quality

The architectural material pass changed stone, cliff, moss and paving logic and compiled relevant shaders. Evidence included geometry counts, import/compilation checks and matched Editor review captures. Reports explicitly note that the result remained visually simpler than the concept art and that Editor screenshots were not runtime captures.

**Lesson:** separate material compile PASS, recorded geometry edits, visual direction acceptance and packaged GPU/performance review.

## 7. Deterministic world state versus reconstruction effects

The intended bridge uses permanent gameplay state for traversability and blocking collision. Chaos/Sequencer/Niagara can show a reconstruction sequence but cannot become the owner of bridge progress. Later reports describe real Editor graph changes while interruption, streaming and full traversal remained untested in those passes.

**Lesson:** commit once, apply state idempotently on reload, and test interrupted effects. Preserve a fallback path where nonessential presentation fails.

## 8. UI authored but not necessarily accepted

F005 developed native UI/session services and Editor widgets; subsequent reports corrected controls layout, HUD notifications and a main-menu camera. These checks had real Editor results but explicitly requested human PIE for navigation and visual behavior.

**Lesson:** a compiled WBP and correct screenshot layout do not prove that the player can actually open menus, remap controls, interact, save or return to gameplay.

## 9. Captures are not test substitutes

The visual pass explicitly described some screenshots as Editor views approximating gameplay camera orientation. They are useful to compare lighting or materials but cannot prove actual player-camera framing, collision or saved-world transitions.

**Lesson:** tag each artifact as reference image, Editor evidence, PIE capture or packaged runtime evidence.