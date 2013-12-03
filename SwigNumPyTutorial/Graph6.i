// Use %extend to handle a situation that is beyond the scope of
// numpy.i

%define %graph_doc
"
Provide a Graph class for describing a network or sparse matrix
structure.
"
%enddef

%module (docstring = %graph_doc) Graph

%{
#define SWIG_FILE_WITH_INIT
#include <sstream>
#include "Graph.hpp"
%}

%feature("autodoc", "1");
%include "std_string.i"
%include "numpy.i"

%init
{
  import_array();
}

%apply (int *IN_ARRAY1, int DIM1) {(int *indices, int numIndices)};

%typemap(out) int METHOD
{
  switch ($1)
    {
    case NEGATIVE_ROW_INDEX:
      PyErr_SetString(PyExc_ValueError, "Row index is negative");
      SWIG_fail;
    case NEGATIVE_COLUMN_INDEX:
      PyErr_SetString(PyExc_ValueError, "Column index is negative");
      SWIG_fail;
    case ROW_INDEX_OUT_OF_RANGE:
      PyErr_SetString(PyExc_ValueError, "Row index is out of range");
      SWIG_fail;
    case COLUMN_INDEX_OUT_OF_RANGE:
      PyErr_SetString(PyExc_ValueError, "Column index is out of range");
      SWIG_fail;
    case USER_ARRAY_TOO_SMALL:
      PyErr_SetString(PyExc_ValueError, "User array is too small");
      SWIG_fail;
    }
  $result = Py_BuildValue("");
}

%apply int METHOD {int Graph::insertIndex,
                   int Graph::insertIndices,
                   int Graph::removeIndex,
                   int Graph::removeIndices,
                   int Graph::extractRowCopy };

%ignore operator<<(std::ostream&, const Graph &);
%ignore Graph::extractRowCopy(int row, int lenOfIndices, int *indices, int &numIndices) const;
%include "Graph.hpp"

%extend Graph
{

  std::string __str__()
  {
    std::stringstream os;
    os << *self;
    return os.str();
  }

  PyObject * extractRowCopy(int row)
  {
    int lenOfIndices = self->numNonzeros(row);
    npy_intp dims[1] = { lenOfIndices };
    PyObject * indArray = PyArray_SimpleNew(1, dims, NPY_INT);
    int *indices = (int*) array_data(indArray);
    int numIndices = 0;
    int errCode = self->extractRowCopy(row, lenOfIndices, indices, numIndices);
    if (errCode == 0)
      return indArray;
    // Bad error code
    PyErr_SetString(PyExc_ValueError, "Bad index value");
    Py_XDECREF(indArray);
    return NULL;
  }

}
