#include "rollforge/entity.h"
#include <algorithm>

namespace rollforge {

void Entity::TakeDamage(int amount) {
    if (amount <= 0) return;
    current_hp -= amount;
    if (current_hp < 0) {
        current_hp = 0;
    }
}

void Entity::Heal(int amount) {
    if (amount <= 0) return;
    current_hp += amount;
    if (current_hp > max_hp) {
        current_hp = max_hp;
    }
}

bool Entity::IsAlive() const {
    return current_hp > 0;
}

void PlayerCharacter::AddItem(const std::string& item) {
    inventory.push_back(item);
}

void PlayerCharacter::RemoveItem(const std::string& item) {
    auto it = std::find(inventory.begin(), inventory.end(), item);
    if (it != inventory.end()) {
        inventory.erase(it);
    }
}

int PlayerCharacter::PerformSkillCheck(StatType stat) const {
    int roll = Dice::Roll(20);
    int modifier = stats.getModifier(stat);
    return roll + modifier;
}

} // namespace rollforge
