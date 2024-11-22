
// -----------------------------------------------------------------------------------
// python_wrapper.hpp
#include <Python.h>
#include "include/matrix.hpp"

// Python object structure
typedef struct {
    PyObject_HEAD
    Matrix* cpp_obj;  // Pointer to our C++ instance
} PyMatrixOps;

// Declare extern "C" interface
#ifdef __cplusplus
extern "C" {
#endif

extern PyTypeObject PyMatrixType;

#ifdef __cplusplus
}
#endif