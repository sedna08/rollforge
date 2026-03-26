# 🎲 RollForge

[![Python 3.10+](https://img.shields.io/badge/python-3.10+-blue.svg)](https://www.python.org/downloads/)
[![C++20](https://img.shields.io/badge/c++-20-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B20)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

A deterministic C++20 D&D mechanics engine with Python bindings.

RollForge is a lightweight, high-performance library specifically tailored to handle D&D style mechanics (stat blocks, dice rolls, health parsing, and combat checks). It's designed to act as a stateless engine for Python backends, ensuring the core rules of your game are processed securely and deterministically in C++ before being passed back to Python or submitted as prompts to AI APIs.

---

## 🎯 Features

- **Built with Modern C++20:** Ensures high performance and strict type safety.
- **Stateless Architecture:** Resolves state changes by taking inputs and emitting the resulting mutations, making it perfectly suited for web servers, LLM integration, and asynchronous game instances.
- **Robust Toolchain:** Built using CMake, dependency management via `vcpkg` (like json and Boost), and packaged for pip via `scikit-build-core` and `pybind11`.

---

## 🚀 Getting Started

RollForge uses a modern CMake and vcpkg toolchain wrapped in Python's build system. To install the library locally for development into your current Python environment, clone the source and run `pip install`.

**Prerequisites:**

- Python 3.10+
- CMake 3.20+
- A C++20 compatible compiler (e.g., MinGW GCC)
- [vcpkg](https://vcpkg.io/en/index.html) installed locally

```bash
# Clone the repository
git clone https://github.com/sedna08/rollforge.git
cd rollforge

# Install via pip
# (Note: Passing the Toolchain and Triplet explicitly ensures vcpkg installs dependencies correctly for your compiler)
pip install . --config-settings="cmake.args=-DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg.cmake;-DVCPKG_TARGET_TRIPLET=x64-mingw-dynamic;-DCMAKE_C_COMPILER=gcc;-DCMAKE_CXX_COMPILER=g++"
```

_**NOTE**: Replace the `CMAKE_TOOLCHAIN_FILE` with the absolute path to your local `vcpkg.cmake` installation._

---

## 💡 Usage

Once compiled/installed, RollForge behaves seamlessly like a native Python object, powered by the C++ engine:

```python
import rollforge

# 1. Initialize State
state = rollforge.SessionState()
state.current_location = "Goblin Camp"

# 2. Setup PC Data
pc = state.player
pc.name = "Eldrin"
pc.stats.dexterity = 16  # Automatically translates to a +3 modifier in C++
pc.armor_class = 15

# 3. Resolve an Attack Check against a target
goblin = rollforge.Entity()
goblin.armor_class = 12
goblin.current_hp = 10

# The ActionResolver rolls a D20, adds Eldrin's Dexterity modifier, and checks vs AC.
hit = rollforge.ActionResolver.resolve_attack(pc, goblin, rollforge.StatType.Dexterity)

# 4. Serialize to JSON for storage/transmission
print(state.serialize_to_json())
```

---

## 📚 Documentation

For a comprehensive guide covering all Python classes (`SessionState`, `AbilityScores`, `PlayerCharacter`, `ActionResolver`, and `Dice`) and their attributes, please refer to the **[API Reference Documentation](docs/_internal/api_reference.md)**.

_(Note: In the raw repo, navigate to `docs/api_reference.md` instead of `docs/_internal/api_reference.md` outside of development.)_

---
