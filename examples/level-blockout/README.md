# T14 Example — Real, Camera-Testable Unreal Blockout

The [Editor Python script](scripts/author_walkable_blockout.py) creates **seven genuine StaticMeshActors** in an **already existing authorized map** using the installed Unreal Engine Cube Static Mesh. These are actual collision-bearing structural actors, not render-only concept images or empty mesh classes.

**Status:** Source is published but **has not been run in a UE 5.9 Editor**. No level asset or native Actor was created by this repository. Executing it **does modify the currently loaded map**, so only the sole authorized Editor operator may run it after inspecting dirty packages.

## Execution contract

1. Confirm a valid **existing** current level and **exclusive Editor ownership**. Duplicate/copy only **your own approved** level if experimenting; do not reset or delete others' work.
2. Set process environment variables **before launching the Editor**:

   - `GPP_BLOCKOUT_MAP`: real loaded level's asset name (e.g. `L_TrainingBlockout`).
   - `GPP_BLOCKOUT_PREFIX`: a unique, approved actor prefix ending in underscore, e.g. `GPP_Training_`.

3. Verify the installed Python Editor Script Plugin, Editor Actor Subsystem and built-in Engine Cube are available.
4. Run the Python script through the **Unreal Editor** supported Python execution route. It refuses a mismatched loaded level or any existing Actor whose label starts with that prefix.
5. Check the **seven created actor labels**, mesh assignments, dimensions, collision and positions. The layout has a plaza, a 220 cm wide connecting walkway, real structural side walls and a recovery platform.
6. Save the **authorized map** using Unreal Editor after inspecting any dirty packages. The script itself does not Save All and does not remove any Actor.
7. Place/integrate the game's **actual** approved PlayerStart, Character/Controller, interaction targets, durable progress Gate Actor and recovery pickup using their real existing systems. This script deliberately does **not** invent them.
8. Run PIE with the actual capsule and T08 camera; measure walkability, camera occlusion, gate collision and safe recovery path. Repeat in a new packaged process when the feature requires it.

**Important:** The walkway remains open until the game's own real gate is integrated. This example creates a physically walkable architectural route, **not a complete collectible/sacrifice feature**. If the route's dimensions conflict with the actual game's capsule or art grid, adapt approved source dimensions before execution.

On a partial failure, report which Actors were created and preserve the map; **do not run an automatic delete/cleanup**.

See [T14 — Level Design, Modeling Mode and Geometry Script](../../tutorials/level-design-modeling-mode-geometry-script.md). Script is original educational code under [MIT](../../LICENSE-CODE.md).
