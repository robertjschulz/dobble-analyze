/*
 * CPermutation.h
 *
 *  Created on: 17.01.2012
 *      Author: rosa
 */

#ifndef CPERMUTATION_H_
#define CPERMUTATION_H_

#include <gsl/gsl_permutation.h>

class CPermutation
{
public:
  gsl_permutation_struct s;
  CPermutation(int n);
  virtual
  ~CPermutation();
};

#endif /* CPERMUTATION_H_ */
