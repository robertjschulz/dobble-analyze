/*
 ============================================================================
 Name        : dobbler.cpp
 Author      : Robert Schulz
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C++,
 ============================================================================
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>     /* for printf */
#include <stdlib.h>    /* for exit */

#undef __need_getopt
#include <getopt.h>

#include "CDobbleCardSet.h"

using namespace std;


void analyze( int n , int k)
{
  cout << "analyze for n["<<n<<"] k["<<k<<"] " << endl;
  CDobbleCardSet set(n, k);
  set.doShuffle(true);
  set.clear();
  set.random();
  cout << "analyze for n["<<n<<"] k["<<k<<"] result:" << endl;
  set.printSummary(cout);
  set.listCards(cout);
}



void tryAll(int n, int prework)
{
  CDobbleCardSet set(n);
  for(int pw=1; pw <= prework; pw++)
    {
      set.construct_part(pw);
    }
  set.checkAllCombinations();
}

void tryRandom(int n, int prework)
{
  srand(time(NULL));
  bool solved = false;
  unsigned long int maxFoundCards=0;
  do {

      CDobbleCardSet set(n);
      set.doShuffle(true);

      for(int pw=1; pw <= prework; pw++)
        {
          set.construct_part(pw);
        }

      set.random();
      set.printSummary(cout);
      // DEBUG:
      set.sort();
      set.listCards(cout, n);

      unsigned long int foundCards = set.cards.size();
      if(foundCards > maxFoundCards)
        {
            set.sort();
            maxFoundCards = foundCards;
            stringstream sb;
            int s = set.s;
            sb << "dobble_solution_n_" << n
                << "_symbols_" << set.getNumUsedSymbols() << "_von_" << s
                << "_cards_" << foundCards << "_von_" << s
                << ".txt";
            string filename = sb.str();
            fstream f(filename.c_str(), ios_base::out);
            set.printSummary(f);
            set.listCards(f, n);
            f.close();
        }

      if(set.check() && set.cards.size() == set.s)
        {
          set.sort();
          set.listCards(cout, n);
          solved = true;
        }
  } while (! solved);
}

void findRulesByRandom()
{
    srand(time(NULL));
    for(int k = 2;k <= 8;k++){
        for(int n = k;n < k * k;n++){
            CDobbleCardSet set(n, k);
            set.doShuffle(true);
            set.random();
            set.printSummary(cout);
        }
    }

}

void testConstruct(int n)
{
  CDobbleCardSet set(n);
  set.construct(n);
  set.listCards(cout);
  set.printSummary(cout);
}

void testConstruct3()
{
    // findRulesByRandom();
    int n = 1;
//    for(int k = 2;k <= 9;k++){
    for(int k = 5;k <= 5;k++){
        CDobbleCardSet set(n, k);
        set.construct_variant3(k);
        set.listCards(cout);
        set.printSummary(cout);
    }
}

void testSimpleConstruct()
{
    // findRulesByRandom();
    for(int k = 2;k <= 9; k++){
        int n = k + (k-1)*(k-1) ;
        CDobbleCardSet set(n, k);
        set.doShuffle(false);
        set.simpleConstruct();
        set.listCards(cout);
        set.printSummary(cout);
    }
}

int main(int argc, char *argv[])
{
  int c;
  int digit_optind = 0;

  int n=5;
  int prework=3;

  while (1)
    {
      int this_option_optind = optind ? optind : 1;
      int option_index = 0;
      static struct option long_options[] = {
          {"test-construct", 1, 0, 'c'},
          {"find-random", 0, 0, 'r'},
          {0, 0, 0, 0}
      };

      c = getopt_long(argc, argv, "crn:p:",
          long_options, &option_index);
      if (c == -1)
        break;

      switch (c)
      {
      case 0:
        printf("option %s", long_options[option_index].name);
        if (optarg)
          printf(" with arg %s", optarg);
        printf("\n");
        break;

      case 'c':
        printf("option a\n");
        break;

      case 'r':
        printf("option b\n");
        break;

      case 'n':
        printf("option n with value '%s'\n", optarg);
        n=atoi(optarg);
        break;

      case 'p':
        printf("option p with value '%s'\n", optarg);
        prework=atoi(optarg);
        break;

      default:
        printf("?? getopt returned character code 0%o ??\n", c);
        break;
      }
    }

  if (optind < argc) {
      printf("non-option ARGV-elements: ");
      while (optind < argc)
        {
          string cmd = argv[optind++];
          printf("%s ", cmd.c_str() );
          printf("\n");
          if(cmd=="test-construct")
            {
              testConstruct(n);
              exit(EXIT_SUCCESS);
            }
          else if(cmd=="try-random")
            {
              tryRandom(n, prework);
              exit(EXIT_SUCCESS);
            }
          else if(cmd=="try-all")
            {
              tryAll(n, prework);
              exit(EXIT_SUCCESS);
            }
          else if(cmd=="test")
            {
              CDobbleCardSet set(n);
              set.clear();
              int max=set.getMaxNumOfDifferentCard();
              cout << "n=" << n << " symbols=" << set.s << " max=" << max << endl;
              for(int i=1; i<=max; i++)
                {
                  CCard card;
                  set.makeCardByNr(card, i);
                  set.cards.push_back(card);
                }
              set.listCards(cout);
              exit(EXIT_SUCCESS);
            }
        }
  }

  cout << "no command given!" << endl;
  exit(EXIT_FAILURE);

  // findRulesByRandom();
//  testSimpleConstruct();
  // testConstruct3();

//  analyze(3,2);

//  for( int i = 1; i<=5; i++)
//    {
//      analyze(11,3);
//    }


  //  analyze(6,3);
//  analyze(7,3);
//  analyze(8,3);
//  analyze(9,3);
//  analyze(10,3);
  return 0;
}
