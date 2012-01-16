/*
 * CMatrixSolver.cpp
 *
 *  Created on: 10.01.2012
 *      Author: rosa
 */

#include "CMatrixSolver.h"
#include <math.h>

#include <assert.h>
#include <set>
#include <list>
#include <utility>
#include <iostream>

using namespace std;

CMatrixSolver::CMatrixSolver()
: m_debug(false)
{
  // TODO Auto-generated constructor stub

}

CMatrixSolver::~CMatrixSolver()
{
  // TODO Auto-generated destructor stub
}

size_t CMatrixSolver::getIndex(int n, int x, int y)
{
    int erg = ((n * m_n) + y) * m_n + x;
//    cerr << "DEBUG: getIndex("
//        << " n=" << n
//        << ", x=" << x
//        << ", y=" << y
//        << ") = " << erg
//        << "/" << data.size()
//        << endl;
    return erg;
}

void CMatrixSolver::getCoords(int index, int & n, int & x, int & y)
{
  int i = index;
  x = i % m_n;
  i = i / m_n;
  y = i % m_n;
  i = i / m_n;
  n = i % m_n;
  i = i / m_n;
  assert(i==0);
}

int CMatrixSolver::getValue(int N, int x, int y)
{
  return data.at(getIndex(N, x, y));
}

void CMatrixSolver::setValue(int N, int x, int y, int v)
{
  data.at(getIndex(N, x, y)) = v;
}

const int CMatrixSolver::EMPTY = -1;

bool CMatrixSolver::checkN(int n)
{
//  cerr << "DEBUG: checkN(n=" << n << ")" << endl;
  bool ok = true;


  for(int _n = 1; _n < n; _n++)
  {
      for(int y=0; y < m_n; y++)
        {
          set<int> matchingLine;
          for(int x=0; x < m_n; x++)
            {
              int v = getValue(n, x, y);
              for(int _y=0; _y < m_n; _y++)
                {
                  if(getValue(_n,x,_y) == v)
                    {
                      if(matchingLine.count(_y) == 0)
                        {
                          matchingLine.insert(_y);
                          if(m_debug) cerr << "DEBUG: adding " << _y << " to matchingLine set." << endl;
                        }
                      else
                        {
                          if(m_debug)
                            {
                              cerr << "failed in line " << __LINE__ << " at this point:" << endl;
                              print (cerr);
                            }
                          return false;
                        }
                    }
                }
            }
          if(matchingLine.size() != m_n)
            {
              cerr << "DEBUG: checkN(n=" << n << "):"
                  << " matchingLine.size()=" << matchingLine.size() << " != m_n=" << m_n << "" << endl;
              print (cerr);
              assert(false);
            }
        }
  }
  return ok;
}

bool CMatrixSolver::recursiveSolver(int i)
{
//  cerr << "DEBUG: recursiveSolver(i=" << i << ")" << endl;
  assert(i<= max_i);

  int n,x,y;

  if(i == max_i)
    {
      n=m_n;
    }
  else
    {
      getCoords(i,n,x,y);
      cerr << "DEBUG: recursiveSolver(i=" << i << " -->"
          << " n=" << n
          << ", x=" << x
          << ", y=" << y
          << ")" << endl;
    }

  bool ok = true;

  if(i == max_i || ( x==0 && y==0) )
    {
      ok = checkN(n-1);
      // cerr << "DEBUG: checkN(n=" << n << ") returned:" << (ok?"true":"false") << endl;
      if(ok)
        {
        }
      else
        {
          cerr << "DEBUG: checkN(n=" << n << ") unexpectedly returned:" << (ok?"true":"false") << endl;
          print(cerr, n);
        }
    }

  if(i == max_i)
    {
      if(ok)
        {
          cout << "SOLUTION FOUND:" << endl;
          print(cout);
        }
      cout << "end of iteration(i==max_i): ok=" << (ok?"true":"false") << endl;
      return ok;
    }

  if(!ok) return false;

  if(n==0)
    {
      // ok = true;
      setValue(n, x, y, y);
      ok = recursiveSolver(i+1);
      return ok;
    }
  else if(n==1 && m_n == 5 && false)
    {
//      int a[]={1,2,3};
      int testSolution[5][5]=
          {
              {0,  1,  3,  4,  2},
              {1,  3,  2,  0,  4},
              {2,  4,  0,  3,  1},
              {3,  2,  4,  1,  0},
              {4,  0,  1,  2,  3}
          };
      setValue(n, x, y, testSolution[y][x]);
      ok = recursiveSolver(i+1);
      return ok;

    }
  else
    {
      if ( x==0 && y==0)
        {
//          cerr << "DEBUG: recursiveSolver(i=" << i << " -->"
//              << " n=" << n
//              << "...)" << endl;

          // initialize THIS planes nref
          CNref & nref = nrefs.at(n);
          for(int _y = 0; _y < m_n; _y++)
            for(int _n = n-1; _n > 0; _n--)
              {
                nref.at(_y, _n).init(m_n);
              }
        }
      CNref & nref = nrefs.at(n);

      // an der Stelle n,x,y die Werte v durch probieren
      for(int v=0; v< m_n; v++)
        {
          ok = true;
          // check vertically
          for(int _y = 0; _y < y; _y++)
            {
              if(getValue(n,x,_y) == v)
                {
                  if(m_debug)
                    {
                      cerr << "failed vertically in line " << __LINE__ << " for i=" << i << " v=" << v << " in line " << __LINE__ << " at this point:" << endl;
                      print(cerr);
                    }

                  ok=false;
                  break; // next v
                }
            }
          if(ok)
            {
              // check horizontally
              for(int _x = 0; _x < x; _x++)
                {
                  if(getValue(n,_x,y) == v)
                    {
                      if(m_debug)
                        {
                          cerr << "failed horizontally in line " << __LINE__ << " for i=" << i << " v=" << v << " in line " << __LINE__ << " at this point:" << endl;
                          print(cerr);
                        }
                      ok=false;
                      break;
                    }
                }
            }
          if(ok)
            {
              // check against previous N
              list<pair<CRef*,int> > undoCache;
              for(int _n = n-1; _n > 0; _n--)
                {
                  CRef & ref = nref.at(y, _n);
                  ok = false;
                  for(int _y=0; _y < m_n; _y++)
                    {
                      if(getValue(_n, x, _y) == v)
                        {
                          if(ref.matchingLines.count(_y) == 0)
                            {
                              ref.matchingLines.insert(_y);
                              ref.vacantLines.erase(_y);
                              undoCache.push_back(make_pair(& ref, _y));
                              ok = true;
                              break; // for y_
    //                          cerr << "DEBUG: adding " << _y << " to matchingLine set." << endl;
                            }
                          else
                            {
    //                          cerr << "failed in line " << __LINE__ << " at this point:" << endl;
    //                          print (cerr);
                            }
                        }
                    }
                  if(!ok) break; //for _n
                }
              if(ok)
                {
                  setValue(n, x, y, v);
                  ok = recursiveSolver(i+1);
                  if(ok) return true;
                }

              if(m_debug)
                {
                  cerr << "failed in plane-Refs in line " << __LINE__ << " for i=" << i << " v=" << v << " in line " << __LINE__ << " at this point:" << endl;
                  print(cerr);
                }

              // undo and try next...
              for(list<pair<CRef*,int> >::iterator it=undoCache.begin(); it!=undoCache.end(); it++)
                {
                  CRef & ref = *(it->first);
                  int _y = it->second;
                  ref.matchingLines.erase(_y);
                  ref.vacantLines.insert(_y);
                }
              setValue(n, x, y, -1);

              // ok = checkN(n,x,y);
            }
        }

        if(m_debug)
        {
//      if(x==0 && y==0 && n >= m_n-3)
          cerr << "DEBUG: recursiveSolver(i=" << i << " -->"
              << " n=" << n
              << ", x=" << x
              << ", y=" << y
              << ") returns false" << endl;
//          print(cerr, n-1);
        }
      return false;
    }
//  cerr << "DEBUG: recursiveSolver(i=" << i << ") returns " << (ok?"true":"false") << endl;

}

void CMatrixSolver::init(int _n)
{
  m_n = _n;
  max_i = pow(m_n , 3) ;
  data.clear();
  data.resize(max_i, EMPTY);
  cerr << "DEBUG: init(m_n=" << m_n << ")" << "=" << max_i << endl;

  // nrefs initialisieren
  nrefs.clear();
  nrefs.assign(m_n, CNref() );
  for(int i=1;i<m_n;i++)
    {
      nrefs.at(i).init(m_n, i); // wir brauchen nur i-1, aber dann sind die indizes blöd...
    }

}


bool CMatrixSolver::solve(int n)
{
  init(n);

  bool ok = recursiveSolver(0);
  return ok;
}

void  CMatrixSolver::print(ostream & os, int max_n)
{
  if (max_n == -1) max_n = m_n;
  for(int N = 0;  N< m_n; N++)
    {
      os << endl;
      os << "N=" << N << endl;
      for( int y=0; y<m_n; y++)
        {
          for(int x=0; x<m_n; x++)
            {
              os.flags ( ios::right  );
              os.width (3);
              os.fill(' ');
              os << getValue(N,x,y);
            }
          os << endl;

        }
    }
}


