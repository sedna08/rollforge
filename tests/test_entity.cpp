#define BOOST_TEST_MODULE EntityTest
#include <boost/test/unit_test.hpp>

#include "rollforge/entity.h"
#include "rollforge/mechanics.h"

using namespace rollforge;

// ---------------------------------------------------------------------------
// 2.1 Entity Base Class
// ---------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(EntityBaseClass)

BOOST_AUTO_TEST_CASE(InitWithValidAttributes) {
    PlayerCharacter pc;
    pc.id = "char_001";
    pc.name = "Eldrin";
    pc.max_hp = 25;
    pc.current_hp = 25;
    pc.armor_class = 14;
    
    BOOST_CHECK_EQUAL(pc.id, "char_001");
    BOOST_CHECK_EQUAL(pc.name, "Eldrin");
    BOOST_CHECK_EQUAL(pc.max_hp, 25);
    BOOST_CHECK_EQUAL(pc.current_hp, 25);
    BOOST_CHECK_EQUAL(pc.armor_class, 14);
}

BOOST_AUTO_TEST_CASE(IsAliveReturnsTrueWhenHPIsGreaterThanZero) {
    PlayerCharacter pc;
    pc.current_hp = 1;
    BOOST_CHECK(pc.IsAlive());
}

BOOST_AUTO_TEST_CASE(IsAliveReturnsFalseWhenHPIsZero) {
    PlayerCharacter pc;
    pc.current_hp = 0;
    BOOST_CHECK(!pc.IsAlive());
}

BOOST_AUTO_TEST_SUITE_END()

// ---------------------------------------------------------------------------
// TakeDamage
// ---------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(EntityTakeDamage)

BOOST_AUTO_TEST_CASE(NormalDamageReducesHP) {
    PlayerCharacter pc;
    pc.current_hp = 20;
    pc.TakeDamage(5);
    BOOST_CHECK_EQUAL(pc.current_hp, 15);
}

BOOST_AUTO_TEST_CASE(DamageDoesNotDropHPBelowZero) {
    PlayerCharacter pc;
    pc.current_hp = 3;
    pc.TakeDamage(10);
    BOOST_CHECK_EQUAL(pc.current_hp, 0);
    BOOST_CHECK(!pc.IsAlive());
}

BOOST_AUTO_TEST_CASE(ZeroDamageHasNoEffect) {
    PlayerCharacter pc;
    pc.current_hp = 20;
    pc.TakeDamage(0);
    BOOST_CHECK_EQUAL(pc.current_hp, 20);
}

BOOST_AUTO_TEST_CASE(ExactLethalDamage) {
    PlayerCharacter pc;
    pc.current_hp = 5;
    pc.TakeDamage(5);
    BOOST_CHECK_EQUAL(pc.current_hp, 0);
    BOOST_CHECK(!pc.IsAlive());
}

BOOST_AUTO_TEST_SUITE_END()

// ---------------------------------------------------------------------------
// Heal
// ---------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(EntityHeal)

BOOST_AUTO_TEST_CASE(NormalHealIncreasesHP) {
    PlayerCharacter pc;
    pc.max_hp = 25;
    pc.current_hp = 10;
    pc.Heal(5);
    BOOST_CHECK_EQUAL(pc.current_hp, 15);
}

BOOST_AUTO_TEST_CASE(HealCannotExceedMaxHP) {
    PlayerCharacter pc;
    pc.max_hp = 25;
    pc.current_hp = 23;
    pc.Heal(10);
    BOOST_CHECK_EQUAL(pc.current_hp, 25);
}

BOOST_AUTO_TEST_CASE(HealFromZero) {
    PlayerCharacter pc;
    pc.max_hp = 25;
    pc.current_hp = 0;
    pc.Heal(5);
    BOOST_CHECK_EQUAL(pc.current_hp, 5);
    BOOST_CHECK(pc.IsAlive());
}

BOOST_AUTO_TEST_CASE(ZeroHealHasNoEffect) {
    PlayerCharacter pc;
    pc.max_hp = 25;
    pc.current_hp = 10;
    pc.Heal(0);
    BOOST_CHECK_EQUAL(pc.current_hp, 10);
}

BOOST_AUTO_TEST_SUITE_END()

// ---------------------------------------------------------------------------
// 2.2 PlayerCharacter
// ---------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(PlayerCharacterInventory)

BOOST_AUTO_TEST_CASE(AddItemToEmptyInventory) {
    PlayerCharacter pc;
    pc.AddItem("Dagger");
    BOOST_REQUIRE_EQUAL(pc.inventory.size(), 1);
    BOOST_CHECK_EQUAL(pc.inventory[0], "Dagger");
}

BOOST_AUTO_TEST_CASE(AddMultipleItems) {
    PlayerCharacter pc;
    pc.AddItem("Rope");
    pc.AddItem("Torch");
    BOOST_REQUIRE_EQUAL(pc.inventory.size(), 2);
    BOOST_CHECK_EQUAL(pc.inventory[0], "Rope");
    BOOST_CHECK_EQUAL(pc.inventory[1], "Torch");
}

BOOST_AUTO_TEST_CASE(RemoveExistingItem) {
    PlayerCharacter pc;
    pc.AddItem("Dagger");
    pc.RemoveItem("Dagger");
    BOOST_CHECK(pc.inventory.empty());
}

BOOST_AUTO_TEST_CASE(RemoveNonExistentItem) {
    PlayerCharacter pc;
    pc.AddItem("Rope");
    pc.RemoveItem("Sword");
    BOOST_REQUIRE_EQUAL(pc.inventory.size(), 1);
    BOOST_CHECK_EQUAL(pc.inventory[0], "Rope");
}

BOOST_AUTO_TEST_CASE(AddDuplicateItems) {
    PlayerCharacter pc;
    pc.AddItem("Potion");
    pc.AddItem("Potion");
    BOOST_REQUIRE_EQUAL(pc.inventory.size(), 2);
    BOOST_CHECK_EQUAL(pc.inventory[0], "Potion");
    BOOST_CHECK_EQUAL(pc.inventory[1], "Potion");
}

BOOST_AUTO_TEST_SUITE_END()

// ---------------------------------------------------------------------------
// PerformSkillCheck
// ---------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(PlayerCharacterSkillCheck)

BOOST_AUTO_TEST_CASE(SkillCheckReturnsRollPlusModifier) {
    Dice::Seed(42);
    // Let's figure out what the seeded roll would be.
    int expected_roll = Dice::Roll(20);
    
    // Reset seed for the actual check
    Dice::Seed(42);
    
    PlayerCharacter pc;
    pc.stats.dexterity = 16; // mod +3
    
    int result = pc.PerformSkillCheck(StatType::Dexterity);
    BOOST_CHECK_EQUAL(result, expected_roll + 3);
}

BOOST_AUTO_TEST_CASE(SkillCheckWithNegativeModifier) {
    Dice::Seed(12345);
    int expected_roll = Dice::Roll(20);
    Dice::Seed(12345);
    
    PlayerCharacter pc;
    pc.stats.strength = 8; // mod -1
    
    int result = pc.PerformSkillCheck(StatType::Strength);
    BOOST_CHECK_EQUAL(result, expected_roll - 1);
}

BOOST_AUTO_TEST_CASE(SkillCheckWithZeroModifier) {
    Dice::Seed(999);
    int expected_roll = Dice::Roll(20);
    Dice::Seed(999);
    
    PlayerCharacter pc;
    pc.stats.wisdom = 10; // mod 0
    
    int result = pc.PerformSkillCheck(StatType::Wisdom);
    BOOST_CHECK_EQUAL(result, expected_roll);
}

BOOST_AUTO_TEST_SUITE_END()
