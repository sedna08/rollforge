#include "rollforge/session_state.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace rollforge {

// Helper to serialize AbilityScores
void to_json(json& j, const AbilityScores& s) {
    j = json{
        {"strength", s.strength},
        {"dexterity", s.dexterity},
        {"constitution", s.constitution},
        {"intelligence", s.intelligence},
        {"wisdom", s.wisdom},
        {"charisma", s.charisma}
    };
}

// Helper to deserialize AbilityScores
void from_json(const json& j, AbilityScores& s) {
    j.at("strength").get_to(s.strength);
    j.at("dexterity").get_to(s.dexterity);
    j.at("constitution").get_to(s.constitution);
    j.at("intelligence").get_to(s.intelligence);
    j.at("wisdom").get_to(s.wisdom);
    j.at("charisma").get_to(s.charisma);
}

// Helper to serialize PlayerCharacter
void to_json(json& j, const PlayerCharacter& p) {
    j = json{
        {"id", p.id},
        {"name", p.name},
        {"max_hp", p.max_hp},
        {"current_hp", p.current_hp},
        {"armor_class", p.armor_class},
        {"level", p.level},
        {"experience", p.experience},
        // We ensure these serialize as empty arrays "[]" rather than null
        {"inventory", p.inventory.empty() ? json::array() : json(p.inventory)},
        {"conditions", p.conditions.empty() ? json::array() : json(p.conditions)},
        {"stats", p.stats}
    };
}

// Helper to deserialize PlayerCharacter
void from_json(const json& j, PlayerCharacter& p) {
    j.at("id").get_to(p.id);
    j.at("name").get_to(p.name);
    j.at("max_hp").get_to(p.max_hp);
    j.at("current_hp").get_to(p.current_hp);
    j.at("armor_class").get_to(p.armor_class);
    j.at("level").get_to(p.level);
    j.at("experience").get_to(p.experience);
    j.at("inventory").get_to(p.inventory);
    j.at("conditions").get_to(p.conditions);
    j.at("stats").get_to(p.stats);
}

// SessionState methods

std::string SessionState::SerializeToJSON() const {
    json j;
    j["current_location"] = current_location;
    
    if (world_flags.empty()) {
        j["world_flags"] = json::object();
    } else {
        j["world_flags"] = world_flags;
    }
    
    j["player"] = player;
    
    return j.dump(); // No indentation for compact string
}

SessionState SessionState::DeserializeFromJSON(const std::string& json_data) {
    json j = json::parse(json_data);
    
    SessionState state;
    j.at("current_location").get_to(state.current_location);
    j.at("world_flags").get_to(state.world_flags);
    j.at("player").get_to(state.player);
    
    return state;
}

} // namespace rollforge
