//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2004-2009 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

/* $Source: /cvsroot/psrchive/psrchive/More/MEAL/MEAL/Phase.h,v $
   $Revision: 1.8 $
   $Date: 2009/06/10 10:53:45 $
   $Author: straten $ */

#ifndef __MEAL_Phase_H
#define __MEAL_Phase_H

#include "MEAL/OneParameter.h"
#include <complex>

namespace MEAL
{
  //! A complex exponential parameterized by phase
  template<typename Parent>
  class Phase : public Parent 
  {

  public:

    //! Default constructor
    Phase () : parameter (this)
    { parameter.set_name ("phase"); }

    // ///////////////////////////////////////////////////////////////////
    //
    // Function implementation
    //
    // ///////////////////////////////////////////////////////////////////

    //! Return the name of the class
    std::string get_name () const
    { return "Phase<" + std::string(Parent::Name) + ">"; }

  protected:

    typedef typename Parent::Result Return;

    //! Calculate the result and its gradient
    void calculate (Return& result, std::vector<Return>* gradient);
   
    OneParameter parameter;
  };

}

//! Calculate the Jones matrix and its gradient, as parameterized by gain
template<typename Parent>
void MEAL::Phase<Parent>::calculate (Return& result, std::vector<Return>* grad)
{
  double phase = this->get_param(0);

  if (MEAL::Function::verbose)
    std::cerr << "MEAL::Phase<Parent>::calculate phase=" << phase << std::endl;

  double cos_phase = cos(phase);
  double sin_phase = sin(phase);

  result = std::complex<double>(cos_phase, sin_phase);

  if (grad)
  {
    (*grad)[0] = std::complex<double>(-sin_phase, cos_phase);
    
    if (MEAL::Function::verbose)
      std::cerr << "MEAL::Phase<Parent>::calculate gradient" << std::endl
	   << "   " << (*grad)[0] << std::endl;
  } 
}

#endif
