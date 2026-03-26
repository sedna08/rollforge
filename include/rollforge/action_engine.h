#pragma once

#include "rollforge/entity.h"
#include "rollforge/session_state.h"

namespace rollforge {

/// Core engine for resolving mechanical interactions between entities and the world state.
class ActionResolver {
public:
    /// Resolves an attack roll against a target's Armor Class using the provided stat.
    /// Returns true if the attack hits (roll + modifier >= target.armor_class).
    [[nodiscard]] static bool ResolveAttack(const PlayerCharacter& attacker, const Entity& target, StatType stat);

    /// Utility wrapper to apply damage to an entity.
    static void ApplyDamage(Entity& target, int damage_amount);

    /// Attempts to pick a lock by making a dexterity check against the given difficulty class (DC).
    /// Updates the session state's "door_unlocked" world flag on success.
    /// Returns true on success, false on failure.
    static bool AttemptLockpick(SessionState& state, int difficulty_class);
};

} // namespace rollforge
