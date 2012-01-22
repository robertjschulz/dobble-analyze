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
#include <stdexcept>

#include "CPermutation.h"

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

bool CMatrixSolver::checkNline(int n, int y)
{
  for(int _n = 1; _n < n; _n++)
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

// this version does not check for x=0!
// because we always set
bool CMatrixSolver::checkNlineSmall(int n, int y)
{
  for(int _n = 1; _n < n; _n++)
    {
      int matchingLine[m_n];
      int matchingLines=1; // x==0 matches _y !
      for(int i=0; i < m_n; i++) matchingLine[i]=0;

      for(int x=1; x < m_n; x++)
        {
          int v = getValue(n, x, y);
          for(int _y=0; _y < m_n; _y++)
            {
              int _v = getValue(_n, x, _y);
              if(_y==y)
                {
                  if(_v==v) return false;
                  continue;
                }
              if(_v == v)
                {
                  if(matchingLine[_y] == 0)
                    {
                      matchingLine[_y] = 1;
                      matchingLines++;
                      // if(m_debug) cerr << "DEBUG: adding " << _y << " to matchingLine set." << endl;
                    }
                  else
                    {
//                      if(m_debug)
//                        {
//                          cerr << "failed in line " << __LINE__ << " at this point:" << endl;
//                          print (cerr);
//                        }
                      return false;
                    }
                }
            }
        }
      if(matchingLines != m_n)
        {
          cerr << "DEBUG: checkN(n=" << n << "):"
              << " matchingLines=" << matchingLines << " != m_n=" << m_n << "" << endl;
          print (cerr);
          assert(false);
        }
    }
  return true;
}


bool CMatrixSolver::checkN(int n)
{
//  cerr << "DEBUG: checkN(n=" << n << ")" << endl;
  bool ok = true;


  for(int y=0; y < m_n; y++)
    {
      if(! checkNline(n,y)) return false;
  }
  return ok;
}


bool CMatrixSolver::tryValues(int i, int n, int x, int y)
{

  if ( x==0 && y==0)
    {
//          cerr << "DEBUG: recursiveSolver(i=" << i << " -->"
//              << " n=" << n
//              << "...)" << endl;

      // initialize THIS planes nref
        initNref(n);
    }
  CNref & nref = nrefs.at(n);

  // an der Stelle n,x,y die Werte v durch probieren
  for(int v=0; v< m_n; v++)
    {
      bool ok = true;
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

// initializes the References to the previous planes with smaller n
void CMatrixSolver::initNref(int n)
{
    //          cerr << "DEBUG: recursiveSolver(i=" << i << " -->"
    //              << " n=" << n
    //              << "...)" << endl;
    // initialize THIS planes nref
    CNref & nref = nrefs.at(n);
    for(int _y = 0;_y < m_n;_y++)
        for(int _n = n - 1;_n > 0;_n--){
            nref.at(_y, _n).init(m_n);
        }

}

void CMatrixSolver::throwError(const std::string & s)
{
  cerr << "failed with message: " << s << endl;
  print(cerr);
  throw runtime_error(s);
}

bool isDerangement(const list<gsl_permutation> & derangementsAbove, CPermutation & p)
{
  for(int i=0; i< p.s.size; i++)
    {
      int vali = gsl_permutation_get(&(p.s), i);
      for(list<gsl_permutation>::const_iterator it=derangementsAbove.begin();
          it != derangementsAbove.end(); it++)
        {
          if( gsl_permutation_get(&(*it), i) ==vali) return false;
        }
    }
  return true;
}

// fills the gicen slice for each x(0..m_n-a)
// if size of given permutation is m_n-1 V(n,0,y) will be set to y

void CMatrixSolver::fill_matrix_slice_x(int n, int y, gsl_permutation_struct *p)
{
  int x = 0;
  int offset = 0;
  if(p->size == m_n-1)
    {
      setValue(n, x, y, y);
      x++;
      offset=1;
    }
  for( ; x < m_n; x++)
    {
        unsigned long int v = gsl_permutation_get(p, x - offset);
        if(v >= y) v++;
        setValue(n, x, y, v);
    }
}

void CMatrixSolver::fill_matrix_slice_x(int n, int y, int v)
{
  for( int x = 0; x < m_n; x++)
    {
      setValue(n, x, y, v);
    }
}

// does NOT check x=0!
bool CMatrixSolver::isDerangementWithAbove(int n, int y,  gsl_permutation_struct *p)
{
  for( int x=1 ; x < m_n; x++)
    {
      int ry=getValue(n,x,y);
      for(int _y=0; _y < y; _y++)
        {
          int ty = getValue(n,x,_y);
          if(ry==ty) return false;
        }
    }
  return true;
}

bool CMatrixSolver::tryDerangement(int i, int n, int x, int y)
{
  //if(x != 0) throw runtime_error("x != 0");
  if(x!=0) throwError("wromng state: x MUST be 0!");

  // not needed here!
  // CNref & nref = nrefs.at(n);


  list<gsl_permutation> derangementsAbove;
  // an der Stelle n,y die horizontalen Permutationen / durchgehen
  // oBdA V(n,0,y)=y

  bool do_print=false;
  if( i > best_i )
    {
      do_print = true;
      print(cerr,n);
    }
  int trynr=0;
  gsl_permutation_struct *p = gsl_permutation_calloc(m_n-1);
  do {
      int reason = 0;
      trynr++;
      fill_matrix_slice_x(n,y, p);

      if(m_debug ) print(cerr,n);
      if(isDerangementWithAbove(n, y, p))
        {
          // more checks...

          // check against n-_n:
          bool ok=false;
          if ( checkNlineSmall(n,y) )
            {
              // fine! continue to next line!
              if( i > best_i ) best_i = i;
              ok = recursiveSolver(i + m_n);
              if(ok) return ok;
            }
          else
            {
              reason = 2;
            }
          // when reaching here: check next permutation!
        }
      else
        {
          reason = 1;
        }
      // TODO: possibly clean fill_matrix_slice_x !?
      if(m_debug);
      if( i > best_i )
        {
          cerr << "DEBUG: tryDerangement(i=" << i << " -->"
          << " n=" << n
          << ", x=" << x
          << ", y=" << y
          << ") -- try nr.";
          cerr.flags ( ios::right  );
          cerr.width (3);
          cerr.fill(' ');
          cerr << trynr;
          cerr << " line : ";
          print_matrix_slice_x(cerr, n,y);
          const char* reasonStr =
              reason== 1 ? "no derangement"
                  : reason== 2 ? "checkN"
                      : "???";
          cerr << " failed with reason: " << reasonStr
              << endl;
        }
      fill_matrix_slice_x(n,y, -1);
  } while(gsl_permutation_next(p) == GSL_SUCCESS);
  gsl_permutation_free(p);

  // when reaching here: everything was tried!

  return false;
}

bool CMatrixSolver::recursiveSolver(int i)
{
//  cerr << "DEBUG: recursiveSolver(i=" << i << ")" << endl;
  assert(i<= max_i);

  int n,x,y;

  if(i == max_i)
    {
      n=m_n;
      bool ok = checkN(n-1);
      if(ok)
        {
          if(m_debug) cout << "SOLUTION FOUND:" << endl;
          if(m_debug) print(cout);
        }
      cout << "end of iteration(i==max_i): ok=" << (ok?"true":"false") << endl;
      return ok;
    }

  getCoords(i,n,x,y);
  if(m_debug || i > best_i)
    {
      cerr << "DEBUG: recursiveSolver(i=" << i << " -->"
          << " n=" << n
          << ", x=" << x
          << ", y=" << y
          << ")"
          << " best_i=" << best_i
          << endl;
    }

  bool ok = true;

//  if(( x==0 && y==0) )
//    {
//      ok = checkN(n-1);
//      // cerr << "DEBUG: checkN(n=" << n << ") returned:" << (ok?"true":"false") << endl;
//      if(ok)
//        {
//        }
//      else
//        {
//          cerr << "DEBUG: checkN(n=" << n << ") unexpectedly returned:" << (ok?"true":"false") << endl;
//          print(cerr, n);
//        }
//    }
//
//  if(!ok) return false;

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
  else if(n==1 && y == 0)
    {
      setValue(n, x, y, x);
      ok = recursiveSolver(i+1);
      return ok;
    }
  else
    {
      if(false && n==1 && y == 0 )
        {
          // print status
          cerr << "DEBUG: recursiveSolver(i=" << i << " -->"
              << " n=" << n
              << ", x=" << x
              << ", y=" << y
              << ")" << endl;
          print(cerr, n, n);
        }

      return tryDerangement(i, n, x, y);

      // old slower version
      // return tryValues(i, n, x, y);
    }
//  cerr << "DEBUG: recursiveSolver(i=" << i << ") returns " << (ok?"true":"false") << endl;

}

void CMatrixSolver::init(int _n)
{
  m_n = _n;
  max_i = pow(m_n , 3) ;
  best_i = 0;
  data.clear();
  data.resize(max_i, EMPTY);
  cerr << "DEBUG: init(m_n=" << m_n << ")" << " max_i=" << max_i << endl;

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

void CMatrixSolver::print_matrix_slice_x(std::ostream& os, int n, int y)
{
  for(int x=0; x<m_n; x++)
    {
      os.flags ( ios::right  );
      os.width (3);
      os.fill(' ');
      os << getValue(n,x,y);
    }
}

void CMatrixSolver::print(std::ostream& os, int min_n, int max_n)
{
  for(int N = min_n;  N<= max_n; N++)
    {
      os << endl;
      os << "N=" << N << endl;
      for( int y=0; y<m_n; y++)
        {
          print_matrix_slice_x(os, N, y);
          os << endl;
        }
    }
}

void  CMatrixSolver::print(ostream & os, int max_n)
{
  if(max_n == -1) max_n=m_n-1;
  print(os, 0, max_n);
}


