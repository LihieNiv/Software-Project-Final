#define PY_SSIZE_T_CLEAN
#include <Python.h>

struct cord {
    double value;
    struct cord *next;
};

struct vector {
    struct vector *next;
    struct cord *cords;
};


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