# RollForge

## Highlights

Here are the main takeaways of this library:

- Deterministic D&D mechanics engine designed for Python API backends.
- Built with modern C++20 for high performance and strict type safety.
- Seamless Python integration using pybind11 and scikit-build-core.
- Robust dependency management via vcpkg.
- Fully stateless architecture, perfect for web servers and LLM integration.

## Overview

RollForge is a lightweight, high-performance C++ library specifically tailored to handle D&D style mechanics. It is designed to bridge the gap between deterministic game logic and non-deterministic Large Language Models. By handling dice rolls, stat calculations, and entity management in a strict C++ environment, RollForge ensures that the core rules of the game are always respected before passing narrative control over to an AI.

### The Problem It Solves

LLMs struggle with true randomness and strict mathematical state tracking. RollForge solves this by providing a reliable backend calculator. Your Python application passes the current game state to RollForge, it resolves the mechanical actions (like skill checks or combat), and returns both the numerical results and a pre-formatted system prompt ready to be sent to your LLM of choice.

## Usage instructions

RollForge acts as a stateless engine. You instantiate the state from your database, let the library resolve the mechanics, and use the result to prompt your AI.

```python
import rollforge

# Load your raw state (e.g., from a database)
raw_json_state = '{"player": {"stats": {"dexterity": 16}}}'

# Deserialize the state into the C++ engine
session = rollforge.SessionState.deserialize(raw_json_state)

# Resolve a mechanical action
result = rollforge.ActionResolver.resolve_check(session, rollforge.StatType.DEXTERITY)

print(f"Roll Total: {result.total}")
print(f"Prompt for LLM: {result.dm_prompt}")
```

## Installation instructions

RollForge uses a modern CMake and vcpkg toolchain wrapped in Python's build system.

To install the library into your Python environment locally, clone the repository and use pip. The build process will automatically invoke CMake and fetch necessary C++ dependencies (like nlohmann/json and Boost for testing).

```bash
git clone <your-repository-url>/rollforge.git
cd rollforge
pip install .
```

**Requirements:**

- C++20 compatible compiler (GCC, Clang, or MSVC)
- CMake 3.20 or higher
- Python 3.10 or higher
