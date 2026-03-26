#include "rollforge/action_engine.h"

namespace rollforge {

bool ActionResolver::ResolveAttack(const PlayerCharacter& attacker, const Entity& target, StatType stat) {
    int attack_roll = attacker.PerformSkillCheck(stat);
    return attack_roll >= target.armor_class;
}

void ActionResolver::ApplyDamage(Entity& target, int damage_amount) {
    target.TakeDamage(damage_amount);
}

bool ActionResolver::AttemptLockpick(SessionState& state, int difficulty_class) {
    int check_result = state.player.PerformSkillCheck(StatType::Dexterity);
    if (check_result >= difficulty_class) {
        state.world_flags["door_unlocked"] = true;
        return true;
    }
    return false;
}

} // namespace rollforge
