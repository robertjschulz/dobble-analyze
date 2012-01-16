/*
 * CCard.h
 *
 *  Created on: 02.01.2012
 *      Author: rosa
 */

#ifndef CCARD_H_
#define CCARD_H_

#include <set>
#include <ostream>
#include <list>

class CCard
{
public:
  typedef std::set<int> tsymbols;
  typedef tsymbols::iterator iterator;
  tsymbols symbols;

public:
  CCard();
  CCard(const CCard & src):symbols(src.symbols){};
  virtual
  ~CCard();

  bool contains(int s);
  void add(int s);
  void remove(int s);

  bool fits(const CCard & _card);

  bool conflicts(std::list<CCard>::iterator start, std::list<CCard>::iterator end);

  int size() { return symbols.size(); }

  void listSymbols(std::ostream &os) const;
  void listSymbolOffsets(std::ostream &os, int n) const;

  int compare(const CCard & rh) const;
  bool operator < (const CCard & rhs){return (compare(rhs) < 0);}

private:
    int countSames(const CCard & _card);

};

std::ostream & operator << (std::ostream & os, const CCard & card);

//bool operator < (const CCard & lhs, const CCard & rhs){return (lhs.compare(rhs) < 0);}


#endif /* CCARD_H_ */
