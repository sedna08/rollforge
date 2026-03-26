#pragma once

#include <string>
#include <map>
#include "rollforge/entity.h"

namespace rollforge {

/// Represents the entire mechanics state of a single player's game at a given moment.
class SessionState {
public:
    PlayerCharacter player;
    std::string current_location;
    std::map<std::string, bool> world_flags;

    /// Serializes the entire session state to a JSON string.
    [[nodiscard]] std::string SerializeToJSON() const;

    /// Deserializes a JSON string into a new SessionState object.
    /// Throws nlohmann::json exceptions (parse_error, out_of_range, type_error) on malformed input.
    [[nodiscard]] static SessionState DeserializeFromJSON(const std::string& json_data);
};

} // namespace rollforge
