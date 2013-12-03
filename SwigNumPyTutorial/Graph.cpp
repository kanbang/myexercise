#include "Graph.hpp"

////////////////////////////////////////////////////////////////////////
// Constructors and destructor
////////////////////////////////////////////////////////////////////////

// Constructor for a given number of rows
Graph::Graph(int rows) :
  _rows(rows)
{
  // If user gives us a negative number of rows, create an unusable
  // Graph
  if (_rows < 0) _rows = 0;

  // Allocate the column data
  _columns = VecSetInt(_rows);
}

// Destructor
Graph::~Graph()
{
}


////////////////////////////////////////////////////////////////////////
// Insertion and removal methods
////////////////////////////////////////////////////////////////////////

// Insert a single index into the graph
int Graph::insertIndex(int row, int index)
{
  // Error checking
  if (row   <  0    ) return NEGATIVE_ROW_INDEX;
  if (row   >= _rows) return ROW_INDEX_OUT_OF_RANGE;
  if (index <  0    ) return NEGATIVE_COLUMN_INDEX;
  if (index >= _rows) return COLUMN_INDEX_OUT_OF_RANGE;

  // Insert the index into the graph
  _columns[row].insert(index);

  // Successful completion
  return SUCCESS;
}

// insert an array of indices into the graph
int Graph::insertIndices(int row, int *indices, int numIndices)
{
  // Error checking
  if (row <  0    ) return NEGATIVE_ROW_INDEX;
  if (row >= _rows) return ROW_INDEX_OUT_OF_RANGE;

  // Insert the indices into the graph
  for (int j=0; j<numIndices; ++j)
    {
      int index = indices[j];
      if (index <  0    ) return NEGATIVE_COLUMN_INDEX;
      if (index >= _rows) return COLUMN_INDEX_OUT_OF_RANGE;
      _columns[row].insert(index);
    }

  // Successful completion
  return SUCCESS;
}

// remove a single index from the graph
int Graph::removeIndex(int row, int index)
{
  // Error checking
  if (row   <  0    ) return NEGATIVE_ROW_INDEX;
  if (row   >= _rows) return ROW_INDEX_OUT_OF_RANGE;
  if (index <  0    ) return NEGATIVE_COLUMN_INDEX;
  if (index >= _rows) return COLUMN_INDEX_OUT_OF_RANGE;

  // Remove the index from the graph
  _columns[row].erase(index);

  // Successful completion
  return SUCCESS;
}

// Remove an array of indices from the graph
int Graph::removeIndices(int row, int *indices, int numIndices)
{
  // Error checking
  if (row <  0    ) return NEGATIVE_ROW_INDEX;
  if (row >= _rows) return ROW_INDEX_OUT_OF_RANGE;

  // Remove the indices from the graph
  for (int j=0; j<numIndices; ++j)
    {
      int index = indices[j];
      if (index <  0    ) return NEGATIVE_COLUMN_INDEX;
      if (index >= _rows) return COLUMN_INDEX_OUT_OF_RANGE;
      _columns[row].erase(index);
    }

  // Successful completion
  return SUCCESS;
}

// Remove all indices from a given row of the graph
int Graph::removeIndices(int row)
{
  // Error checking
  if (row <  0    ) return NEGATIVE_ROW_INDEX;
  if (row >= _rows) return ROW_INDEX_OUT_OF_RANGE;

  // Mark the indices as removed
  _columns[row].clear();

  // Successful completion
  return SUCCESS;
}


////////////////////////////////////////////////////////////////////////
// Extraction methods
////////////////////////////////////////////////////////////////////////

// Extract a copied array of indices from the given row of the graph
int Graph::extractRowCopy(int row, int lenOfIndices, int *indices, int &numIndices) const
{
  // Error checking
  if (row <  0    ) return NEGATIVE_ROW_INDEX;
  if (row >= _rows) return ROW_INDEX_OUT_OF_RANGE;

  // Check that the given array is big enough
  numIndices = _columns[row].size();
  if (lenOfIndices < numIndices) return USER_ARRAY_TOO_SMALL;

  // Copy the indices from the graph to the given array
  int j = 0;
  for (SetInt_it sii=_columns[row].begin(); sii != _columns[row].end(); ++sii)
    indices[j++] = *sii;

  // Successful completion
  return SUCCESS;
}


////////////////////////////////////////////////////////////////////////
// Query methods
////////////////////////////////////////////////////////////////////////

// Return whether a given row/column index pair is a nonzero in the graph
bool Graph::isIndex(int row, int index) const
{
  if ((row >= 0) and (row < _rows))
    {
      SetInt_it sii = _columns[row].find(index);
      return (sii != _columns[row].end());
    }
  else
    {
      return false;
    } 
}

// Return whether the graph is lower triangular
bool Graph::lowerTriangular() const
{
  for (int row=0; row<_rows; ++row)
    {
      for (SetInt_it sii = _columns[row].begin(); sii != _columns[row].end(); ++sii)
	if (*sii > row) return false;
    }
  return true;
}

// Return whether the graph is upper triangular
bool Graph::upperTriangular() const
{
  for (int row=0; row<_rows; ++row)
    {
      for (SetInt_it sii = _columns[row].begin(); sii != _columns[row].end(); ++sii)
	if (*sii < row) return false;
    }
  return true;
}

// Return whether the graph is purely diagonal
bool Graph::diagonal() const
{
  for (int row=0; row<_rows; ++row)
    {
      if ( _columns[row].size()  >  1  ) return false;
      if (*_columns[row].begin() != row) return false;
    }
  return true;
}

// Return whether all diagonal entries of the graph are zero
bool Graph::noDiagonal() const
{
  for (int row=0; row<_rows; ++row)
    {
      SetInt_it index = _columns[row].find(row);
      if (index != _columns[row].end()) return false;
    }
  return true;
}


////////////////////////////////////////////////////////////////////////
// Attribute access methods
////////////////////////////////////////////////////////////////////////

// Return the number of rows in the graph
int Graph::numRows() const
{
  return _rows;
}

// Return the maximum number of nonzeros per row
int Graph::maxNumNonzeros() const
{
  size_t result = 0;
  for (int row=0; row<_rows; ++row)
    result = result < _columns[row].size() ? _columns[row].size() : result;
  return (int) result;
}

// Return the number of nonzeros for a given row
int Graph::numNonzeros(int row) const
{
  // Error checking
  if (row <  0    ) return NEGATIVE_ROW_INDEX;
  if (row >= _rows) return ROW_INDEX_OUT_OF_RANGE;

  return _columns[row].size();
}

// Return the total number of nonzeros in the graph
int Graph::numNonzeros() const
{
  int result = 0;
  for (int row=0; row<_rows; ++row)
    result += _columns[row].size();
  return result;
}

// Return the number of diagonals with nonzeros in the graph
int Graph::numDiagonals() const
{
  int upperDiagonals = 0;
  int lowerDiagonals = 0;
  for (int row=0; row<_rows; ++row)
    {
      for (SetInt_it sii = _columns[row].begin(); sii != _columns[row].end(); ++sii)
	{
	  if (*sii > row)
	    upperDiagonals = upperDiagonals < *sii-row ? *sii-row : upperDiagonals;
	  else
	    lowerDiagonals = lowerDiagonals < row-*sii ? row-*sii : lowerDiagonals;
	}
    }
  return lowerDiagonals + upperDiagonals + 1;
}


////////////////////////////////////////////////////////////////////////
// I/O operators
////////////////////////////////////////////////////////////////////////

// Define a std::ostream << operator
std::ostream & operator<<(std::ostream & os, const Graph & g)
{
  for (int row=0; row < g._rows; ++row)
    {
      os << row << ": ";
      for (std::set<int>::iterator sii = g._columns[row].begin();
	   sii != g._columns[row].end(); ++sii)
	os << *sii << " ";
      os << std::endl;
    }
  return os;
}
