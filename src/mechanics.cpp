#include "rollforge/mechanics.h"

#include <algorithm>

namespace rollforge {

int AbilityScores::getModifier(StatType stat) const {
    int score = 0;
    switch (stat) {
    case StatType::Strength:
        score = strength;
        break;
    case StatType::Dexterity:
        score = dexterity;
        break;
    case StatType::Constitution:
        score = constitution;
        break;
    case StatType::Intelligence:
        score = intelligence;
        break;
    case StatType::Wisdom:
        score = wisdom;
        break;
    case StatType::Charisma:
        score = charisma;
        break;
    }
    return static_cast<int>(std::floor(static_cast<double>(score - 10) / 2.0));
}

std::mt19937& Dice::getEngine() {
    thread_local std::mt19937 engine{std::random_device{}()};
    return engine;
}

void Dice::Seed(unsigned int seed) {
    getEngine().seed(seed);
}

int Dice::Roll(int sides) {
    std::uniform_int_distribution<int> dist(1, sides);
    return dist(getEngine());
}

int Dice::RollWithAdvantage(int sides) {
    int roll1 = Roll(sides);
    int roll2 = Roll(sides);
    return std::max(roll1, roll2);
}

int Dice::RollWithDisadvantage(int sides) {
    int roll1 = Roll(sides);
    int roll2 = Roll(sides);
    return std::min(roll1, roll2);
}

} // namespace rollforge
