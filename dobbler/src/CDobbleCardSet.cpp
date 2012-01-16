/*
 * CDobbleCardSet.cpp
 *
 *  Created on: 02.01.2012
 *      Author: rosa
 */

#include <algorithm>
#include <vector>
#include <map>
#include <ostream>
#include <iostream>
#include <assert.h>

#include "CDobbleCardSet.h"
#include "CCard.h"

#include <NTL/ZZ.h>

// NTL::set conflicts with std::set
// NTL_CLIENT

using namespace std;

template <class Iterator>
void listCards(std::ostream &os, Iterator start, Iterator end)
{
  int i=1;
  for(Iterator it=start; it != end; it++, i++)
    {
      os << "card nr." << i << ": ";
      it->listSymbols(os);
      os << std::endl;
    }
}

CDobbleCardSet::CDobbleCardSet(int _s, int _n)
: s(_s)
, n(_n)
{
  clear();
}

CDobbleCardSet::CDobbleCardSet(int _n)
: n(_n)
{
  s=n+(n-1)*(n-1);
  clear();
}

CDobbleCardSet::~CDobbleCardSet() {
	// TODO Auto-generated destructor stub
}

void CDobbleCardSet::clear()
{
  cards.clear();
}

//struct CCardCompare {
//  bool operator() (const CCard & lhs, const CCard & rhs) { return (lhs<rhs);}
//} cCardCompare;

void CDobbleCardSet::sort()
{
  cards.sort();
//  std::sort(cards.begin(), cards.end(), cCardCompare);
//  std::sort(cards.begin(), cards.end());
}


bool CDobbleCardSet::check()
{
  for(tcards::iterator it1=cards.begin(); it1 != cards.end(); it1++)
    {
      for(tcards::iterator it2=cards.begin(); it2 != cards.end(); it2++)
        {
          if(it1 != it2)
            {
              if(! it1->fits(*it2))
                {
                  cerr << "check failed with " << endl;
                  cerr << "a: " << (*it1) << endl;
                  cerr << "b: " << (*it2) << endl;
                  return false;
                }
            }
        }
    }
  return true;
}




// diese Abfolge führt dazu, dass strukturelle Fehler passieren.
// Z.Bsp. wird mit n=11, k=3 immer nur eine 5-Karten-Lösung gefunden!
// Das liegt daran, dass zwar die Reihenfolge der Symbole zufällig ist,
// jedoch immer das Prinzip gleich bleibt, dass das erste Symbol konstant bleibt, während alle anderen gesucht werden.
// es findet sich dann immer eine Lösung, bei der die Karten das erste Symbol gleich haben,
// die restlichen Symbole aber nicht mehr abgedeckt werden können
void CDobbleCardSet::checkNextSymbolRecursively(const CCard & _card)
{
  CCard card(_card); // TODO: performance: remove copying!

  // DEBUG:
//   cout << "checkNextSymbolRecursively n["<<n<<"] k["<<k<<"] card: ";
//   card.listSymbols(cout);
//   cout << endl;

  if(card.size() < n)
    {
      // create a randomly ordered symbol list
      typedef std::vector<int> tsymbolOrder;
      tsymbolOrder symbolOrder; // list of symbols to try as next symbol
      if(m_doShuffle) {
          for(int i = 1; i<=s; i++)
            {
              if(! card.contains(i)) symbolOrder.push_back(i);
            }
          std::random_shuffle(symbolOrder.begin(), symbolOrder.end() );
      }
      else
        {
          CCard::tsymbols::reverse_iterator it = card.symbols.rbegin();
          int start=(it != card.symbols.rend()) ? (*it) + 1 : 1;
            {
              for(int i = start; i<=s; i++)
                {
                  symbolOrder.push_back(i);
                }
            }
        }

      for( tsymbolOrder::iterator it = symbolOrder.begin() ; it != symbolOrder.end(); it++)
        {
          card.add(*it);
          checkNextSymbolRecursively(card);
          card.remove(*it);
        }
    }
  else
    {
      if(fits(card))
        {
          // DEBUG:
          // cout << "card " << card << " fits!" << endl;
          cards.push_back(card);
        }
      else
        {
          // DEBUG:
          // cout << "card " << card << " does not fit." << endl;
        }
    }
}

bool CDobbleCardSet::fits(const CCard & _card)
{
  for(tcards::iterator it=cards.begin(); it != cards.end(); it++)
    {
      if(! it->fits(_card)) return false;
    }
  return true;
}


// should set n=(k*1)^2+k and shuffle=false
void CDobbleCardSet::simpleConstruct()
{
  clear();

  // go through all combinations

  CCard card;
  checkNextSymbolRecursively(card);
}

void CDobbleCardSet::random_bad()
{
  clear();

  // go through all combinations

  CCard card;

  checkNextSymbolRecursively(card);

}


void CDobbleCardSet::addCardsRecursively(vector<CCard> & all_cards, CCard & card, int min)
{
  // DEBUG:
//   cout << "addCardsRecursively n["<<n<<"] k["<<k<<"] min["<<min<<"] card: ";
//   card.listSymbols(cout);
//   cout << endl;

  int max = 1 + s - (n - card.size() ); // leave place for follwing symbols
  if(card.size() < n)
    {
      for(int i = min; i <= max; i++)
        {
          card.add(i);
          addCardsRecursively(all_cards, card, i+1);
          card.remove(i);
        }
    }
  else
    {
      all_cards.push_back(card);
    }
}


// j = 1 creates n cards with symbol 1 and (n-1) completely new and different symbols
// example for n=7:
//
// card nr.  1:   1   2   3   4   5   6   7 |
// card nr.  2:   1   8   9  10  11  12  13 |
// card nr.  3:   1  14  15  16  17  18  19 |
// card nr.  4:   1  20  21  22  23  24  25 |
// card nr.  5:   1  26  27  28  29  30  31 |
// card nr.  6:   1  32  33  34  35  36  37 |
// card nr.  7:   1  38  39  40  41  42  43 |
//
// j = 2 creates n-1 cards with symbol 2 and (n-1) combinations of the former
//
// card nr.  8:   2   8  14  20  26  32  38 |    0   0   0   0   0   0
// card nr.  9:   2   9  15  21  27  33  39 |    1   1   1   1   1   1
// card nr. 10:   2  10  16  22  28  34  40 |    2   2   2   2   2   2
// card nr. 11:   2  11  17  23  29  35  41 |    3   3   3   3   3   3
// card nr. 12:   2  12  18  24  30  36  42 |    4   4   4   4   4   4
// card nr. 13:   2  13  19  25  31  37  43 |    5   5   5   5   5   5
//
// j=3 and higher is not yet correct!
void CDobbleCardSet::construct_part(int j)
{
  if(j<1) return;
  if(j==1)
    {
      for(int i = 1;i <= n;i++){
          CCard card;
          card.add(1);
          for(int j = 2;j <= n;j++){
              card.add(j + (i - 1) * (n - 1));
          }
          cards.push_back(card);
      }
      return;
    }

    for(int i = 0;i < (n - 1);i++){
        CCard card;
        card.add(j);
        for(int m = 0;m < n - 1;m++){
            // V1 alt
            // gibt doppelte ab k=5:
            // card.add( 2 + m*(k-1) + ((i-2) + (j-2)*(m-1)) % (k-1) );
            // V2 alt
            //              int perm  = (j==2) ? 0:1 ; // 2 wird nicht permutiert/derangiert
            //              int shift = j-2;
            //              card.add( 2 + m*(k-1) + ( ( i + perm*(m+shift)) % (k-1) ) );
            // V3 get matching symbol recursively...
            if(j == 2 || m == 0)
                card.add(2 + (m + 1) * (n - 1) + i);

            else{
                int D = m == 0 ? 0 : 1 + ((m - 1) + (j - 3)) % (n - 2);
                int shift = (i + D) % (n - 1);
                int val = 2 + (m + 1) * (n - 1) + shift;
                cout << "j=" << j << " i=" << i << " m=" << m << " D=" << D << " shift=" << shift << " val=" << val << endl;
                card.add(val);
            }
            // card.add(getMatchingSymbol(j,i,m));
        }

        cards.push_back(card);
    }

}

// die Lösung ist nicht einzigartig:
//
// durch Vertauchen der Zahlen "umbenennen" sollte man alle anderen Möglichkeiten erhalten!?
//
// Vermutlich nicht: die "Permutation ohne Fixpunkte" bzw. das "Derangement"
// für die Horizontalen gibt ab k=5
// wohl mehr Varianten
void CDobbleCardSet::construct(int _k)
{
  n=_k;
  s=n*(n-1)+1; // == k + (k-1)*(k-1)
  clear();

  // Bsp. k=4:
  // 1  1  1  1
  // 2  5  8 11
  // 3  6  9 12
  // 4  7 10 13

  // create all with a 1 (die Senkrechten) --> k Stk.
  construct_part(1);

  // create all with a 1 (die Senkrechten) --> k Stk.
  construct_part(2);

  // create all with a 2..k (die Waagerechten) --> (k-1)*(k-1) Stk.
  for(int j=2; j <= n; j++ )
    {
      construct_part(j);
    }
}

int CDobbleCardSet::getMatchingSymbol(int j, int i, int m)
{
  // j==2 festlegen
  if(j==2) return(j + m*(n-1) + i );

  //if(m==1) return(getMatchingSymbol(2,i,m));

  return(getMatchingSymbol(j-1, (i+(m-1)) % (n-1) , m));

//  if(j <= m+1)
//  return(getMatchingSymbol(j-1, (i+(m-1)) % (k-1) , m));
//
//  // m==1 soll immer mit den j=2 matchen
//  // m==2 soll  immer mit den j=3 matchen
//  // etc.
//  if(j>m+1) return(getMatchingSymbol(m+1, (i+(m-1)) % (k-1) , m));
}

void CDobbleCardSet::construct_variant3(int _n)
{
  n=_n;
  s=n+(n-1)*(n-1);
  for(int s_i = 1; s_i<=s; s_i++) // Symbole i durchgehen
    {
      for(int j=0;j<n;j++) // jedes symbol n mal verteilen
        {
          bool added = false;
          for(tcards::iterator it=cards.begin(); it != cards.end() && (! added) ; it++)
            {
              if(it->contains(s_i))
                {
                  // skip to next card
                }
              else if (it->size() >= n)
                {
                  assert(it->size() == n);
                  // skip to next card
                }
              else
                {
                  CCard card(*it);
                  card.add(s_i);

                  if( ! card.conflicts(cards.begin(), it) )
                    {
                      // put symbol on this card and continue to next symbol
                      it->add(s_i);
                      added = true;
                    }
                }
            }
          if(!added)
            {
              CCard card;
              card.add(s_i);
              cards.push_back(card);
            }
        }
    }
}

long long int fak(long long int n)
{
  long long int erg = n<=1 ? 1 : n*fak(n-1);
  cout << "fak(" << n << ")=" << erg << endl;
  return erg;
}

NTL::ZZ fak2(NTL::ZZ n)
{
  NTL::ZZ erg;
  erg = 1;
  if (n > 1) erg = n*fak2(n-1);
  cout << "fak2(" << n << ")=" << erg << endl;
  return erg;
}

long long int m_aus_n(int m, int n)
{
  static const int NULLVAL = -1;
  static vector<vector<long long int> > cache;
  if(cache.size() <= m) cache.resize(m+1,vector<long long int>(n+1, NULLVAL));
  vector<long long int> & slice = cache[m];
  if(slice.size() <= n) slice.resize(n+1, NULLVAL);

  long long int & cacheVal = slice[n];

  if(cacheVal != NULLVAL) return cacheVal;

  double erg=1;
  NTL::ZZ erg2;
  erg2 = 1;
  for(int i=0;i<m;i++)
    {
      erg = erg*(n-i)/(1+i);
      erg2 = erg2*(n-i)/(1+i);
    }
  if(erg != erg2)
    {

      cout << "ERROR: m_aus_n(" << m <<"," << n << ")=" << erg << "/" << erg2 << endl;
      exit (-1);
    }

  cacheVal = erg;
  //DEBUG:
//  int debug=true;
//  if(debug)
//    {
//      long long int check = fak(n)/ ( fak(n-m) * fak(m) );
//      cout << "check=" << check <<" m_aus_n(" << m <<"," << n << ")=" << erg << endl;
//    }

  return erg;
/*
static map<int,int> cache;
  // recursion end
  if(m==1)
    {
      return n;
    }

  // cache
  int idx=(m+n)*(m+n+1)/2+n;
  map<int,int>::iterator found=cache.find(idx);
  if(found != cache.end() )
    {
      return *it;
    }

  // recurse
  return m_aus_n(m-1,n)
*/
}

// s! / (s-n)! / n!
// = fakt s* ...(s+1-n) / (1*...n)
int CDobbleCardSet::getMaxNumOfDifferentCard()
{
    int erg = m_aus_n(n, s);
    return erg;
  /*
  long n=1, z=1;
  for(int i=0; i<n; i++)
    {
      z = z * s;
      n = n * (i+1);
    }
  return(z/n);
  */
}

// _n new symbols will be added to the given card
// the symbols will be from the range i to _s
// the function recurses.
//
// the symbols are predefined by the given "rest" value and
// are guaranteed to give different symbol combinations for different rest values.
// The each value from 1 to the maximum of combinations will be allowed for rest
//
// s. makeCardByNr for details
void addSymbolsByNr(CCard & card, int rest, int i, int _s, int _n)
{
// DEBUG:
  //  cout << "addSymbolsByNr rest=" << rest
//       << " i=" << i
//       << " _s=" << _s
//       << " _n=" << _n
//       << endl;
  while(rest > m_aus_n(_n-1, _s-i) )
    {
      rest -= m_aus_n(_n-1,_s-i);
      i++;
    }
  card.add(i);
  if(_n>1) addSymbolsByNr(card, rest, i+1, _s, _n-1);
}

void CDobbleCardSet::makeCardByNr(CCard & card, int nr /* 1..maxNum */)
{
  // abzaehlen....
  // ist das erste (kleinste) Syxmbol die 1, so gibt es (s-1) über (n-1) Varianten für den Rest
  // --> ist also 0<=Zahl<(s-1)über(n-1), dann nehmen wir die 1,
  // sonst überspringen wir (s-1)über(n-1) und machen mit Symbol 2 weiter...
  // Optimierung:
  // - die über-Zahlen sollen gecacht werden!
  // - eine Map pro Level mit den über-zahlen-Summen ermöglicht effizientes Finden des zu wählenden Symbols
  // die Funktionen sollen von Anfang an auf die Map-Abfragen zugeschnitten sein!
  // wir nennen die Datenstruktur "numberedCards"
  int rest = nr;
  int i = 1;
  card.symbols.clear();
  addSymbolsByNr(card, nr, 1, s, n);
}

// Bsp.: Karten mit 3 von 5 Symbolen
// 1  --> 1 2 3
// 2  --> 1 2 4
// 3  --> 1 2 5
// 4  --> 1 3 4
// 5  --> 1 3 5
// 6  --> 1 4 5
// 7  --> 2 3 4
// 8  --> 2 3 5
// 9  --> 2 4 5
// 10 --> 3 4 5

int CDobbleCardSet::getNumberOfCard(const CCard & card)
{
  int erg=1;
  int i = 1;
  int _n = card.symbols.size();
  int _s = s;
  for(set<int>::iterator it=card.symbols.begin(); it!=card.symbols.end(); it++)
    {
      while(*it > i)
        {
          erg += m_aus_n(_n-1,_s-i);
          i++;
        }
      _n--;
      i++;
    }
  return erg;
}

void CDobbleCardSet::random()
{
//  randomByshuffledCards();
  randomByshuffledCardNumbers();
}

int CDobbleCardSet::createVectorWithAllNumbers(vector<int> & all_card_numbers)
{
    int maxNum = getMaxNumOfDifferentCard();
    all_card_numbers.reserve(maxNum);
    cout << "filling cardNumbers array with maxNum=" << maxNum << " values..." << endl;
    for(int i = 1;i <= maxNum;i++){
        all_card_numbers.push_back(i);
    }
    return maxNum;
}

void CDobbleCardSet::randomByshuffledCardNumbers()
{
  // go randomly through all combinations

  // 1st: create all cards
  vector<int> all_card_numbers;
  int maxNum = createVectorWithAllNumbers(all_card_numbers);

  // DEBUG:
//  cout << "all_cards:" << endl;
//  ::listCards(cout, all_cards.begin(), all_cards.end());

  // 2nd: shuffle them
  cout << "shuffling maxNum="<<maxNum<<" values in cardNumbers array with size="<<all_card_numbers.size()<<" ..." << endl;
  std::random_shuffle(all_card_numbers.begin(), all_card_numbers.end() );


  CCard card;
  int i=1, printedPercentStep=10, lastPrintedPercent=-printedPercentStep;
  time_t start=time(NULL);
  time_t last=start;
  int time_step=60;

  for(vector<int>::iterator it=all_card_numbers.begin(); it != all_card_numbers.end(); it++, i++)
    {
      int actualPercent=((long long int)i*100/maxNum);
      time_t actualTime=time(NULL);
      while(actualPercent >= lastPrintedPercent+printedPercentStep)
        {
          lastPrintedPercent += printedPercentStep;
          last=actualTime;
          cout << "checking"
              << " card " << i << ""
              << " after " << (last-start) << "s:"
              << " " << lastPrintedPercent << "% ..." << endl;
        }
      if(actualTime-last > time_step)
        {
          last=actualTime;
          cout << "checking"
              << " card " << i << ""
              << " after " << (last-start) << "s:"
              << " " << actualPercent << "% ..." << endl;
        }
      makeCardByNr(card, *it);
      if(fits(card))
        {
          // DEBUG:
          // cout << "card " << card << " fits!" << endl;
          cards.push_back(card);
        }
      else
        {
          // DEBUG:
//          int debug = 1;
//          if (debug) cout << "card " << card << " does not fit!" << endl;
        }
    }
}

void CDobbleCardSet::randomByshuffledCards()
{
  // clear();

  // go randomly through all combinations

  // 1st: create all cards
  vector<CCard> all_cards;
  CCard card;

  addCardsRecursively(all_cards, card, 1);

  // DEBUG:
//  cout << "all_cards:" << endl;
//  ::listCards(cout, all_cards.begin(), all_cards.end());

  // 2nd: shuffle them
  std::random_shuffle(all_cards.begin(), all_cards.end() );

  for(vector<CCard>::iterator it=all_cards.begin(); it != all_cards.end(); it++)
    {
      if(fits(*it))
        {
          // DEBUG:
          // cout << "card " << card << " fits!" << endl;
          cards.push_back(*it);
        }
    }
}

int CDobbleCardSet::getNumUsedSymbols()
{
    set<int> allSymbols;
    for(tcards::iterator it = cards.begin();it != cards.end();it++){
        allSymbols.insert(it->symbols.begin(), it->symbols.end());
    }
    int usedSymbolCount = allSymbols.size();
    return usedSymbolCount;
}

void CDobbleCardSet::printSummary(std::ostream &os)
{
  int usedSymbolCount = getNumUsedSymbols();
  bool checkOk = check();
  os << "summary for n["<<s<<"] k["<<n<<"] "
     << " used symbols   : " << usedSymbolCount << "/" << s
     << " number of cards: " << cards.size()
     << " check: " << checkOk
     << endl;

}

void CDobbleCardSet::listCards(std::ostream &os, int n)
{
  int i=1;
  for(tcards::iterator it=cards.begin(); it != cards.end(); it++, i++)
    {
      os << "card nr.";
      os.flags ( ios::right  );
      os.width (3);
      os.fill(' ');
      os << i << ": ";
      it->listSymbols(os);
      if(n>0)
        {
          os << " | ";
          it->listSymbolOffsets(os, n);
        }
      os << std::endl;
    }

}

