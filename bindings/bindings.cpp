#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "rollforge/mechanics.h"
#include "rollforge/entity.h"
#include "rollforge/session_state.h"
#include "rollforge/action_engine.h"

namespace py = pybind11;
using namespace rollforge;

PYBIND11_MODULE(rollforge, m) {
    m.doc() = "RollForge D&D Mechanics Library";

    // 1. Mechanics Module
    py::enum_<StatType>(m, "StatType")
        .value("Strength", StatType::Strength)
        .value("Dexterity", StatType::Dexterity)
        .value("Constitution", StatType::Constitution)
        .value("Intelligence", StatType::Intelligence)
        .value("Wisdom", StatType::Wisdom)
        .value("Charisma", StatType::Charisma)
        .export_values();

    py::class_<AbilityScores>(m, "AbilityScores")
        .def(py::init<>())
        .def_readwrite("strength", &AbilityScores::strength)
        .def_readwrite("dexterity", &AbilityScores::dexterity)
        .def_readwrite("constitution", &AbilityScores::constitution)
        .def_readwrite("intelligence", &AbilityScores::intelligence)
        .def_readwrite("wisdom", &AbilityScores::wisdom)
        .def_readwrite("charisma", &AbilityScores::charisma)
        .def("get_modifier", &AbilityScores::getModifier);

    py::class_<Dice>(m, "Dice")
        .def_static("roll", &Dice::Roll)
        .def_static("roll_with_advantage", &Dice::RollWithAdvantage)
        .def_static("roll_with_disadvantage", &Dice::RollWithDisadvantage)
        .def_static("seed", &Dice::Seed);

    // 2. Entity Module
    py::class_<Entity>(m, "Entity")
        .def(py::init<>())
        .def_readwrite("id", &Entity::id)
        .def_readwrite("name", &Entity::name)
        .def_readwrite("max_hp", &Entity::max_hp)
        .def_readwrite("current_hp", &Entity::current_hp)
        .def_readwrite("armor_class", &Entity::armor_class)
        .def_readwrite("stats", &Entity::stats)
        .def("take_damage", &Entity::TakeDamage)
        .def("heal", &Entity::Heal)
        .def("is_alive", &Entity::IsAlive);

    py::class_<PlayerCharacter, Entity>(m, "PlayerCharacter")
        .def(py::init<>())
        .def_readwrite("level", &PlayerCharacter::level)
        .def_readwrite("experience", &PlayerCharacter::experience)
        .def_readwrite("inventory", &PlayerCharacter::inventory)
        .def_readwrite("conditions", &PlayerCharacter::conditions)
        .def("add_item", &PlayerCharacter::AddItem)
        .def("remove_item", &PlayerCharacter::RemoveItem)
        .def("perform_skill_check", &PlayerCharacter::PerformSkillCheck);

    // 3. Session State Module
    py::class_<SessionState>(m, "SessionState")
        .def(py::init<>())
        .def_readwrite("player", &SessionState::player)
        .def_readwrite("current_location", &SessionState::current_location)
        .def_readwrite("world_flags", &SessionState::world_flags)
        .def("serialize_to_json", &SessionState::SerializeToJSON)
        .def_static("deserialize_from_json", &SessionState::DeserializeFromJSON);

    // 4. Action Engine Module
    py::class_<ActionResolver>(m, "ActionResolver")
        .def_static("resolve_attack", &ActionResolver::ResolveAttack)
        .def_static("apply_damage", &ActionResolver::ApplyDamage)
        .def_static("attempt_lockpick", &ActionResolver::AttemptLockpick);
}