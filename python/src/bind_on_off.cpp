#include <bind_on_off.hpp>

#include <SymGen/OnOff.hpp>

void bind_on_off(pybind11::module_& m)
{
	namespace py = pybind11;

	m.attr("ON")  = py::int_(static_cast<int>(SymGen::ON));
	m.attr("OFF") = py::int_(static_cast<int>(SymGen::OFF));
}
