// Add a __str__() method to the Graph class so that the python print
// statement will provide the same output as the C++ << operator.
// Also, get rid of the swig warning by using %ignore.

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
