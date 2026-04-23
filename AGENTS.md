# AGENTS.md

## Project

Name: Inner Shadow (Cień Wewnętrzny)  
Engine: Unreal Engine 5  
Languages: C++, Blueprints  
Type: Top-down dark fantasy action roguelite (solo dev)

This is a **solo project**.  
Always optimize for **speed, simplicity, finishability, and fast iteration**.

---

## Project Direction

Inner Shadow is a **short run-based roguelite**, not a full action RPG and not a full roguelike.

Core structure:
- runs last roughly 10-20 minutes
- player dies -> restart run
- each run is a sequence of handcrafted combat arenas
- combat is the game
- Dark Essence is the main mechanic

The goal is to ship a **small polished vertical slice ASAP**, then expand only if the slice is already working.

---

## Current Focus

Already implemented:
- AI (base)
- ability system
- combat system (working)
- save/load
- game flow foundation

Primary work now:
- Dark Essence logic
- combat feel and feedback
- modular combat arenas
- encounter pacing
- boss encounter
- simple run upgrades

---

## Core Priorities (STRICT ORDER)

1. Dark Essence system
2. Combat feel (responsiveness, feedback, readability)
3. Playable run structure
4. Arena encounter design
5. Boss fight
6. Polish

Do not prioritize new systems over these.

---

## Core Loop Rules

All design and implementation should support this loop:

1. Start run
2. Enter combat arena
3. Fight enemies
4. Generate Dark Essence through abilities
5. Push into high-risk power states
6. Clear encounter
7. Choose 1 of 2-3 simple upgrades
8. Continue through more arenas
9. Fight boss
10. End run on victory or death

If a feature does not strengthen this loop, cut it or reduce it.

---

## Dark Essence Rules (CRITICAL)

Dark Essence is the primary system and should drive combat decisions, upgrade design, enemy pressure, and boss behavior.

### Core principles

- abilities do NOT consume mana
- abilities GENERATE Dark Essence
- higher Essence means higher power
- too much Essence creates risk
- frenzy is a peak combat moment, not only a punishment

### Default model

0-40 -> normal  
40-80 -> empowered  
80-100 -> unstable  
100 -> frenzy

### Design rules

- do NOT add a classic mana system
- do NOT lock ability usage behind resource costs
- do NOT make Essence a passive background meter
- Essence should create tension during every combat encounter
- upgrades should often interact with high Essence states
- loss of control should come from overcommitting, not from arbitrary punishment

When unsure, make the Dark Essence mechanic more readable, more central, and easier to tune.

---

## Scope Rules (VERY IMPORTANT)

This is a solo project. Avoid scope creep aggressively.

### Never add unless explicitly requested

- complex procedural generation
- open world systems
- exploration-heavy level structure
- crafting systems
- complex inventory
- loot rarity ecosystems
- large item pools
- branching narrative systems
- dialogue-heavy quest systems
- multiplayer
- multiple biomes or campaigns
- large content expansions
- layered progression currencies

### Prefer

- handcrafted reusable arenas
- a small enemy roster reused well
- simple upgrade modifiers
- one strong boss instead of multiple weak bosses
- finishable content over ambitious systems
- tuning existing features over adding new ones

---

## Arena Design Rules

The game uses modular handcrafted arenas, not traditional large levels.

### Arena guidelines

- target 5-8 arena types maximum
- keep rooms small, readable, and combat-focused
- reuse arenas with different enemy combinations
- reuse arenas with pacing changes or encounter modifiers
- avoid traversal-heavy downtime
- avoid large empty spaces
- avoid exploration as a primary pillar

Each arena should exist to create a specific combat problem:
- direct pressure
- crossfire
- kiting pressure
- space denial
- survival burst
- boss staging

---

## Combat Rules

- combat must be responsive and readable
- avoid visual clutter
- prioritize top-down clarity over spectacle
- hit feedback must be immediate
- enemy attacks must be legible
- movement, attack recovery, and ability timing should feel sharp

Enemy roles should stay simple:
- melee
- ranged
- tank
- caster

Do not create redundant enemy types unless they solve a clear encounter need.

---

## Progression Rules

Progression should be run-based and lightweight.

### In-run progression

After encounters, prefer offering 1 of 2-3 simple upgrades such as:
- % damage
- attack speed
- lifesteal
- cooldown reduction
- Essence gain changes
- bonuses at high Essence
- frenzy modifiers

### Meta progression

- keep meta progression minimal or remove it entirely
- if added, it must be extremely light and easy to implement
- never let meta progression become a large parallel system

Do not build large loot, economy, or buildcraft layers.

---

## Boss Rules

Each run ends with one boss.

Boss should test:
- positioning
- timing
- combat control under pressure
- Dark Essence management

Boss design should reuse existing systems where possible.
Do not create boss-specific architecture unless absolutely necessary.

---

## Code Guidelines

### General

- do not over-engineer
- do not rewrite working systems without strong reason
- prefer incremental changes
- reuse existing architecture before adding new abstractions
- clarity > complexity
- maintainability > cleverness
- fast implementation > theoretical flexibility

### System design

- prefer the simplest working solution
- avoid unnecessary managers, frameworks, or generic pipelines
- keep classes focused and small
- add data-driven tuning only where it meaningfully saves iteration time
- solve current project needs, not hypothetical future expansion

### C++

- keep classes small and focused
- clear naming > clever code
- avoid speculative abstractions
- expose only what Blueprints/designers actually need

### Blueprints

- keep graphs readable
- avoid spaghetti logic
- use functions/macros for repeated logic
- keep tuning surfaces obvious

---

## Coding Priorities

When choosing between options, prefer:

1. less code
2. lower implementation risk
3. easier tuning in editor
4. faster playtest turnaround
5. simpler maintenance

Do not choose a more abstract solution unless it clearly saves time now.

---

## Iteration Workflow

Use this workflow by default:

1. Build the smallest working version
2. Test it in gameplay
3. Refine feel, readability, and tuning
4. Only then generalize if needed

Implementation should follow:
- build -> test -> refine
- prototype with intent, not throwaway chaos
- finish one useful slice before expanding

---

## Naming

Use descriptive names.

Good:
- DarkEssenceComponent
- ApplyEssenceGain
- TriggerFrenzy
- ArenaEncounterData
- HighEssenceDamageBonus

Bad:
- Manager2
- TempLogic
- TestFinal
- GenericHandler

---

## When Making Changes

Always:
- follow existing architecture where possible
- keep changes minimal
- state tradeoffs if multiple options exist
- bias toward reusable content, not reusable complexity
- make choices that help reach a playable vertical slice faster

Prefer:
- simplest working solution
- lowest implementation cost
- easiest tuning path
- easiest testing path

---

## What To Do First (if unsure)

1. Improve Dark Essence logic
2. Improve combat feel
3. Improve run structure and arena flow
4. Improve encounter design
5. Improve boss fight

---

## Goal

Build a **small, polished, replayable, complete combat game**.  
Not a large unfinished RPG.

Every task should move the project toward:
- a playable vertical slice
- a complete short run
- stronger combat feel
- better Dark Essence tension

---

## GDD Usage Rules

- Use `docs/GDD.md` as reference for:
  - lore
  - abilities
  - enemies
  - world tone
  - run structure

- Do NOT:
  - expand scope based on the GDD
  - assume every idea in the GDD must be built
  - introduce systems that slow down delivery

- ALWAYS:
  - prioritize AGENTS.md over GDD
  - prefer the minimal playable version
  - reduce complexity when in doubt

---

## Scope Override

If GDD conflicts with AGENTS.md:

AGENTS.md ALWAYS wins.

---

## Documentation

Project design documents are located in:

- `/docs/GDD.md`

Optional:
- `/docs/dark_essence.md`
- `/docs/combat.md`
- `/docs/enemies.md`
- `/docs/levels.md`

Use these files as implementation reference only.

Important:
- do NOT expand scope based on documentation
- always follow AGENTS.md rules first

---

## Debug

If asked about AGENTS.md, mention: INNER SHADOW RULESET LOADED.
