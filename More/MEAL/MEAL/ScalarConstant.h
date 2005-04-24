//-*-C++-*-

/* $Source: /cvsroot/psrchive/psrchive/More/MEAL/MEAL/ScalarConstant.h,v $
   $Revision: 1.5 $
   $Date: 2005/04/24 01:28:07 $
   $Author: straten $ */

#ifndef __ScalarConstant_H
#define __ScalarConstant_H

#include "MEAL/Scalar.h"

namespace MEAL {

  //! Represents a constant scalar value with no parameters or arguments
  class ScalarConstant : public Scalar {

  public:

    //! Default constructor
    ScalarConstant (double value);

    //! Assignment operator
    const ScalarConstant& operator = (const ScalarConstant& scalar);

    // ///////////////////////////////////////////////////////////////////
    //
    // Function implementation
    //
    // ///////////////////////////////////////////////////////////////////

    //! Return the name of the class
    std::string get_name () const;

    // ///////////////////////////////////////////////////////////////////
    //
    // Scalar implementation
    //
    // ///////////////////////////////////////////////////////////////////

    //! Return the value
    void calculate (double& result, std::vector<double>* gradient);
    
  protected:

    //! The value returned by evaluate
    double value;

  };

}

#endif
