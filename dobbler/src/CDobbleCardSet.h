/*
 * CDobbleCardSet.h
 *
 *  Created on: 02.01.2012
 *      Author: rosa
 */

#ifndef CDobbleCardSet_H_
#define CDobbleCardSet_H_

#include <list>
#include <vector>

#include "CCard.h"

#include <gsl/gsl_combination.h>

long long int fak(long long int n);


class CDobbleCardSet {

public:
  int s; /// NUmber of different symbols in the card stack
  int n; /// Number of symbols on each card

  bool m_doShuffle;

  typedef std::list<CCard> tcards ;
  tcards cards;

  int m_best_size; /// remember best saved solution;
  long long int m_max_iteration_number;
  long long int m_current_iteration_number;

public:
  CDobbleCardSet(int s, int n);
  CDobbleCardSet(int n);
  virtual ~CDobbleCardSet();

  void clear();
  void sort();

  int getMaxNumOfDifferentCard();
  void makeCardByNr(CCard & card, int nr);
  int getNumberOfCard(const CCard & card);

  // gsl_ ...
  void checkAllCombinations();
  void checkLeftCombinations(gsl_combination_struct *c, bool first=false);

  // own ...
    // card set constructing function:
    void random_bad();
    void random();
    void randomByshuffledCards();
    void randomByshuffledCardNumbers();
    void tryMatrixSolver();

    // set cards that are obviously / oBdA part of the solution
    void presetCards();


    int getMatchingSymbol(int j, int i, int m); // helper for construct to recursively get matching symbols
    void construct_part(int j);
    void construct(int _n);
    void construct_variant3(int _n);
    void simpleConstruct();
    void printSummary(std::ostream & os);
    void listCards(std::ostream & os, int n=0);
    void saveSummary();


    template<class H>
    void forEachCardRecurse(H & cardsCounter, CCard & card, int min)
    {
      int max = 1 + s - (n - card.size() ); // leave place for follwing symbols
      if(card.size() < n)
        {
          for(int i = min; i <= max; i++)
            {
              card.add(i);
              forEachCardRecurse(cardsCounter, card, i+1);
              card.remove(i);
            }
        }
      else
        {
          cardsCounter.handle(card);
        }
    }

    template<class H>
    void forEachCard(H & cardsCounter)
    {
      CCard card;
      forEachCardRecurse(cardsCounter, card, 1);
    }


    void checkNextSymbolRecursively(const CCard & _card);
    void addCardsRecursively(std::vector<CCard> & all_cards, CCard & card, int min);
    bool fits(const CCard & _card);
    bool fits(gsl_combination_struct *candidate);

    bool check();
    void doShuffle(bool _doShuffle)
    {
        m_doShuffle = _doShuffle;
    }

    ;
    int getNumUsedSymbols();
private:
    int createVectorWithAllNumbers(std::vector<int> & all_card_numbers);


};

#endif /* CDobbleCardSet_H_ */
