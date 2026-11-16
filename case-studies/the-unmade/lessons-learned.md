# The Unmade — Lessons to Reuse

## Keep the game executable before maximizing visual density

The playable slice forced early inspection of camera, movement, interaction, collision and the actual cost of a voluntary sacrifice. A concept image or correctly compiled Blueprint could not substitute for traversing the path and observing the state change.

## Distinguish authoritative systems from presentation

Keep a single durable owner for gameplay progress. Meshes, water, NPC visibility, Niagara and Sequencer should **respond to** a committed event. This makes interrupted animation, streaming and Save/Load easier to reason about.

## Use specialized tools only at their appropriate boundary

Blender and generator workflows produce editable geometry and animation; Unreal Editor produces and integrates native engine assets; code agents implement actual systems; documentation collects confirmed decisions and factual reports. Each handoff needs a usable, tested artifact and a named receiver.

## Context and reuse matter more than model quantity

Before writing a new manager or trying a new plugin, inspect the current source and editor state. One task owner and one Editor operator prevent expensive overwrites. A narrower prompt with exact acceptance and file boundaries is more useful than an unbounded “finish the whole game” assignment.

## Test success is not interchangeable

The F006 case demonstrates why an overall process success cannot override an individual failed asset integration test. The character case shows why an imported mesh does not prove correctly scaled animation. The visual case shows why shader compilation does not prove final in-game appearance.

## Preserve failures, limits and scope cuts

Incomplete AnimGraphs, unsaved packages, lack of full runtime tests and missing release evidence were documented rather than rewritten as success. Those are useful teaching examples, because real teams must often decide whether to fix, cut or defer.

## Preserve an honest case-study boundary

The documented pipeline is reusable with no AI or with any future model. The Unmade reports provide observations under particular tools and versions; they are not a controlled benchmark proving AI is faster than manual production. A public educational guide must never expose unrelated private work or publish source/assets without rights.