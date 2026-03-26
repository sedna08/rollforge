#define BOOST_TEST_MODULE SessionStateTest
#include <boost/test/unit_test.hpp>
#include <nlohmann/json.hpp>

#include "rollforge/session_state.h"

using namespace rollforge;
using json = nlohmann::json;

// Helper function to create a fully populated session state for testing
SessionState CreateTestState() {
    SessionState state;
    state.current_location = "Goblin Cave";
    state.world_flags["boss_defeated"] = false;
    state.world_flags["has_key"] = true;

    state.player.id = "char_001";
    state.player.name = "Eldrin";
    state.player.max_hp = 25;
    state.player.current_hp = 18;
    state.player.armor_class = 14;
    state.player.level = 3;
    state.player.experience = 950;
    
    state.player.inventory.push_back("Rope");
    state.player.inventory.push_back("Dagger");
    state.player.inventory.push_back("Thieves' Tools");
    
    // Explicitly leaving conditions empty to test empty array serialization
    
    state.player.stats.strength = 10;
    state.player.stats.dexterity = 16;
    state.player.stats.constitution = 12;
    state.player.stats.intelligence = 14;
    state.player.stats.wisdom = 10;
    state.player.stats.charisma = 8;
    
    return state;
}

// ---------------------------------------------------------------------------
// 3.1 SerializeToJSON
// ---------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(Serialization)

BOOST_AUTO_TEST_CASE(FullStateSerializesCorrectly) {
    SessionState state = CreateTestState();
    std::string json_str = state.SerializeToJSON();
    
    // Parse it back to verify structure without dealing with string exact matching
    json parsed = json::parse(json_str);
    
    BOOST_CHECK_EQUAL(parsed["current_location"], "Goblin Cave");
    BOOST_CHECK_EQUAL(parsed["world_flags"]["has_key"], true);
    BOOST_CHECK_EQUAL(parsed["world_flags"]["boss_defeated"], false);
    
    BOOST_CHECK_EQUAL(parsed["player"]["id"], "char_001");
    BOOST_CHECK_EQUAL(parsed["player"]["name"], "Eldrin");
    BOOST_CHECK_EQUAL(parsed["player"]["max_hp"], 25);
    BOOST_CHECK_EQUAL(parsed["player"]["current_hp"], 18);
    BOOST_CHECK_EQUAL(parsed["player"]["armor_class"], 14);
    BOOST_CHECK_EQUAL(parsed["player"]["level"], 3);
    BOOST_CHECK_EQUAL(parsed["player"]["experience"], 950);
    
    BOOST_CHECK_EQUAL(parsed["player"]["inventory"].size(), 3);
    BOOST_CHECK_EQUAL(parsed["player"]["inventory"][0], "Rope");
    
    BOOST_CHECK_EQUAL(parsed["player"]["stats"]["dexterity"], 16);
    BOOST_CHECK_EQUAL(parsed["player"]["stats"]["charisma"], 8);
}

BOOST_AUTO_TEST_CASE(EmptyCollectionsSerializeEmptyObjectAndArray) {
    SessionState state; // Default initialized
    std::string json_str = state.SerializeToJSON();
    json parsed = json::parse(json_str);
    
    BOOST_CHECK(parsed["player"]["inventory"].is_array());
    BOOST_CHECK(parsed["player"]["inventory"].empty());
    
    BOOST_CHECK(parsed["player"]["conditions"].is_array());
    BOOST_CHECK(parsed["player"]["conditions"].empty());
    
    BOOST_CHECK(parsed["world_flags"].is_object());
    BOOST_CHECK(parsed["world_flags"].empty());
}

BOOST_AUTO_TEST_SUITE_END()

// ---------------------------------------------------------------------------
// 3.2 DeserializeFromJSON
// ---------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(Deserialization)

BOOST_AUTO_TEST_CASE(ValidCompleteJSONReconstructsState) {
    // We already trust our serializer from the previous test suite, 
    // so we can use it to generate our test input.
    SessionState original = CreateTestState();
    std::string json_str = original.SerializeToJSON();
    
    SessionState reconstructed = SessionState::DeserializeFromJSON(json_str);
    
    BOOST_CHECK_EQUAL(reconstructed.current_location, "Goblin Cave");
    BOOST_CHECK_EQUAL(reconstructed.world_flags.at("has_key"), true);
    
    BOOST_CHECK_EQUAL(reconstructed.player.id, "char_001");
    BOOST_CHECK_EQUAL(reconstructed.player.name, "Eldrin");
    BOOST_CHECK_EQUAL(reconstructed.player.max_hp, 25);
    BOOST_CHECK_EQUAL(reconstructed.player.inventory.size(), 3);
    BOOST_CHECK_EQUAL(reconstructed.player.conditions.empty(), true);
    
    BOOST_CHECK_EQUAL(reconstructed.player.stats.dexterity, 16);
}

BOOST_AUTO_TEST_CASE(RoundTripConsistency) {
    SessionState original = CreateTestState();
    std::string first_json = original.SerializeToJSON();
    
    SessionState reconstructed = SessionState::DeserializeFromJSON(first_json);
    std::string second_json = reconstructed.SerializeToJSON();
    
    // The JSON output should be canonically identical
    BOOST_CHECK_EQUAL(first_json, second_json);
}

BOOST_AUTO_TEST_SUITE_END()

// ---------------------------------------------------------------------------
// 3.3 Error Handling (Malformed Input)
// ---------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(DeserializationErrors)

BOOST_AUTO_TEST_CASE(EmptyStringThrowsParseError) {
    BOOST_CHECK_THROW(SessionState::DeserializeFromJSON(""), json::parse_error);
}

BOOST_AUTO_TEST_CASE(InvalidJSONSyntaxThrowsParseError) {
    BOOST_CHECK_THROW(SessionState::DeserializeFromJSON("{bad json"), json::parse_error);
}

BOOST_AUTO_TEST_CASE(MissingPlayerKeyThrowsException) {
    std::string invalid_json = R"({"current_location": "Town", "world_flags": {}})";
    BOOST_CHECK_THROW(SessionState::DeserializeFromJSON(invalid_json), json::out_of_range);
}

BOOST_AUTO_TEST_CASE(MissingStatsKeyInPlayerThrowsException) {
    std::string invalid_json = R"(
    {
      "current_location": "Town",
      "world_flags": {},
      "player": {
        "id": "1", "name": "Bob", "max_hp": 10, "current_hp": 10, 
        "armor_class": 10, "level": 1, "experience": 0, 
        "inventory": [], "conditions": []
      }
    })";
    BOOST_CHECK_THROW(SessionState::DeserializeFromJSON(invalid_json), json::out_of_range);
}

BOOST_AUTO_TEST_CASE(WrongTypeThrowsTypeException) {
    std::string invalid_json = R"(
    {
      "current_location": "Town",
      "world_flags": {},
      "player": {
        "id": "1", "name": "Bob", "max_hp": "not_a_number", "current_hp": 10, 
        "armor_class": 10, "level": 1, "experience": 0, 
        "inventory": [], "conditions": [],
        "stats": {
           "strength": 10, "dexterity": 10, "constitution": 10,
           "intelligence": 10, "wisdom": 10, "charisma": 10
        }
      }
    })";
    BOOST_CHECK_THROW(SessionState::DeserializeFromJSON(invalid_json), json::type_error);
}

BOOST_AUTO_TEST_CASE(MissingLocationThrowsException) {
    std::string invalid_json = R"(
    {
      "world_flags": {},
      "player": {
        "id": "1", "name": "Bob", "max_hp": 10, "current_hp": 10, 
        "armor_class": 10, "level": 1, "experience": 0, 
        "inventory": [], "conditions": [],
        "stats": {
           "strength": 10, "dexterity": 10, "constitution": 10,
           "intelligence": 10, "wisdom": 10, "charisma": 10
        }
      }
    })";
    BOOST_CHECK_THROW(SessionState::DeserializeFromJSON(invalid_json), json::out_of_range);
}

BOOST_AUTO_TEST_CASE(ExtraUnknownKeysAreTolerated) {
    SessionState original = CreateTestState();
    json j = json::parse(original.SerializeToJSON());
    j["extra_random_key"] = "this should be ignored";
    j["player"]["extra_player_key"] = 42;
    
    SessionState reconstructed = SessionState::DeserializeFromJSON(j.dump());
    BOOST_CHECK_EQUAL(reconstructed.current_location, "Goblin Cave");
    BOOST_CHECK_EQUAL(reconstructed.player.id, "char_001");
}

BOOST_AUTO_TEST_SUITE_END()
