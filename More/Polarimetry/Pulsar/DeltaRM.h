//-*-C++-*-

/* $Source: /cvsroot/psrchive/psrchive/More/Polarimetry/Pulsar/DeltaRM.h,v $
   $Revision: 1.1 $
   $Date: 2005/12/08 11:48:37 $
   $Author: straten $ */

#ifndef __Pulsar_DeltaRM_h
#define __Pulsar_DeltaRM_h

#include "Reference.h"
#include "Estimate.h"

namespace Pulsar {

  class Archive;

  //! This class refines an RM estimate using two halves of the band
  class DeltaRM  {

  public:

    //! Default constructor
    DeltaRM ();

    //! Destructor
    ~DeltaRM ();

    //! Set the rotation measure (initial guess)
    void set_rotation_measure (const Estimate<double>& rm)
    { rotation_measure = rm; }

    //! Get the rotation measure 
    Estimate<double> get_rotation_measure () const
    { return rotation_measure; }

    //! Set the archive from which to derive the refined rotation measure
    void set_data (Archive*);

    //! Refine the rotation measure estimate
    void refine ();

  protected:

    //! The rotation measure
    Estimate<double> rotation_measure;

    //! The archive from which the rotation measure will be refined
    Reference::To<Archive> data;

  };

}

#endif
