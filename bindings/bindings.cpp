#include <pybind11/pybind11.h>
// #include <rollforge/mechanics.h> // You will include your headers here

namespace py = pybind11;

PYBIND11_MODULE(rollforge, m) {
    m.doc() = "RollForge D&D Mechanics C++ Backend";
}