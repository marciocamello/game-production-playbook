# The Unmade — Selected Creative and Technical Decisions

## Creative design

**DIRECTOR'S CUT** was interpreted as the deliberate choice of what is removed, preserved, sacrificed and restored—not a mandatory filmmaking interface. The protagonist's incomplete body and the city's missing architectural elements share one art language. The selected tone was Gothic stylized 3D with a stable elevated view, a restrained cold-to-warm palette, environmental storytelling and no essential combat.

The approved progression focused on **Light donation at the fountain**, **Impulse donation at a mechanical bridge**, then a **voluntary final heart gift**. Light returns later from a distinct fragment, without undoing the fountain; Impulse does not simply return immediately, because the new permanent bridge substitutes for it. Fundamental walking and interaction must remain available to avoid a softlock.

## Architecture

A C++ + Blueprints Unreal Engine 5.9 project was selected. Gameplay Ability System/Gameplay Tags handle real capacities; an in-memory progression subsystem owns logical state; World Actors apply that state to collisions and visuals. An approved SaveGame design uses versioned snapshots and two alternating slots; actual crash-durability claims require tests. UMG/MVVM projects state into HUD, menus and settings; ViewModels do not replace authoritative game services.

**Technical invariants:** one coherent world/World Partition map; stable IDs for progression-critical Actors; idempotent state reapplication after streaming and Load; a new action triggers one transition effect; Niagara/Chaos/Sequencer presentation must not mutate authoritative progress.

## Collaboration

GPT owned canonical Story/GDD/TDD/SDD, logs and audit consolidation. Codex handled primary native C++ and automation tests. Claude handled sole Unreal Editor/MCP integration where assigned. Antigravity/Blender and external tools handled approved 3D production. The human Game Director approved decisions and exclusively performed manual PIE according to the project's policy.

**Scope control:** a feature was only complete after actual integration and observed acceptance—not because a spec, C++ compile, generated asset, or editor screenshot existed.

## Environment and visual gate

The V1 Blueprint inheritance work preceded whole-world visual approval. V2 defined the overall city and before/after landmark concepts. V3 converted those into actual routes and a spatial blockout with elevated plazas, stairs, a bridge gap and an approach to the heart. Geometry created with native Modeling tools could remain final rather than always being replaced by externally generated 3D.

## Research-only and optional technologies

Unreal Canvas, Engine source API reference, PCG, Mesh Terrain, Chaos Cache, CommonUI frameworks and additional AI tools were investigated or considered **only where a real production advantage justified the integration cost**. Presence in a document or plugin list is not evidence of a tested, shipping feature.