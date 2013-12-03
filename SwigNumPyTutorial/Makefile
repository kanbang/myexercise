
all: Graph

Graph_wrap.cpp: Graph.i Graph.hpp Graph.cpp
	swig -c++ -python -o $@ $<

Graph: Graph_wrap.cpp
	python setup.py build

clean:
	$(RM) *~
	$(RM) Graph.i
	$(RM) Graph_wrap.cpp
	$(RM) Graph.py
	$(RM) -r build
