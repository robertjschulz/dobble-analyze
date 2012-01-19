/*
 * CPermutation.cpp
 *
 *  Created on: 17.01.2012
 *      Author: rosa
 */

#include "CPermutation.h"

CPermutation::CPermutation(int n)
{
  s.size=n;
  gsl_permutation_init(&s);
}

CPermutation::~CPermutation()
{
  free(s.data);
}

