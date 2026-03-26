# 🎲 RollForge

[![Python 3.10+](https://img.shields.io/badge/python-3.10+-blue.svg)](https://www.python.org/downloads/)
[![C++20](https://img.shields.io/badge/c++-20-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B20)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

A deterministic C++20 D&D mechanics engine with Python bindings.

RollForge is a lightweight, high-performance library specifically tailored to handle D&D style mechanics (stat blocks, dice rolls, health parsing, and combat checks). It is designed to act as a stateless engine for Python backends, ensuring the core rules of your game are processed securely and deterministically in C++ before being passed back to Python or submitted as prompts to AI APIs.

---

## 🎯 Features

- **Built with Modern C++20:** Ensures high performance and strict type safety.
- **Stateless Architecture:** Resolves state changes by taking inputs and emitting the resulting mutations, making it perfectly suited for web servers, LLM integration, and asynchronous game instances.
- **Robust Toolchain:** Built using CMake, dependency management via `vcpkg` (for `nlohmann-json` and Boost), and packaged for Python via `scikit-build-core` and `pybind11`.

---

## 🚀 Getting Started (Pre-compiled)

To make installation as easy as possible, RollForge is distributed as a pre-compiled Python Wheel (`.whl`). You do **not** need to install CMake, C++ compilers, or vcpkg to use this library in your Python projects.

1. Navigate to the [Releases](https://github.com/sedna08/rollforge/releases) page.
2. Download the `.whl` file that matches your operating system and Python version (e.g., `cp312` for Python 3.12).
3. Install it directly into your virtual environment:

**Using standard pip:**

```bash
python -m pip install rollforge-0.1.0-cp312-cp312-win_amd64.whl
```

**Using uv (Recommended):**

```bash
uv pip install rollforge-0.1.0-cp312-cp312-win_amd64.whl
```

### ⚠️ Windows Git Bash Troubleshooting

If you are using Git Bash/MSYS2 on Windows, your terminal might default to the MSYS2 Python environment instead of your project's virtual environment, causing a `ModuleNotFoundError`.
To ensure the correct environment executes your script, always run your code using `uv run` or explicitly point to the virtual environment:

```bash
uv run python your_script.py
# OR
.venv/Scripts/python.exe your_script.py
```

---

## 🛠️ Building from Source

If you want to contribute to the C++ core or compile the library for an unsupported architecture, you can build RollForge locally.

**Prerequisites:**

- C++20 compatible compiler (MSVC, GCC, or Clang)
- CMake 3.20+
- [vcpkg](https://vcpkg.io/) installed and bootstrapped.

**Build Steps:**

1.  Ensure your `VCPKG_ROOT` environment variable is set to your local vcpkg installation path.
2.  Clone the repository:
    ```bash
    git clone [https://github.com/sedna08/rollforge.git](https://github.com/sedna08/rollforge.git)
    cd rollforge
    ```
3.  Build and install the Python package:
    ```bash
    uv pip install .
    # OR
    python -m pip install .
    ```
    _Note: `scikit-build-core` will automatically invoke CMake, read your `VCPKG_ROOT`, and compile the extension._

---

## 💡 Usage

Once installed, RollForge behaves seamlessly like a native Python object, powered by the C++ engine:

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

For a comprehensive guide covering all Python classes (`SessionState`, `AbilityScores`, `PlayerCharacter`, `ActionResolver`, and `Dice`) and their attributes, please refer to the **[API Reference Documentation](docs/api_reference.md)**.
