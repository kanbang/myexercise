#! /usr/bin/env python

from Graph import *

# Return a graph that represents a tridiagonal matrix
def tridiagonalGraph(rows):

    g = Graph(rows)
    for i in range(rows):
        g.insertIndex(i, i)
        if i > 0     : g.insertIndex(i, i-1)
        if i < rows-1: g.insertIndex(i, i+1)

    return g
