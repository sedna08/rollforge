#pragma once

#include <string>
#include <vector>
#include "rollforge/mechanics.h"

namespace rollforge {

/// Base interface for any object that can take part in mechanical interactions (combat, checks).
class Entity {
public:
    std::string id;
    std::string name;
    int max_hp = 0;
    int current_hp = 0;
    int armor_class = 10;
    AbilityScores stats;

    virtual ~Entity() = default;

    /// Reduces current_hp by the given amount. Cannot drop below 0.
    virtual void TakeDamage(int amount);

    /// Increases current_hp by the given amount. Cannot exceed max_hp.
    virtual void Heal(int amount);

    /// Returns true if current_hp > 0.
    [[nodiscard]] virtual bool IsAlive() const;
};

/// A fully fleshed-out character controlled by a player.
class PlayerCharacter : public Entity {
public:
    int level = 1;
    int experience = 0;
    std::vector<std::string> inventory;
    std::vector<std::string> conditions;

    /// Adds an item to the character's inventory.
    void AddItem(const std::string& item);

    /// Removes an item from the character's inventory if it exists.
    void RemoveItem(const std::string& item);

    /// Performs a standard d20 skill check using the specified stat.
    [[nodiscard]] int PerformSkillCheck(StatType stat) const;
};

} // namespace rollforge
