#define BOOST_TEST_MODULE ActionEngineTest
#include <boost/test/unit_test.hpp>

#include "rollforge/action_engine.h"

using namespace rollforge;

// ---------------------------------------------------------------------------
// ActionResolver
// ---------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(ActionResolution)

BOOST_AUTO_TEST_CASE(ResolveAttackExceedsOrMeetsACReturnsHit) {
    Dice::Seed(42); 
    // Roll(20) seed 42 results in some roll. Let's say we set the modifier very high
    // so it always hits.
    
    PlayerCharacter pc;
    pc.stats.strength = 30; // Mod +10
    
    Entity goblin;
    goblin.armor_class = 10;
    
    // As long as roll(1-20) + 10 >= 10, this hits (always hits).
    bool hit = ActionResolver::ResolveAttack(pc, goblin, StatType::Strength);
    BOOST_CHECK(hit);
}

BOOST_AUTO_TEST_CASE(ResolveAttackBelowACReturnsMiss) {
    // We set the target AC extremely high so it always misses
    PlayerCharacter pc;
    pc.stats.strength = 10; // Mod +0
    
    Entity dragon;
    dragon.armor_class = 30; // Roll max 20 + 0 = 20 < 30
    
    bool hit = ActionResolver::ResolveAttack(pc, dragon, StatType::Strength);
    BOOST_CHECK(!hit);
}

BOOST_AUTO_TEST_CASE(ApplyDamageReducesHealthAndSavesState) {
    PlayerCharacter pc;
    Entity goblin;
    goblin.current_hp = 15;
    
    // Suppose an attack hit, apply 5 damage
    ActionResolver::ApplyDamage(goblin, 5);
    
    BOOST_CHECK_EQUAL(goblin.current_hp, 10);
}

BOOST_AUTO_TEST_CASE(AttemptLockpickUsesDexterityAndModifiesFlags) {
    Dice::Seed(999);
    
    SessionState state;
    state.world_flags["door_unlocked"] = false;
    
    state.player.stats.dexterity = 20; // +5 Mod
    // If DC is 10, Roll(20) + 5 >= 10 is very likely
    // Let's test with DC 2 to guarantee success (minimum roll is 1 + 5 = 6 >= 2).
    
    bool success = ActionResolver::AttemptLockpick(state, 2);
    BOOST_CHECK(success);
    BOOST_CHECK_EQUAL(state.world_flags["door_unlocked"], true);
}

BOOST_AUTO_TEST_CASE(AttemptLockpickFailsAgainstHighDC) {
    SessionState state;
    state.world_flags["door_unlocked"] = false;
    
    state.player.stats.dexterity = 10; // +0 Mod
    
    bool success = ActionResolver::AttemptLockpick(state, 30); // Max possible is 20
    BOOST_CHECK(!success);
    BOOST_CHECK_EQUAL(state.world_flags["door_unlocked"], false); // Flag not changed
}

BOOST_AUTO_TEST_SUITE_END()
