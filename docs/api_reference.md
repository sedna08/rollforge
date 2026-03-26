# RollForge API Reference

RollForge is a C++20 D&D mechanics library with Python bindings via `pybind11`. This document describes the Python API exposed by the `rollforge` module.

## Importing the Library

```python
import rollforge
```

---

## 1. Mechanics Module

### `StatType` (Enum)
Represents the six core D&D ability scores.
- `rollforge.StatType.Strength`
- `rollforge.StatType.Dexterity`
- `rollforge.StatType.Constitution`
- `rollforge.StatType.Intelligence`
- `rollforge.StatType.Wisdom`
- `rollforge.StatType.Charisma`

### `AbilityScores`
A structure holding a creature's or character's core stats.
- **Attributes:**
  - `strength` (int): default 10
  - `dexterity` (int): default 10
  - `constitution` (int): default 10
  - `intelligence` (int): default 10
  - `wisdom` (int): default 10
  - `charisma` (int): default 10
- **Methods:**
  - `get_modifier(stat: StatType) -> int`: Returns the D&D 5e modifier for the given stat (e.g., 16 -> +3).

### `Dice`
A static utility class for rolling dice. Randomness is seeded per-thread.
- **Methods:**
  - `Dice.roll(sides: int) -> int`: Rolls a single die with `sides` faces (e.g., `Dice.roll(20)`).
  - `Dice.roll_with_advantage(sides: int) -> int`: Rolls twice and returns the highest value.
  - `Dice.roll_with_disadvantage(sides: int) -> int`: Rolls twice and returns the lowest value.
  - `Dice.seed(seed: int) -> None`: Seeds the random number generator (useful for testing/determinism).

---

## 2. Entity Module

### `Entity`
The base class for anything in the game that interacts mechanically (has HP, AC, etc.).
- **Attributes:**
  - `id` (str): Unique identifier.
  - `name` (str): Display name.
  - `max_hp` (int): Maximum hit points.
  - `current_hp` (int): Current hit points.
  - `armor_class` (int): Target number to hit this entity.
  - `stats` (AbilityScores): The entity's core attributes.
- **Methods:**
  - `take_damage(amount: int) -> None`: Reduces `current_hp` (clamped to 0).
  - `heal(amount: int) -> None`: Increases `current_hp` (clamped to `max_hp`).
  - `is_alive() -> bool`: Returns `True` if `current_hp > 0`.

### `PlayerCharacter` (Inherits from `Entity`)
A fully fleshed-out character controlled by a player.
- **Additional Attributes:**
  - `level` (int): Character level.
  - `experience` (int): Current XP.
  - `inventory` (List[str]): List of items currently carried.
  - `conditions` (List[str]): List of active status conditions (e.g., "Poisoned").
- **Methods:**
  - `add_item(item: str) -> None`: Adds an item to the inventory.
  - `remove_item(item: str) -> None`: Removes the first instance of an item from the inventory.
  - `perform_skill_check(stat: StatType) -> int`: Rolls a d20 and adds the modifier for the specified stat.

---

## 3. Session State Module

### `SessionState`
Represents the entire mechanics state of a single player's game at a given moment. Used for serialization.
- **Attributes:**
  - `player` (PlayerCharacter): The main character's state.
  - `current_location` (str): The ID or name of the current room/node.
  - `world_flags` (Dict[str, bool]): Global boolean flags (e.g., `{"door_unlocked": True}`).
- **Methods:**
  - `serialize_to_json() -> str`: Returns a compact JSON string representing the entire state.
  - `SessionState.deserialize_from_json(json_data: str) -> SessionState`: Static method that reconstructs the state from a JSON string. Throws `RuntimeError` (mapping from C++ `json::exception`) if the JSON is malformed or missing keys.

---

## 4. Action Engine

### `ActionResolver`
A static utility class that resolves interactions between entities and the game world.
- **Methods:**
  - `ActionResolver.resolve_attack(attacker: PlayerCharacter, target: Entity, stat: StatType) -> bool`: Performs a skill check for the attacker using the given stat. Returns `True` if the result equals or exceeds the target's `armor_class`.
  - `ActionResolver.apply_damage(target: Entity, amount: int) -> None`: Wrapper around `target.take_damage(amount)`.
  - `ActionResolver.attempt_lockpick(state: SessionState, difficulty_class: int) -> bool`: Performs a Dexterity check for the player. If it meets or beats the `difficulty_class`, it sets `state.world_flags["door_unlocked"] = True` and returns `True`. Otherwise returns `False`.

---

## 5. Example Usage (Python)

```python
import rollforge

# 1. Initialize State
state = rollforge.SessionState()
state.current_location = "Dungeon Entrance"

# 2. Setup Character
player = state.player
player.name = "Eldrin"
player.max_hp = 30
player.current_hp = 30
player.armor_class = 15
player.stats.strength = 16    # +3 Mod
player.stats.dexterity = 14   # +2 Mod

player.add_item("Longsword")
player.add_item("Health Potion")

# 3. Setup Enemy
goblin = rollforge.Entity()
goblin.name = "Sneaky Goblin"
goblin.armor_class = 12
goblin.max_hp = 10
goblin.current_hp = 10

# 4. Resolve an Attack
hit = rollforge.ActionResolver.resolve_attack(player, goblin, rollforge.StatType.Strength)
if hit:
    print(f"{player.name} hit the {goblin.name}!")
    # Apply damage
    damage = rollforge.Dice.roll(8) + player.stats.get_modifier(rollforge.StatType.Strength)
    rollforge.ActionResolver.apply_damage(goblin, damage)
else:
    print(f"{player.name} missed.")

# 5. Save Game State
save_data = state.serialize_to_json()
print("Saved Game Data:", save_data)

# 6. Load Game State
loaded_state = rollforge.SessionState.deserialize_from_json(save_data)
print("Loaded Location:", loaded_state.current_location)
```
