#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>
#include <iostream>

void process_array(const std::vector<float>& array) {
    for (size_t i = 0; i < array.size(); i++) {
        std::cout << "Element " << i << ": " << array[i] << std::endl;
    }
}

PYBIND11_MODULE(example, m) {
    m.def("process_array", &process_array, "Process a list of floats");
}