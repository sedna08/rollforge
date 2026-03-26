#pragma once

#include <cmath>
#include <random>

namespace rollforge {

/// The six core D&D ability stats.
enum class StatType {
    Strength,
    Dexterity,
    Constitution,
    Intelligence,
    Wisdom,
    Charisma,
};

/// Holds the classic six ability scores and computes D&D modifiers.
struct AbilityScores {
    int strength = 10;
    int dexterity = 10;
    int constitution = 10;
    int intelligence = 10;
    int wisdom = 10;
    int charisma = 10;

    /// Returns the D&D modifier for the given stat: floor((score - 10) / 2).
    [[nodiscard]] int getModifier(StatType stat) const;
};

/// Handles all random number generation using a high-quality Mersenne Twister
/// engine. Thread-safe via thread_local storage.
class Dice {
public:
    /// Roll a single die with the given number of sides (1 to sides inclusive).
    static int Roll(int sides);

    /// Roll two dice and return the higher result (advantage).
    static int RollWithAdvantage(int sides);

    /// Roll two dice and return the lower result (disadvantage).
    static int RollWithDisadvantage(int sides);

    /// Seed the RNG for deterministic testing. Affects only the calling thread.
    static void Seed(unsigned int seed);

private:
    static std::mt19937& getEngine();
};

} // namespace rollforge
