#! /usr/bin/env python

from distutils.core import *
from numpy          import get_include

_Graph = Extension("_Graph",
                   ["Graph_wrap.cpp", "Graph.cpp"],
                   include_dirs = [get_include()]
                   )

setup(name        = "Graph",
      py_modules  = ["Graph"],
      ext_modules = [_Graph]
      )
