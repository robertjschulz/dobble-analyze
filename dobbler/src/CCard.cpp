/*
 * CCard.cpp
 *
 *  Created on: 02.01.2012
 *      Author: rosa
 */

#include "CCard.h"
#include <assert.h>
#include <iostream>

CCard::CCard()
{
  // TODO Auto-generated constructor stub

}

CCard::~CCard()
{
  // TODO Auto-generated destructor stub
}

bool CCard::contains(int s)
{
  return (symbols.find(s) != symbols.end() );
}
//bool CCard::contains(int s)
//{
//  for(iterator it = symbols.begin(); it!=symbols.end(); it++)
//    {
//      if(*it == s) return true;
//    }
//  return false;
//}

void CCard::add(int s)
{
  assert(! contains(s));
  //symbols.push_back(s);
  symbols.insert(s);
}

void CCard::remove(int s)
{
  assert(contains(s));
  symbols.erase(s);
}

CCard::CCard(gsl_combination_struct *src)
{
  for(int i=0; i< src->k; i++)
    {
      symbols.insert(gsl_combination_get(src, i)+1);
    }
}

int CCard::countSames(const CCard & _card)
{
    tsymbols::iterator it1 = symbols.begin();
    tsymbols::iterator it2 = _card.symbols.begin();
    int count = 0;
    while(it1 != symbols.end() && it2 != _card.symbols.end())
      {
        if(*it1 < *it2)
          it1++;

        else
          if(*it1 > *it2)
            it2++;

          else
            if(*it1 == *it2){
                count++;
                it1++;
                it2++;
            }
      }

    return count;
}

bool CCard::fits(const CCard & _card)
{
  assert(symbols.size() == _card.symbols.size() );
  tsymbols::iterator it1 = symbols.begin();
  tsymbols::iterator it2 = _card.symbols.begin();
  int count = 0;
  while(count <= 1 && it1 != symbols.end() && it2 != _card.symbols.end())
    {
      if(*it1 < *it2)
        it1++;

      else
        if(*it1 > *it2)
          it2++;

        else
          if(*it1 == *it2){
              count++;
              it1++;
              it2++;
          }
    }
  return(count == 1);
}

bool CCard::fits(gsl_combination_struct *candidate)
{
  assert(symbols.size() == candidate->k );
  tsymbols::iterator it1 = symbols.begin();
  int i = 0;
  int count = 0;
  while(count <= 1 && it1 != symbols.end() && i < candidate->k)
    {
      size_t v2 = gsl_combination_get(candidate, i)+1;
      if(*it1 < v2)
        it1++;
      else if(*it1 > v2)
        i++;
      else // if(*it1 == v2)
        {
          count++;
          it1++;
          i++;
        }
    }
  return(count == 1);
}

// meant for usage when filling cards with symbols.
// compare only, if there too many same symbols with one of the given card range
bool CCard::conflicts(std::list<CCard>::iterator start, std::list<CCard>::iterator end)
{
  for(std::list<CCard>::iterator it = start; it != end; it++)
    {
      if(countSames(*it) > 1) return true;
    }
  return false;
}

void CCard::listSymbols(std::ostream &os) const
{
  int i=0;
  for(tsymbols::iterator it=symbols.begin(); it != symbols.end(); it++, i++)
    {
      if(i>0) os << " ";
      os.flags ( std::ios::right  );
      os.width (3);
      os.fill(' ');
      os << *it;
    }
}


void CCard::listSymbolOffsets(std::ostream &os, int n) const
{
  if(symbols.count(1)>0) return;
  int i=0;
  for(tsymbols::iterator it=symbols.begin(); it != symbols.end(); it++, i++)
    {

      if(*it > n)
        {
          if( i>0 ) os << " ";
          os.flags ( std::ios::right  );
          os.width (3);
          os.fill(' ');
          os << (*it - 2 - i*(n-1)) ;
        }
    }
}


std::ostream & operator << (std::ostream & os, const CCard & card)
{
  card.listSymbols(os);
  return os;
}

int CCard::compare(const CCard & rh) const
{
  CCard::tsymbols::iterator it1 = symbols.begin();
  CCard::tsymbols::iterator it2 = rh.symbols.begin();
  while(it1 != symbols.end() && it2 != rh.symbols.end())
    {
      if(*it1 < *it2)
        return -1;
      else if(*it1 > *it2)
        return 1;
      it1++;
      it2++;
    }
  if(it2 != rh.symbols.end()) return -1;
  if(it1 != symbols.end()) return 1;
  return 0;
}
