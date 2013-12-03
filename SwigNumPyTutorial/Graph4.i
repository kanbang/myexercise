// Change from integer return codes to python exceptions

%define %graph_doc
"
Provide a Graph class for describing a network or sparse matrix
structure.
"
%enddef

%module (docstring = %graph_doc) Graph

%{
#include <sstream>
#include "Graph.hpp"
%}

%feature("autodoc", "1");
%include "std_string.i"

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
%include "Graph.hpp"

%extend Graph
{

  std::string __str__()
  {
    std::stringstream os;
    os << *self;
    return os.str();
  }

}
