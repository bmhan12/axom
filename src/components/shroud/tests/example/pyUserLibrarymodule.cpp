// pyUserLibrarymodule.cpp
// This is generated code, do not edit
// blah blah
// yada yada
//
#include "pyUserLibrarymodule.hpp"
// splicer begin include
// splicer end include

namespace example {
namespace nested {
// splicer begin C_definition
// splicer end C_definition
PyObject *PP_error_obj;
// splicer begin additional_functions
// splicer end additional_functions

static char PP_local_function1__doc__[] =
"documentation"
;

static PyObject *
PP_local_function1(
  PyObject *self,    /* not used */
  PyObject *args,
  PyObject *kwds)
{
// splicer begin function.local_function1
    local_function1();
    Py_RETURN_NONE;
// splicer end function.local_function1
}

static char PP_is_name_valid__doc__[] =
"documentation"
;

static PyObject *
PP_is_name_valid(
  PyObject *self,    /* not used */
  PyObject *args,
  PyObject *kwds)
{
// splicer begin function.is_name_valid
    char * name;
    const char *kwcpp = "name";
    char *kw_list[] = { (char *) kwcpp+0, NULL };
    
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "s:isNameValid", kw_list,
        &name))
    {
        return NULL;
    }
    std::string SH_name(name);
    bool rv = isNameValid(SH_name);
    PyObject * SH_Py_rv = PyBool_FromLong(rv);
    return (PyObject *) SH_Py_rv;
// splicer end function.is_name_valid
}

static char PP_is_initialized__doc__[] =
"documentation"
;

static PyObject *
PP_is_initialized(
  PyObject *self,    /* not used */
  PyObject *args,
  PyObject *kwds)
{
// splicer begin function.is_initialized
    bool rv = isInitialized();
    PyObject * SH_Py_rv = PyBool_FromLong(rv);
    return (PyObject *) SH_Py_rv;
// splicer end function.is_initialized
}

static PyObject *
PP_test_names(
  PyObject *self,    /* not used */
  PyObject *args,
  PyObject *kwds)
{
// splicer begin function.test_names
    char * name;
    const char *kwcpp = "name";
    char *kw_list[] = { (char *) kwcpp+0, NULL };
    
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "s:test_names", kw_list,
        &name))
    {
        return NULL;
    }
    std::string SH_name(name);
    test_names(SH_name);
    Py_RETURN_NONE;
// splicer end function.test_names
}

static PyObject *
PP_test_names_flag(
  PyObject *self,    /* not used */
  PyObject *args,
  PyObject *kwds)
{
// splicer begin function.test_names_flag
    char * name;
    int flag;
    const char *kwcpp = "name\0flag";
    char *kw_list[] = { (char *) kwcpp+0,(char *) kwcpp+5, NULL };
    
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "si:test_names", kw_list,
        &name, &flag))
    {
        return NULL;
    }
    std::string SH_name(name);
    test_names(SH_name, flag);
    Py_RETURN_NONE;
// splicer end function.test_names_flag
}

static char PP_testoptional_2__doc__[] =
"documentation"
;

static PyObject *
PP_testoptional_2(
  PyObject *self,    /* not used */
  PyObject *args,
  PyObject *kwds)
{
// splicer begin function.testoptional
    Py_ssize_t SH_nargs = 0;
    int i;
    long j;
    const char *kwcpp = "i\0j";
    char *kw_list[] = { (char *) kwcpp+0,(char *) kwcpp+2, NULL };
    
    if (args != NULL) SH_nargs += PyTuple_Size(args);
    if (kwds != NULL) SH_nargs += PyDict_Size(args);
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|il:testoptional", kw_list,
        &i, &j))
    {
        return NULL;
    }
    switch (SH_nargs) {
    case 0:
        testoptional();
        break;
    case 1:
        testoptional(i);
        break;
    case 2:
        testoptional(i, j);
        break;
    }
    Py_RETURN_NONE;
// splicer end function.testoptional
}

static char PP_testmpi__doc__[] =
"documentation"
;

static PyObject *
PP_testmpi(
  PyObject *self,    /* not used */
  PyObject *args,
  PyObject *kwds)
{
// splicer begin function.testmpi
    MPI_Comm comm;
    const char *kwcpp = "comm";
    char *kw_list[] = { (char *) kwcpp+0, NULL };
    
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O:testmpi", kw_list,
        &comm))
    {
        return NULL;
    }
    testmpi(MPI_Comm_f2c(comm));
    Py_RETURN_NONE;
// splicer end function.testmpi
}

static char PP_test_names__doc__[] =
"documentation"
;

static PyObject *
PP_test_names(
  PyObject *self,    /* not used */
  PyObject *args,
  PyObject *kwds)
{
// splicer begin function.test_names
    Py_ssize_t SH_nargs = 0;
    if (args != NULL) SH_nargs += PyTuple_Size(args);
    if (kwds != NULL) SH_nargs += PyDict_Size(args);
    PyObject *rvobj;
    if (SH_nargs == 1) {
        rvobj = PP_test_names(self, args, kwds);
        if (!PyErr_Occurred()) {
            return rvobj;
        } else if (! PyErr_ExceptionMatches(PyExc_TypeError)) {
            return rvobj;
        }
        PyErr_Clear();
    }
    if (SH_nargs == 2) {
        rvobj = PP_test_names_flag(self, args, kwds);
        if (!PyErr_Occurred()) {
            return rvobj;
        } else if (! PyErr_ExceptionMatches(PyExc_TypeError)) {
            return rvobj;
        }
        PyErr_Clear();
    }
    PyErr_SetString(PyExc_TypeError, "wrong arguments multi-dispatch");
    return NULL;
// splicer end function.test_names
}
static PyMethodDef PP_methods[] = {
{"local_function1", (PyCFunction)PP_local_function1, METH_NOARGS, PP_local_function1__doc__},
{"isNameValid", (PyCFunction)PP_is_name_valid, METH_VARARGS|METH_KEYWORDS, PP_is_name_valid__doc__},
{"isInitialized", (PyCFunction)PP_is_initialized, METH_NOARGS, PP_is_initialized__doc__},
{"testoptional", (PyCFunction)PP_testoptional_2, METH_VARARGS|METH_KEYWORDS, PP_testoptional_2__doc__},
{"testmpi", (PyCFunction)PP_testmpi, METH_VARARGS|METH_KEYWORDS, PP_testmpi__doc__},
{"test_names", (PyCFunction)PP_test_names, METH_VARARGS|METH_KEYWORDS, PP_test_names__doc__},
{NULL,   (PyCFunction)NULL, 0, NULL}            /* sentinel */
};

/*
 * inituserlibrary - Initialization function for the module
 * *must* be called inituserlibrary
 */
static char PP__doc__[] =
"library documentation"
;

struct module_state {
    PyObject *error;
};

#ifdef IS_PY3K
#define GETSTATE(m) ((struct module_state*)PyModule_GetState(m))
#else
#define GETSTATE(m) (&_state)
static struct module_state _state;
#endif

#ifdef IS_PY3K
static int userlibrary_traverse(PyObject *m, visitproc visit, void *arg) {
    Py_VISIT(GETSTATE(m)->error);
    return 0;
}

static int userlibrary_clear(PyObject *m) {
    Py_CLEAR(GETSTATE(m)->error);
    return 0;
}

static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "userlibrary", /* m_name */
    PP__doc__, /* m_doc */
    sizeof(struct module_state), /* m_size */
    PP_methods, /* m_methods */
    NULL, /* m_reload */
    userlibrary_traverse, /* m_traverse */
    userlibrary_clear, /* m_clear */
    NULL  /* m_free */
};

#define RETVAL m
#define INITERROR return NULL
#else
#define RETVAL
#define INITERROR return
#endif

#ifdef __cplusplus
extern "C" {
#endif
PyMODINIT_FUNC
MOD_INITBASIS(void)
{
    PyObject *m = NULL;
    const char * error_name = "userlibrary.Error";

// splicer begin C_init_locals
// splicer end C_init_locals


    /* Create the module and add the functions */
#ifdef IS_PY3K
    m = PyModule_Create(&moduledef);
#else
    m = Py_InitModule4("userlibrary", PP_methods,
                       PP__doc__,
                       (PyObject*)NULL,PYTHON_API_VERSION);
#endif
    if (m == NULL)
        return RETVAL;
    struct module_state *st = GETSTATE(m);


// ExClass1
    PP_ExClass1_Type.tp_new   = PyType_GenericNew;
    PP_ExClass1_Type.tp_alloc = PyType_GenericAlloc;
    if (PyType_Ready(&PP_ExClass1_Type) < 0)
        return RETVAL;
    Py_INCREF(&PP_ExClass1_Type);
    PyModule_AddObject(m, "ExClass1", (PyObject *)&PP_ExClass1_Type);


// ExClass2
    PP_ExClass2_Type.tp_new   = PyType_GenericNew;
    PP_ExClass2_Type.tp_alloc = PyType_GenericAlloc;
    if (PyType_Ready(&PP_ExClass2_Type) < 0)
        return RETVAL;
    Py_INCREF(&PP_ExClass2_Type);
    PyModule_AddObject(m, "ExClass2", (PyObject *)&PP_ExClass2_Type);


    PP_error_obj = PyErr_NewException((char *) error_name, NULL, NULL);
    if (PP_error_obj == NULL)
        return RETVAL;
    st->error = PP_error_obj;
    PyModule_AddObject(m, "Error", st->error);

// splicer begin C_init_body
// splicer end C_init_body

    /* Check for errors */
    if (PyErr_Occurred())
        Py_FatalError("can't initialize module userlibrary");
    return RETVAL;
}
#ifdef __cplusplus
}
#endif

}  // namespace example
}  // namespace nested