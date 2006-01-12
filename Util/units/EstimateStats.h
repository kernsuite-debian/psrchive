//-*-C++-*-

/* $Source: /cvsroot/psrchive/psrchive/Util/units/EstimateStats.h,v $
   $Revision: 1.1 $
   $Date: 2006/01/12 22:46:19 $
   $Author: straten $ */

#ifndef __EstimateStats_h
#define __EstimateStats_h

#include "Estimate.h"

template <typename T, typename U>
double chisq (const Estimate<T,U>& a, const Estimate<T,U>& b) 
{
  T diff = a.get_value() - b.get_value();
  T var  = a.get_variance() + b.get_variance();

  if (var == 0.0)
    return 0;
  else
    return diff * diff / var;
}

template <typename T, typename U>
double chisq (const MeanEstimate<T,U>& a, const MeanEstimate<T,U>& b) 
{
  return chisq (a.get_Estimate(), b.get_Estimate());
}

template <typename T, typename U>
double chisq (const MeanRadian<T,U>& a, const MeanRadian<T,U>& b)
{
  return 0.5 * ( chisq(a.get_cos(), b.get_cos()) + 
		 chisq(a.get_sin(), b.get_sin()) );
}

#endif
