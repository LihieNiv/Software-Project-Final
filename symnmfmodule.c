#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "symnmf.h"

// Helper function to convert a Python list of lists to a vector linked list
struct vector* convert_pylist_to_vector(PyObject *pylist) {
    struct vector *head = NULL;
    struct vector *v_prev = NULL;

    Py_ssize_t num_vectors = PyList_Size(pylist);
    for (Py_ssize_t i = 0; i < num_vectors; i++) {
        PyObject *pycords = PyList_GetItem(pylist, i);
        struct vector *v = (struct vector *)PyMem_Malloc(sizeof(struct vector));
        v->next = NULL;
        v->cords = NULL;

        struct cord *c_prev = NULL;
        Py_ssize_t num_cords = PyList_Size(pycords);
        for (Py_ssize_t j = 0; j < num_cords; j++) {
            double value = PyFloat_AsDouble(PyList_GetItem(pycords, j));
            struct cord *c = (struct cord *)PyMem_Malloc(sizeof(struct cord));
            c->value = value;
            c->next = NULL;

            if (c_prev == NULL) {
                v->cords = c;
            } else {
                c_prev->next = c;
            }
            c_prev = c;
        }

        if (v_prev == NULL) {
            head = v;
        } else {
            v_prev->next = v;
        }
        v_prev = v;
    }

    return head;
}

// Helper function to convert a vector linked list to a Python list of lists
PyObject* convert_vector_to_pylist(struct vector *head) {
    PyObject *pylist = PyList_New(0);

    struct vector *v = head;
    while (v != NULL) {
        PyObject *pycords = PyList_New(0);
        struct cord *c = v->cords;
        while (c != NULL) {
            PyObject *value = PyFloat_FromDouble(c->value);
            PyList_Append(pycords, value);
            Py_DECREF(value);  // Decrement reference count after adding
            c = c->next;
        }
        PyList_Append(pylist, pycords);
        Py_DECREF(pycords);  // Decrement reference count after adding
        v = v->next;
    }

    return pylist;
}

/* Python wrapper for `norm` */
static PyObject* py_norm(PyObject* self, PyObject* args) {
    PyObject* py_data;
    int n;

    if (!PyArg_ParseTuple(args, "Oi", &py_data, &n)) {
        return NULL;
    }

    struct vector *data = convert_pylist_to_vector(py_data);
    if (!data) return NULL;

    struct vector *result = norm(data, n);

    PyObject* py_result = convert_vector_to_pylist(result);

    /* Cleanup */

    return py_result;
}

/* Python wrapper for `ddg` */
static PyObject* py_ddg(PyObject* self, PyObject* args) {
    PyObject* py_data;
    int n;

    if (!PyArg_ParseTuple(args, "Oi", &py_data, &n)) {
        return NULL;
    }

    struct vector *data = convert_pylist_to_vector(py_data);
    if (!data) return NULL;

    struct vector *result = ddg(data, n);

    PyObject* py_result = convert_vector_to_pylist(result);

    /* Cleanup */

    return py_result;
}

/* Python wrapper for `sym` */
static PyObject* py_sym(PyObject* self, PyObject* args) {
    PyObject* py_data;
    int n;

    if (!PyArg_ParseTuple(args, "Oi", &py_data, &n)) {
        return NULL;
    }

    struct vector* data = convert_pylist_to_vector(py_data);
    if (!data) return NULL;

    struct vector *result = sym(data, n);

    PyObject* py_result = convert_vector_to_pylist(result);
    return py_result;
}


static PyObject* py_symnmf(PyObject* self, PyObject* args) {
    PyObject *py_H, *py_W;
    int n, k;

    if (!PyArg_ParseTuple(args, "OOii", &py_H, &py_W, &n, &k)) {
        return NULL;
    }

    struct vector *H = convert_pylist_to_vector(py_H);
    if (!H) return NULL;

    struct vector *W = convert_pylist_to_vector(py_W);
    if (!W) return NULL;
    
    struct vector *result = symnmf(H, W, n, k);
    PyObject* py_result = convert_vector_to_pylist(result);

    return py_result;
}

/* Method definitions */
static PyMethodDef SymnmfMethods[] = {
    {"norm", (PyCFunction) py_norm, METH_VARARGS, "Calculate the norm of the data"},
    {"ddg", (PyCFunction) py_ddg, METH_VARARGS, "Calculate the diagonal degree matrix"},
    {"sym", (PyCFunction) py_sym, METH_VARARGS, "Calculate the symmetric matrix"},
    {"symnmf", (PyCFunction) py_symnmf, METH_VARARGS, "Calculate full symnmf"},
    {NULL, NULL, 0, NULL}
};

/* Module definition */
static struct PyModuleDef symnmfmodule = {
    PyModuleDef_HEAD_INIT,
    "symnmfmod",
    "A module that wraps C functions",
    -1,
    SymnmfMethods
};

// Initialize the module
PyMODINIT_FUNC PyInit_symnmfmod(void) {
    return PyModule_Create(&symnmfmodule);
}
