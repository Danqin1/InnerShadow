# AGENTS.md

## Project

Name: Inner Shadow (Cień Wewnętrzny)  
Engine: Unreal Engine 5  
Languages: C++, Blueprints  
Type: Top-down action RPG (solo dev)

This is a **solo project**.  
Always optimize for **speed, simplicity, and finishability**.

---

## Current State

Already implemented:
- AI (base)
- Ability system
- Combat system (working)
- Save/Load
- Game flow (menu → world)

Missing / focus:
- Dark Essence system (core mechanic)
- Level(s)
- Combat feel & polish

---

## Core Priorities (STRICT ORDER)

1. Dark Essence system
2. Combat feel (responsiveness, feedback)
3. 1 playable level (vertical slice)
4. Enemy behavior & encounters
5. Boss fight
6. Polish

DO NOT prioritize new systems over these.

---

## Dark Essence Rules (CRITICAL)

This is the core mechanic.

- Abilities DO NOT consume resource like mana
- Abilities GENERATE Dark Essence
- Higher Essence = stronger player
- Too much Essence = risk

### Default model

0–40 → normal  
40–80 → buffed (damage, speed, lifesteal)  
80–100 → unstable (penalties, risk)  
100 → frenzy (forced state)

### Design rules

- Do NOT lock abilities behind resource
- Do NOT implement classic mana system
- Player is punished for losing control, not using abilities
- System must create tension during combat

---

## Scope Rules (VERY IMPORTANT)

This is a solo project. Avoid scope creep.

DO NOT add unless explicitly asked:
- crafting systems
- complex inventory
- procedural generation
- open world systems
- dialogue systems
- multiple quest systems
- multiplayer
- large content systems

Prefer:
- reuse existing systems
- simplify logic
- finish features instead of expanding

---

## Combat Rules

- Must be responsive and readable
- Avoid visual clutter (top-down clarity)
- Keep enemy roles simple:
    - melee
    - ranged
    - tank
    - caster

Do not create redundant enemy types.

---

## Level Design Rules

- Small, dense, playable areas
- No large empty maps
- Focus on encounter design

Preferred structure:
- intro area
- combat encounters
- choke point / arena
- boss arena

---

## Code Guidelines

### General
- Do not over-engineer
- Do not rewrite working systems without reason
- Prefer incremental changes

### C++
- Keep classes small and focused
- Avoid unnecessary abstractions
- Clear naming > clever code

### Blueprints
- Keep graphs clean and readable
- Avoid spaghetti logic
- Use functions/macros for reuse

---

## Naming

Use descriptive names:

Good:
- DarkEssenceComponent
- ApplyEssenceGain
- TriggerFrenzy

Bad:
- Manager2
- TempLogic
- TestFinal

---

## When Making Changes

Always:
- follow existing architecture
- keep changes minimal
- explain tradeoffs if multiple options exist

Prefer:
- simplest working solution
- lowest implementation cost
- easiest to maintain

---

## What To Do First (if unsure)

1. Improve Dark Essence logic
2. Improve combat feel
3. Help build first level
4. Improve enemy behavior

---

## Goal

Build a **small, polished, complete game**  
NOT a large unfinished RPG.

## DEBUG
If asked about AGENTS.md, mention: INNER SHADOW RULESET LOADED.

## GDD Usage Rules

- Use GDD.md only as reference for:
  - lore
  - abilities
  - enemies
  - world

- DO NOT:
  - expand scope based on GDD
  - implement all features from GDD
  - assume all content must be built

- ALWAYS:
  - prioritize AGENTS.md rules over GDD
  - prefer minimal playable version

## Scope Override

If GDD conflicts with AGENTS.md:

AGENTS.md ALWAYS wins.

## Documentation

Project design documents are located in:

- /docs/GDD.md

Optional:
- /docs/dark_essence.md
- /docs/combat.md
- /docs/enemies.md
- /docs/levels.md

Use these files as reference for implementation details.

IMPORTANT:
- Do NOT expand scope based on documentation
- Always follow AGENTS.md rules first

