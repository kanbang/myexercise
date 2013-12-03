#ifndef GRAPH_H
#define GRAPH_H

// Include headers
#include <vector>
#include <set>
#include <iostream>

// Graph error codes
#define SUCCESS                     0
#define NEGATIVE_ROW_INDEX         -1
#define NEGATIVE_COLUMN_INDEX      -2
#define ROW_INDEX_OUT_OF_RANGE     -3
#define COLUMN_INDEX_OUT_OF_RANGE  -4
#define USER_ARRAY_TOO_SMALL       -5

class Graph
{
public:
  // Constructors and destructor
  Graph(int rows);
  ~Graph();

  // Insertion and removal methods
  int insertIndex(int row, int index);
  int insertIndices(int row, int *indices, int numIndices);
  int removeIndex(int row, int index);
  int removeIndices(int row, int *indices, int numIndices);
  int removeIndices(int row);

  // Extraction methods
  int extractRowCopy(int row, int lenOfIndices, int *indices, int &numIndices) const;

  // Query methods
  bool isIndex(int row, int index) const;
  bool lowerTriangular() const;
  bool upperTriangular() const;
  bool diagonal() const;
  bool noDiagonal() const;

  // Attribute access methods
  int numRows() const;
  int maxNumNonzeros() const;
  int numNonzeros(int row) const;
  int numNonzeros() const;
  int numDiagonals() const;

  // I/O operators
  friend
  std::ostream & operator<<(std::ostream& os, const Graph & g);

private:
  typedef std::set<int>           SetInt;
  typedef std::set<int>::iterator SetInt_it;
  typedef std::vector<SetInt>     VecSetInt;

  int       _rows;
  VecSetInt _columns;

};

#endif
