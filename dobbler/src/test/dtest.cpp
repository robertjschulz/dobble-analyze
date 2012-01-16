
#include "CMatrixSolver.h"
#include <iostream>
#include <sys/time.h>
#include <string>
#include <sstream>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

class mytime
{
public:
  mytime()
  {
    gettimeofday(&tv, &tz);
  }
  struct timeval tv;
  struct timezone tz;

  long long int getLongLongUS() const
  {
    return tv.tv_sec*1000000+tv.tv_usec;
  }

  string getDurationAsString(const mytime &start)
  {
    stringstream ss;
    long long int r = ( getLongLongUS() - start.getLongLongUS());
    int us = r % 1000;
    r = r / 1000;
    int ms = r % 1000;
    r = r / 1000;
    int s = r % 60;
    r = r / 60;
    int m = r % 60;
    r = r / 60;
    int h = r;


    ss.flags ( ios::right  );
    ss.width(2);
    ss.fill('0');
    ss << h << ":";

    ss.flags ( ios::right  );
    ss.width(2);
    ss.fill('0');
    ss << m << ":";

    ss.flags ( ios::right  );
    ss.width(2);
    ss.fill('0');
    ss << s << ".";

    ss.flags ( ios::right  );
    ss.width(3);
    ss.fill('0');
    ss << ms << ".";

    ss.flags ( ios::right  );
    ss.width(3);
    ss.fill('0');
    ss << us;

    return ss.str();
  }

};

int main(int argc, char **argv)
{
  CMatrixSolver m;
  int n=5;
  bool nfnd=false;
  int opt;
  while ((opt = getopt(argc, argv, "n:")) != -1) {
      switch (opt) {
      case 'n':
          n = atoi(optarg);
          nfnd = true;
          break;
      default: /* '?' */
          cerr << "Usage: " << argv[0] << " [-n <n>] \n";
          exit(EXIT_FAILURE);
          break;
      }
  }

  cout << endl;
  cout << "solving with n=" << n << "..." << endl;

  mytime start;
  // time_t started = time(NULL);

  //      m.m_debug=true;
  bool erg = m.solve(n);
  if(erg)
    {
      m.print(cout);
    }
  else
    {
      cout << "solving failed!" << endl;
    }
  cout << "solving with n=" << n << " took ";
  mytime finished;
  // time_t finished = time(NULL);

  string duration = finished.getDurationAsString(start);

  cout << duration;

  cout << endl;
}
