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
#include <gsl/gsl_permutation.h>

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
  void print(std::ostream& o, int min_n, int max_n);
  void print_matrix_slice_x(std::ostream& os, int n, int y);

  bool m_debug;

  int getValue(int N, int x, int y);

private:
  int m_n, max_i;
  int best_i;
  std::vector<int> data;

  // will get m_n entries each for one n-plane which each gets
  // m_n entries each for one y-line which each gets
  // n entries each for one referenced plane with lower n being
  // a set of already matching lines in that plane
  std::vector<CNref > nrefs;

  static const int EMPTY;
  void initNref(int n);

  size_t getIndex(int N, int x, int y);
  void getCoords(int index, int & n, int & x, int & y);

  void setValue(int N, int x, int y, int v);

  bool checkNline(int n, int y);
  bool checkNlineSmall (int n, int y);

  bool checkN(int n);

  bool tryValues(int i, int n, int x, int y);


  void fill_matrix_slice_x(int n, int y, gsl_permutation_struct *p);
  void fill_matrix_slice_x(int n, int y, int v);

  bool isDerangementWithAbove(int n, int y,  gsl_permutation_struct *p);
  bool tryDerangement(int i, int n, int x, int y);

//  bool check();
  bool recursiveSolver(int i);

  void init(int n);


  void throwError(const std::string & s);
};

#endif /* CMATRIXSOLVER_H_ */
