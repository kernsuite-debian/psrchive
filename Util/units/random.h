//-*-C++-*-

/* $Source: /cvsroot/psrchive/psrchive/Util/units/random.h,v $
   $Revision: 1.1 $
   $Date: 2003/02/25 09:48:20 $
   $Author: straten $ */

#ifndef __random_H
#define __random_H

#include <complex>
#include <stdlib.h>

#include "psr_cpp.h"

template <class T, class U>
void random_value (T& value, U scale)
{
  value = ( double(rand()) - 0.5*double(RAND_MAX) ) * 2.0 * scale / RAND_MAX;
}

template <class T, class U>
void random_value (complex<T>& value, U scale)
{
  T real=0, imag=0;

  random_value (real, scale);
  random_value (imag, scale);

  value = complex<T> (real, imag);
}

template <class T, class U>
void random_vector (T& array, U scale)
{
  for (unsigned i=0; i<array.size(); i++)
    random_value (array[i], scale);
}

#endif
