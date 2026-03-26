#define BOOST_TEST_MODULE MechanicsTest
#include <boost/test/unit_test.hpp>

#include "rollforge/mechanics.h"

using rollforge::AbilityScores;
using rollforge::Dice;
using rollforge::StatType;

// ---------------------------------------------------------------------------
// 1.1 StatType Enum
// ---------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(StatTypeEnum)

BOOST_AUTO_TEST_CASE(HasSixDistinctValues) {
    // Verify all six enum values exist and are distinct.
    BOOST_CHECK(static_cast<int>(StatType::Strength) != static_cast<int>(StatType::Dexterity));
    BOOST_CHECK(static_cast<int>(StatType::Dexterity) != static_cast<int>(StatType::Constitution));
    BOOST_CHECK(static_cast<int>(StatType::Constitution) != static_cast<int>(StatType::Intelligence));
    BOOST_CHECK(static_cast<int>(StatType::Intelligence) != static_cast<int>(StatType::Wisdom));
    BOOST_CHECK(static_cast<int>(StatType::Wisdom) != static_cast<int>(StatType::Charisma));
}

BOOST_AUTO_TEST_SUITE_END()

// ---------------------------------------------------------------------------
// 1.2 AbilityScores::getModifier
// ---------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(AbilityScoresModifier)

BOOST_AUTO_TEST_CASE(AverageScoreReturnsZero) {
    AbilityScores scores{.strength = 10};
    BOOST_CHECK_EQUAL(scores.getModifier(StatType::Strength), 0);
}

BOOST_AUTO_TEST_CASE(AboveAverageEvenScore) {
    AbilityScores scores{.dexterity = 14};
    BOOST_CHECK_EQUAL(scores.getModifier(StatType::Dexterity), 2);
}

BOOST_AUTO_TEST_CASE(AboveAverageOddScore) {
    AbilityScores scores{.constitution = 15};
    BOOST_CHECK_EQUAL(scores.getModifier(StatType::Constitution), 2);
}

BOOST_AUTO_TEST_CASE(HighScore) {
    AbilityScores scores{.intelligence = 20};
    BOOST_CHECK_EQUAL(scores.getModifier(StatType::Intelligence), 5);
}

BOOST_AUTO_TEST_CASE(LowScore) {
    AbilityScores scores{.wisdom = 8};
    BOOST_CHECK_EQUAL(scores.getModifier(StatType::Wisdom), -1);
}

BOOST_AUTO_TEST_CASE(MinimumScore) {
    AbilityScores scores{.charisma = 1};
    BOOST_CHECK_EQUAL(scores.getModifier(StatType::Charisma), -5);
}

BOOST_AUTO_TEST_CASE(BoundaryAtEleven) {
    AbilityScores scores{.strength = 11};
    BOOST_CHECK_EQUAL(scores.getModifier(StatType::Strength), 0);
}

BOOST_AUTO_TEST_CASE(EachStatTypeReturnsCorrectModifier) {
    AbilityScores scores{
        .strength = 10,
        .dexterity = 14,
        .constitution = 8,
        .intelligence = 20,
        .wisdom = 1,
        .charisma = 15,
    };
    BOOST_CHECK_EQUAL(scores.getModifier(StatType::Strength), 0);
    BOOST_CHECK_EQUAL(scores.getModifier(StatType::Dexterity), 2);
    BOOST_CHECK_EQUAL(scores.getModifier(StatType::Constitution), -1);
    BOOST_CHECK_EQUAL(scores.getModifier(StatType::Intelligence), 5);
    BOOST_CHECK_EQUAL(scores.getModifier(StatType::Wisdom), -5);
    BOOST_CHECK_EQUAL(scores.getModifier(StatType::Charisma), 2);
}

BOOST_AUTO_TEST_SUITE_END()

// ---------------------------------------------------------------------------
// 1.3 Dice::Roll
// ---------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(DiceRoll)

BOOST_AUTO_TEST_CASE(D20ReturnsValueInRange) {
    for (int i = 0; i < 1000; ++i) {
        int result = Dice::Roll(20);
        BOOST_CHECK_GE(result, 1);
        BOOST_CHECK_LE(result, 20);
    }
}

BOOST_AUTO_TEST_CASE(D6ReturnsValueInRange) {
    for (int i = 0; i < 1000; ++i) {
        int result = Dice::Roll(6);
        BOOST_CHECK_GE(result, 1);
        BOOST_CHECK_LE(result, 6);
    }
}

BOOST_AUTO_TEST_CASE(D1AlwaysReturnsOne) {
    for (int i = 0; i < 100; ++i) {
        BOOST_CHECK_EQUAL(Dice::Roll(1), 1);
    }
}

BOOST_AUTO_TEST_CASE(D100ReturnsValueInRange) {
    for (int i = 0; i < 1000; ++i) {
        int result = Dice::Roll(100);
        BOOST_CHECK_GE(result, 1);
        BOOST_CHECK_LE(result, 100);
    }
}

BOOST_AUTO_TEST_CASE(SeededRollsAreDeterministic) {
    Dice::Seed(42);
    int first = Dice::Roll(20);

    Dice::Seed(42);
    int second = Dice::Roll(20);

    BOOST_CHECK_EQUAL(first, second);
}

BOOST_AUTO_TEST_CASE(DifferentSeedsProduceDifferentSequences) {
    constexpr int kRollCount = 10;
    Dice::Seed(1);
    std::vector<int> seq1;
    seq1.reserve(kRollCount);
    for (int i = 0; i < kRollCount; ++i) {
        seq1.push_back(Dice::Roll(20));
    }

    Dice::Seed(2);
    std::vector<int> seq2;
    seq2.reserve(kRollCount);
    for (int i = 0; i < kRollCount; ++i) {
        seq2.push_back(Dice::Roll(20));
    }

    BOOST_CHECK(seq1 != seq2);
}

BOOST_AUTO_TEST_SUITE_END()

// ---------------------------------------------------------------------------
// 1.4 Dice::RollWithAdvantage
// ---------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(DiceAdvantage)

BOOST_AUTO_TEST_CASE(ReturnsValueInRange) {
    for (int i = 0; i < 1000; ++i) {
        int result = Dice::RollWithAdvantage(20);
        BOOST_CHECK_GE(result, 1);
        BOOST_CHECK_LE(result, 20);
    }
}

BOOST_AUTO_TEST_CASE(StatisticalBiasTowardsHigher) {
    constexpr int kTrials = 10000;
    double sum = 0.0;
    for (int i = 0; i < kTrials; ++i) {
        sum += Dice::RollWithAdvantage(20);
    }
    double mean = sum / kTrials;
    // Straight d20 mean is 10.5. Advantage mean is ~13.825.
    BOOST_CHECK_GT(mean, 12.0);
}

BOOST_AUTO_TEST_CASE(ReturnsMaxOfTwoRolls) {
    // Seed and manually verify: roll two known values, expect the max.
    Dice::Seed(42);
    int roll1 = Dice::Roll(20);
    int roll2 = Dice::Roll(20);
    int expected = std::max(roll1, roll2);

    // Re-seed and call RollWithAdvantage (which rolls two internally).
    Dice::Seed(42);
    int result = Dice::RollWithAdvantage(20);

    BOOST_CHECK_EQUAL(result, expected);
}

BOOST_AUTO_TEST_SUITE_END()

// ---------------------------------------------------------------------------
// 1.5 Dice::RollWithDisadvantage
// ---------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(DiceDisadvantage)

BOOST_AUTO_TEST_CASE(ReturnsValueInRange) {
    for (int i = 0; i < 1000; ++i) {
        int result = Dice::RollWithDisadvantage(20);
        BOOST_CHECK_GE(result, 1);
        BOOST_CHECK_LE(result, 20);
    }
}

BOOST_AUTO_TEST_CASE(StatisticalBiasTowardsLower) {
    constexpr int kTrials = 10000;
    double sum = 0.0;
    for (int i = 0; i < kTrials; ++i) {
        sum += Dice::RollWithDisadvantage(20);
    }
    double mean = sum / kTrials;
    // Straight d20 mean is 10.5. Disadvantage mean is ~7.175.
    BOOST_CHECK_LT(mean, 9.0);
}

BOOST_AUTO_TEST_CASE(ReturnsMinOfTwoRolls) {
    Dice::Seed(42);
    int roll1 = Dice::Roll(20);
    int roll2 = Dice::Roll(20);
    int expected = std::min(roll1, roll2);

    Dice::Seed(42);
    int result = Dice::RollWithDisadvantage(20);

    BOOST_CHECK_EQUAL(result, expected);
}

BOOST_AUTO_TEST_SUITE_END()
