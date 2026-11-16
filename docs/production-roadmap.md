# Complete Game Production Roadmap — 197 Work Items

This is a **reference process**, not an assertion that all work was performed in The Unmade. Use it to define your own project scope. Many steps can be combined; some engine-specific steps can be omitted when unnecessary. A missing optional tool never means that a required gameplay behavior can be faked.

**Task execution:** each relevant step produces a concrete deliverable, an identified owner, acceptance criteria, failures/risks, and traceable evidence. For every historical case, distinguish `Planned`, `In Progress`, `Implemented`, `Tested`, `Human Validated`, and `Released`.

**Mode independence:** a human, a human assisted by AI, or a tool-using agent may execute a task. Ownership and verification gates do not change.

## 01 — Discovery and project definition

| ID | Work item |
| --- | --- |
| P001 | Identify why the game should exist |
| P002 | Define audience and target platforms |
| P003 | Choose independent, commercial, academic or jam context |
| P004 | Identify deadline, budget, team and hard constraints |
| P005 | Research comparable games and differentiation |
| P006 | Collect ideas before committing |
| P007 | Compare fun, originality, feasibility and cost |
| P008 | Record rejected hypotheses and why |
| P009 | Select the primary concept |
| P010 | Approve a project charter |

**Exit evidence:** Project charter, audience, constraints and a documented concept decision.

## 02 — Rules, preparation and tools

| ID | Work item |
| --- | --- |
| P011 | Study rules, legal requirements and license constraints |
| P012 | Separate preparation from production-limited activities |
| P013 | Select the engine and engine version |
| P014 | Compare C++, Blueprints, C#, GDScript or other approaches |
| P015 | Identify distribution platforms and baseline requirements |
| P016 | Choose modeling, animation, editing and sound tools |
| P017 | Research AI tool capabilities and restrictions |
| P018 | Budget credits, subscription quotas and storage |
| P019 | Set up repository, ignore rules and version conventions |
| P020 | Define privacy, provenance and security rules |

**Exit evidence:** Approved tool matrix, rights checklist, clean repository baseline.

## 03 — Production process

| ID | Work item |
| --- | --- |
| P021 | Assign accountable human roles |
| P022 | Define optional agent roles |
| P023 | Write operating instructions |
| P024 | Approve project constitution and conventions |
| P025 | Organize skills and context routing |
| P026 | Identify canonical document owners |
| P027 | Create an actionable production backlog |
| P028 | Assign exclusive owners per editable file and asset |
| P029 | Assign exclusive operators to shared stateful editors |
| P030 | Define safe branches, commits and integration |
| P031 | Define code-art-editor handoff boundaries |
| P032 | Define evidence vocabulary and gates |
| P033 | Define review, rework and acceptance policy |

**Exit evidence:** A working ownership matrix and task-handoff contract.

## 04 — Narrative and game design

| ID | Work item |
| --- | --- |
| P034 | Write the narrative premise |
| P035 | Define theme, tone and core promise |
| P036 | Develop protagonist and supporting roles |
| P037 | Design conflict, emotional arc and ending |
| P038 | Identify gameplay pillars |
| P039 | Design the core gameplay loop |
| P040 | Specify abilities, actions and consequences |
| P041 | Design puzzles, challenges and progression |
| P042 | Author a Game Design Document with approvals |
| P043 | Separate core, desirable and optional scope |
| P044 | Identify softlock and scope risks |
| P045 | Approve design before large-scale implementation |

**Exit evidence:** Approved GDD, narrative, critical path and scope-cut list.

## 05 — Technical architecture

| ID | Work item |
| --- | --- |
| P046 | Audit existing implementations and dependencies |
| P047 | Split authoritative code from presentation assets |
| P048 | Define entities, components and interfaces |
| P049 | Select gameplay authority and state ownership |
| P050 | Design abilities/effects, using GAS if justified |
| P051 | Specify input, cameras, interaction and control |
| P052 | Design progress, world and player persistence |
| P053 | Design UI presentation architecture such as MVVM |
| P054 | Define messages, events and synchronization |
| P055 | Organize modules, assets and plugins |
| P056 | Define builds, test layers and packaging strategy |
| P057 | Approve TDD and architecture decisions |

**Exit evidence:** Technical design, state ownership, risks and verified dependency choices.

## 06 — Visual direction

| ID | Work item |
| --- | --- |
| P058 | Collect art references with rights and provenance |
| P059 | Write the art bible |
| P060 | Define palettes, materials, form vocabulary and lighting |
| P061 | Design silhouette and character anatomy |
| P062 | Produce concept sheets and consistent multiviews |
| P063 | Develop global environment concepts |
| P064 | Compare before/after or other world-state concepts |
| P065 | Specify modular architectural and prop vocabulary |
| P066 | Check consistency across images |
| P067 | Approve visual references before 3D production |

**Exit evidence:** Approved art bible, references and actionable asset briefs.

## 07 — Level and spatial design

| ID | Work item |
| --- | --- |
| P068 | Define districts, goals and playable regions |
| P069 | Draw the player route and progression graph |
| P070 | Design topology and critical pathways |
| P071 | Specify scale, coordinates and elevation |
| P072 | Plan stairs, crossings, gates and traversable routes |
| P073 | Test camera readability in the spatial design |
| P074 | Place landmarks and navigational cues |
| P075 | Build a technical blockout in the engine |
| P076 | Use reusable Modeling Mode geometry where appropriate |
| P077 | Validate collision, interaction and navigation geometry |
| P078 | Playtest the blockout |
| P079 | Correct actual scale, framing and traversal issues |
| P080 | Approve the layout for refinement |

**Exit evidence:** Human-reviewed playable blockout and softlock-safe route.

## 08 — SDD and core gameplay

| ID | Work item |
| --- | --- |
| P081 | Decompose into executable features |
| P082 | Define dependencies between features |
| P083 | Audit reusable classes, Blueprints and services |
| P084 | Specify observable requirements and edge cases |
| P085 | Assign C++, editor and content owners |
| P086 | Implement playable movement and camera |
| P087 | Implement contextual interaction |
| P088 | Implement real abilities and effects |
| P089 | Implement pickup, objectives and durable world changes |
| P090 | Integrate save/load and configuration |
| P091 | Write and run relevant automated tests |
| P092 | Integrate code into Blueprints and actual maps |
| P093 | Exercise the feature end to end in the game |
| P094 | Fix defects and regressions |
| P095 | Accept only with sufficient evidence |

**Exit evidence:** Integrated feature, reproducible tests, human playtest and explicit unresolved limitations.

## 09 — 3D production

| ID | Work item |
| --- | --- |
| P096 | Inventory needed assets |
| P097 | Classify structural, gameplay and decorative geometry |
| P098 | Prefer reuse over per-object generation |
| P099 | Prepare isolated model reference images |
| P100 | Specify scale, pivots, materials and technical requirements |
| P101 | Choose Blender, generator or engine modeling tools |
| P102 | Produce actual 3D models |
| P103 | Use an authorized Blender agent where useful |
| P104 | Inspect mesh scale, normals and topology |
| P105 | Correct topology, UVs and PBR materials |
| P106 | Build modular asset variants |
| P107 | Configure appropriate LOD/Nanite strategy |
| P108 | Export FBX/GLB with editable sources preserved |
| P109 | Reimport representative exports for integrity checks |
| P110 | Record tool, version, credit and rights provenance |

**Exit evidence:** Real exported meshes, provenance and representative import evidence.

## 10 — Character animation

| ID | Work item |
| --- | --- |
| P111 | Define rig and deformation requirements |
| P112 | Rig and skin character |
| P113 | Verify rest pose, bone axes, scale and hierarchy |
| P114 | Create locomotion animation |
| P115 | Create gestures, interactions and narrative reactions |
| P116 | Evaluate relevant animation-generation tools |
| P117 | Author IK rigs and retargeters when needed |
| P118 | Verify exported and imported clips |
| P119 | Set up real animation-state graphs and blending |
| P120 | Connect animation to authoritative gameplay events |
| P121 | Test transition, floor-contact and collision behavior |

**Exit evidence:** Reusable clips, skeleton compatibility and in-game animation tests.

## 11 — World integration

| ID | Work item |
| --- | --- |
| P122 | Import and inspect game assets |
| P123 | Replace blockout without breaking functional Actors |
| P124 | Build reusable material libraries and instances |
| P125 | Integrate vegetation, ruins, water and environmental detail |
| P126 | Evaluate PCG or other procedural placement |
| P127 | Author broken/restored visual states |
| P128 | Drive transformations from actual game state |
| P129 | Create district lighting and atmosphere |
| P130 | Validate World Partition, streaming and visual persistence |
| P131 | Fix collisions, materials, geometry and composition |

**Exit evidence:** Engine-authored level with tested state-driven visuals and safe traversal.

## 12 — VFX and cinematic design

| ID | Work item |
| --- | --- |
| P132 | List gameplay actions that need visible feedback |
| P133 | Build reusable Niagara effects |
| P134 | Build material and lighting reactions |
| P135 | Evaluate Chaos and Geometry Collections only where justified |
| P136 | Control authored sequences with Sequencer |
| P137 | Implement functional, deterministic reconstruction |
| P138 | Keep permanent gameplay state separate from temporary FX |
| P139 | Create camera shots and transitions |
| P140 | Handle interruptions, loading and skipped cinematics |
| P141 | Test effects in Editor, PIE and package |

**Exit evidence:** Functional transitions with reliable state and tested camera/input return.

## 13 — UI and accessibility

| ID | Work item |
| --- | --- |
| P142 | Design the end-to-end player flow |
| P143 | Create visual identity, logo and UI primitives |
| P144 | Build main menu, pause, new game and continue |
| P145 | Build HUD, objectives, notifications and context prompts |
| P146 | Integrate UMG/MVVM or suitable alternative |
| P147 | Represent actual loading and error states |
| P148 | Implement display, graphics, audio and game options |
| P149 | Implement remappable input and conflict handling |
| P150 | Create controls page and contextual onboarding |
| P151 | Check keyboard focus, scale and legibility |
| P152 | Verify settings persist and feedback is truthful |

**Exit evidence:** Menus, HUD and options integrated with real authorities and tested with input.

## 14 — Audio and story presentation

| ID | Work item |
| --- | --- |
| P153 | Define sonic identity |
| P154 | Plan music, ambience and sound effects |
| P155 | Generate or license original audio |
| P156 | Trim, normalize, loop and export usable formats |
| P157 | Configure spatial audio, buses and volume routing |
| P158 | Connect sound to committed gameplay state |
| P159 | Synchronize sound, Niagara and sequences |
| P160 | Create introduction, story beats and ending |
| P161 | Return player control after cinematics |
| P162 | Audit musical rights, mixing and packaged playback |

**Exit evidence:** Audible event-driven feedback, rights records and complete playable narrative.

## 15 — QA and stability

| ID | Work item |
| --- | --- |
| P163 | Build a feature-level test matrix |
| P164 | Run native compilation and automated tests |
| P165 | Validate assets in Editor |
| P166 | Run human manual PIE tests |
| P167 | Verify objectives, ability changes, interactions and collisions |
| P168 | Test save/load failure and separate-process reload |
| P169 | Test streaming restoration and replay safety |
| P170 | Test menus, controls, audio and accessibility |
| P171 | Create intermediate packaged builds |
| P172 | Run local cook and packaging |
| P173 | Launch standalone and Shipping executable |
| P174 | Measure CPU, GPU, memory, streaming and frametime |
| P175 | Fix crashes, regressions and presentation errors |
| P176 | Play the complete game from start to ending |

**Exit evidence:** Reproducible test evidence and human-accepted packaged gameplay.

## 16 — Release and publication

| ID | Work item |
| --- | --- |
| P177 | Review destination-specific submission requirements |
| P178 | Freeze scope and identify release blockers |
| P179 | Audit licenses, credits and external content |
| P180 | Record genuine screenshots, trailer and gameplay video |
| P181 | Write game description and installation instructions |
| P182 | Prepare final distribution and checksum |
| P183 | Test install and first launch |
| P184 | Publish accessible development evidence where needed |
| P185 | Complete store or competition listing |
| P186 | Submit through the authorized human owner |
| P187 | Verify downloadable release and links |

**Exit evidence:** Public release with verified download, rights and submission record.

## 17–18 — Retrospective and maintenance

| ID | Work item |
| --- | --- |
| P188 | Rebuild the actual historical timeline |
| P189 | Compare planned versus executed delivery |
| P190 | Record explicit scope cuts and cancellations |
| P191 | Document defects, limits and repaired causes |
| P192 | Review real cost, productivity and quality |
| P193 | Compare manual, assisted and AI-driven evidence when available |
| P194 | Analyze which tools actually added value |
| P195 | Publish lessons and reproducible examples |
| P196 | Plan future maintenance, upgrades and migrations |
| P197 | Maintain versioned playbook guidance |

**Exit evidence:** Evidence-based postmortem and a bounded maintenance plan.


## Scheduling, precedence, and reasonable cuts

Do not perform these items as an unprioritized 197-task waterfall. Focus first on a **real vertical slice** through movement, interaction, a meaningful world change, feedback, and human playtest. Package the executable early; continue to the remaining required route and finale. Art, animation, and audio can proceed in parallel only when their source references are approved and file ownership is non-conflicting. Mark skipped work explicitly as `Cut`, `Deferred`, or `Not Applicable` with the rationale, without portraying it as tested.

For an illustrated history of priorities and failures, read [The Unmade case study](../case-studies/the-unmade/README.md).