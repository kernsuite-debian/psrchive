//-*-C++-*-

/* $Source: /cvsroot/psrchive/psrchive/More/Polarimetry/Pulsar/SingleAxisCalibrator.h,v $
   $Revision: 1.3 $
   $Date: 2003/04/04 08:44:44 $
   $Author: straten $ */

#ifndef __SingleAxisCalibrator_H
#define __SingleAxisCalibrator_H

#include "PolnCalibrator.h"
#include "SingleAxisModel.h"

namespace Pulsar {

  //! Uses SingleAxisModel to represent the system response
  class SingleAxisCalibrator : public PolnCalibrator {
    
  public:

    friend class SingleAxisCalibratorPlotter;

    //! Construct from an vector of PolnCal Pulsar::Archives
    SingleAxisCalibrator (const vector<Archive*>& a) : PolnCalibrator (a) {}

    //! Construct from an single PolnCal Pulsar::Archive
    SingleAxisCalibrator (const Archive* a) : PolnCalibrator (a) {}

    //! Destructor
    ~SingleAxisCalibrator ();

  protected:

    //! Return the system response as determined by the SingleAxisModel
    virtual Jones<double> solve (const vector<Estimate<double> >& hi,
				 const vector<Estimate<double> >& lo,
				 unsigned ichan);

    //! Resize the space used to store SingleAxisModel parameters
    virtual void resize_parameters (unsigned nchan);

    //! SingleAxisModel representation of the instrument
    vector<Calibration::SingleAxisModel> model;

  };

}

#endif
