// Add module documentation and automatically generated docstrings for
// class methods

%define %graph_doc
"
Provide a Graph class for describing a network or sparse matrix
structure.
"
%enddef

%module (docstring = %graph_doc) Graph

%{
#include "Graph.hpp"
%}

%feature("autodoc", "1");

%include "Graph.hpp"
