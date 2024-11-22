// python_wrapper.cpp
#include "python_wrapper.hpp"
#include <stdexcept>

extern "C" {

// Constructor
static PyObject* PyMatrixOps_new(PyTypeObject* type, PyObject* args, PyObject* kwargs) {
    PyMatrixOps* self = (PyMatrixOps*)type->tp_alloc(type, 0);
    if (self != NULL) {
        double initial_value = 0.0;
        static char* kwlist[] = {"initial_value", NULL};
        
        // Parse optional initial value
        if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|d", kwlist, &initial_value)) {
            Py_DECREF(self);
            return NULL;
        }
        
        try {
            self->cpp_obj = new MatrixOperations(initial_value);
        } catch (const std::exception& e) {
            Py_DECREF(self);
            PyErr_SetString(PyExc_RuntimeError, e.what());
            return NULL;
        }
    }
    return (PyObject*)self;
}

// Destructor
static void PyMatrixOps_dealloc(PyMatrixOps* self) {
    delete self->cpp_obj;
    Py_TYPE(self)->tp_free((PyObject*)self);
}



// Wrapper for power method (with keyword arguments)
static PyObject* PyMathOps_power(PyObject* self, PyObject* args, PyObject* kwargs) {
    double exponent;
    int absolute = 0;  // Default to false
    static char* kwlist[] = {"exponent", "absolute", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "d|p", kwlist, 
                                    &exponent, &absolute)) {
        return NULL;
    }
    
    try {
        double result = ((PyMathOps*)self)->cpp_obj->power(exponent, absolute != 0);
        return PyFloat_FromDouble(result);
    } catch (const std::exception& e) {
        PyErr_SetString(PyExc_RuntimeError, e.what());
        return NULL;
    }
}


// Method definitions
static PyMethodDef PyMathOps_methods[] = {
    {"add", PyMathOps_add, METH_VARARGS,
     "Add a number to current value"},
    {"power", (PyCFunction)PyMathOps_power, METH_VARARGS | METH_KEYWORDS,
     "Raise current value to a power, optionally take absolute value first"},
    // Add other methods...
    {NULL, NULL, 0, NULL}
};

// Property definitions
static PyGetSetDef PyMathOps_getset[] = {
    {"value", PyMatrixOps_get_value, NULL, "Current value", NULL},
    {NULL, NULL, NULL, NULL, NULL}
};

// Type definition
PyTypeObject PyMathOpsType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "math_module.MathOperations",
    .tp_basicsize = sizeof(PyMathOps),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor)PyMathOps_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "MathOperations object",
    .tp_methods = PyMathOps_methods,
    .tp_getset = PyMathOps_getset,
    .tp_init = 0,  // Will use default init
    .tp_new = PyMathOps_new,
};

// Module definition
static PyModuleDef matrixmodule = {
    PyModuleDef_HEAD_INIT,
    "matrixmodule",
    "Example module that wraps Matrix Operations class",
    -1,
    NULL
};

// Module initialization
PyMODINIT_FUNC PyInit_matrixmodule(void) {
    PyObject* m;

    if (PyType_Ready(&PyMathOpsType) < 0)
        return NULL;

    m = PyModule_Create(&matrixmodule);
    if (m == NULL)
        return NULL;

    Py_INCREF(&PyMathOpsType);
    if (PyModule_AddObject(m, "MathOperations", (PyObject*)&PyMatrixType) < 0) {
        Py_DECREF(&PyMathOpsType);
        Py_DECREF(m);
        return NULL;
    }

    return m;
}

} // extern "C"