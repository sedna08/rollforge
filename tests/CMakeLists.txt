# tests/CMakeLists.txt

# Find the Boost test framework
find_package(Boost REQUIRED COMPONENTS unit_test_framework)

# Create the test executable for mechanics
add_executable(test_mechanics test_mechanics.cpp)

# Link the core library and Boost
target_link_libraries(test_mechanics PRIVATE 
    rollforge_core 
    Boost::unit_test_framework
)

# Register the test with CTest
add_test(NAME MechanicsTest COMMAND test_mechanics)

# Create the test executable for entity
add_executable(test_entity test_entity.cpp)
target_link_libraries(test_entity PRIVATE rollforge_core Boost::unit_test_framework)
add_test(NAME EntityTest COMMAND test_entity)

# (Optional) Add more test executables here as the project grows
# add_executable(test_session_state test_session_state.cpp)