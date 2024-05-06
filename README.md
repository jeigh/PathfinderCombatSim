I created this with the ambitous goal of creating a combat simulator for Pathfinder 2e, mainly to be used as a DM to simulate combat between multiple teams of NPCs to gauge the relative strengths of each other.

As of yet, there are MANY unimplemented features; the most obvious ones being 

Future Feature List

* Multithreading support.  This app currently underutilizes the CPU, especially on ones with multiple cores.   Eventually I would like it to max out all cores by default.
* User Input.   Right now, if you want to simulate combat, you must hard-code the combat teams in code (either program.cs or main.css) and hit the F5 button.   Ideally, these sequences should be managable from within some form of UI instead of being hard-coded.
* Persistence.  At some point in the near future, I want the NPC information to be stored in a database instead of just loading the characters from the Static Definitions library.
* Movement (And any form of a battlefield)
* Attacks of opportunity
* Special abilities and attacks like trip, disarm, etc...
* Simple reports showing the outcome of these simulations in aggregate.
* Combat visualizations, Replays, etc, allowing for someone to step through the sequence of an individual replay
* Simple AI for more intelligent NPCs to use those special abilities, avoid AoOs, and position themselves to avoid sneak attacks.
* Any magic spells or similar effects.
* Magic Weapons (above and beyond what can be simulated through just increasing the attack modifiers of the mob's primary weapon)
* Any form of character or NPC progression.   At the moment, if you want your characters to advance, you simply make a new static definition with updated stats.
* A portal for players and DMs to upload their characters.
* The concept of a session, where DMs can create a session, players can connect to it, and whatever live visualizations I can think of.
* DM and player authentication via social network identies.
* Advanced AI 

Here is a list of the current Feature Matrix...

|Feature|C++ Version|C# Version|
|-------------------------------------------------|-|-|
|Simple Turn-Based Combat, Death, Unconciousness  |✅|✅|
|Simple Console-based UI                          |✅|✅|
|Multi-Team support                               | |✅|
|Multi-Threading support                          |✅||
|Properly Functioning Initiative                  |✅|(unconfirmed)|
|User Input  |||
|Database Persistence |||
|Simple 2D Battlefield Mechanics (Movement) |||
|Simple 3D Battlefield Mechanics (Flying) |||
|Obstacles & Pathing |||
|Special Abilities (trim, disarm, etc...) |||
|Simple Reports|||
|Combat Visualizations, Replays, etc...|||
|Simple AI|||
|Magic Weapons|||
|Magic Spells|||
|Advanced AI|||



