/*
 * CMatrixSolver.h
 *
 *  Created on: 10.01.2012
 *      Author: rosa
 */

#ifndef CMATRIXSOLVER_H_
#define CMATRIXSOLVER_H_

#include <ostream>
#include <vector>
#include <set>

template<class C>
class MultiDimensionalArray
{
  std::vector<int> dimensions;
  unsigned long int m_size;
  std::vector<C> values;
  MultiDimensionalArray(const std::vector<int> & _dimensions, const C& initalValue)
  {
    dimensions = _dimensions;
    m_size=1;
    for(std::vector<int>::iterator it=dimensions.begin(); it != dimensions.end(); it++)
      {
        m_size *= *it;
      }
    values.assign(m_size, initalValue);
  };
};


class CRef
{
public:
  int m_n;
  std::set<int> matchingLines;
  std::set<int> vacantLines;
  void init(int _n) {
    m_n=_n;
    matchingLines.clear();
    vacantLines.clear();
    for(int n=0; n<_n; n++) vacantLines.insert(n);
  }
};

class CNref
{

public:
  std::vector<std::vector<CRef> >refs;

  CNref()  {  };

  void init(int numLines, int numReferencedPlanes)
  {
    refs.assign(numLines, std::vector<CRef>(numReferencedPlanes,CRef()) );
  };

  CRef & at(int _y, int _n){return refs.at(_y).at(_n);}
};

class CMatrixSolver
{
public:
  CMatrixSolver();
  virtual
  ~CMatrixSolver();

  bool solve(int n);
  void print(std::ostream& o, int max_n=-1);
  bool m_debug;

private:
  int m_n, max_i;
  std::vector<int> data;

  // will get m_n entries each for one n-plane which each gets
  // m_n entries each for one y-line which each gets
  // n entries each for one referenced plane with lower n being
  // a set of already matching lines in that plane
  std::vector<CNref > nrefs;

  static const int EMPTY;

  size_t getIndex(int N, int x, int y);
  void getCoords(int index, int & n, int & x, int & y);

  int getValue(int N, int x, int y);
  void setValue(int N, int x, int y, int v);

  bool checkN(int n);

  void init(int n);

//  bool check();
  bool recursiveSolver(int i);
};

#endif /* CMATRIXSOLVER_H_ */
