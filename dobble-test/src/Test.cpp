#include "cute.h"
#include "ide_listener.h"
#include "cute_runner.h"

#include <CDobbleCardSet.h>

#include <vector>
#include <stdio.h>
#include <iostream>

using namespace std;

class CCardsCounter
{
public:
  CDobbleCardSet & set;
  vector<int> & cards_usage_count;
  CCardsCounter(CDobbleCardSet & _set, vector<int> & _cards_usage_count)
  : set(_set)
  , cards_usage_count(_cards_usage_count)
  {};


  void handle(const CCard & card)
  {
    int nr = set.getNumberOfCard(card);
    count(nr);
  }

  void count(int i)
  {
    // cout << "counting card nr." << i << endl;
    (cards_usage_count.at(i-1))++;
  }
};

void testCompletenessOfSetNumbers()
{
  ASSERT_EQUAL(3, CDobbleCardSet(2).s);
  ASSERT_EQUAL(3, CDobbleCardSet(2).getMaxNumOfDifferentCard());

  ASSERT_EQUAL(7, CDobbleCardSet(3).s);
  ASSERT_EQUAL(35, CDobbleCardSet(3).getMaxNumOfDifferentCard());
  // run from 2 to 7
  // calculate sets of all cards
  for(int n=2; n<=7; n++)
    {
      CDobbleCardSet set(n);
      int maxNum = set.getMaxNumOfDifferentCard();

      cout << "n=" << n << " s=" << set.s << " maxNum=" << maxNum <<endl;

      vector<int> cards_usage_count(maxNum, 0);

      // run through all cards

      CCardsCounter cardsCounter(set, cards_usage_count);

      set.forEachCard(cardsCounter);

      // check whether all were touched once...
      for(int i = 1; i <= maxNum; i++){
          int count=cards_usage_count.at(i-1);
          if(count != 1)
            {
              char buf[80];
              sprintf(buf, "card number %d has wrong count", i);
              ASSERT_EQUALM(buf, 1, count);
            }
      }
    }

}

void testBijectivenessOfCardNumbers()
{
  for(int n=2; n<=3; n++)
    {
      CDobbleCardSet set(n);
      int maxNum = set.getMaxNumOfDifferentCard();
      for(int nr=1; nr<=maxNum; nr++)
        {
          CCard card;
          set.makeCardByNr(card, nr);
          int ref_nr = set.getNumberOfCard(card);
          ASSERT_EQUALM("Number not equal", nr, ref_nr);
        }
    }
}




void testFakultaet() {
  ASSERTM("Fakultät fehlgeschlagen", fak(0) == 1);
  ASSERTM("Fakultät fehlgeschlagen", fak(1) == 1);
  ASSERTM("Fakultät fehlgeschlagen", fak(2) == 2);
  ASSERTM("Fakultät fehlgeschlagen", fak(3) == 6);
  ASSERTM("Fakultät fehlgeschlagen", fak(4) == 24);
}


void runSuite(){
	cute::suite s;
	//TODO add your test here
	s.push_back(CUTE(testFakultaet));
        s.push_back(CUTE(testBijectivenessOfCardNumbers));
	s.push_back(CUTE(testCompletenessOfSetNumbers));
	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "The Suite");
}

int main(){
    runSuite();
    return 0;
}



