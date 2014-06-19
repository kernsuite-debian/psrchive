//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2009-2011 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

// psrchive/More/Polarimetry/Pulsar/FourthMomentStats.h

#ifndef __Pulsar_FourthMomentStats_h
#define __Pulsar_FourthMomentStats_h

#include "Pulsar/PolnProfileStats.h"
#include "Matrix.h"

namespace Pulsar {

  class StokesCovariance;

  //! Computes fourth-order moment pulse profile statistics
  class FourthMomentStats : public PolnProfileStats {
    
  public:

    //! Default constructor
    FourthMomentStats (const PolnProfile* profile = 0);

    //! Destructor
    ~FourthMomentStats();

    void set_profile (const PolnProfile* _profile);

    //! Get the estimated covariance matrix for the specified phase bin
    Matrix< 4,4,Estimate<double> > get_covariance (unsigned ibin) const;

    const StokesCovariance* get_covariance () const { return covariance; }

    //! Get the eigen polarization profiles
    void eigen (PolnProfile* = 0, PolnProfile* = 0, PolnProfile* = 0);

    //! Get the eigen values of the polarization vector space
    const Profile* get_eigen_value (unsigned k) { return eigen_value.at(k); }

    //! Get the regression coefficients for each polarization vector
    const Profile* get_regression_coefficient (unsigned k)
    { return regression_coefficient.at(k); }

    //! Get the mode-separated profiles
    void separate (PolnProfile& modeA, PolnProfile& modeB);

    //! Returns the variance of the baseline for the specified moment
    Estimate<double> get_moment_variance (unsigned) const;

  protected:

    Reference::To<const StokesCovariance> covariance;

    std::vector< Reference::To<Profile> > eigen_value;
    std::vector< Reference::To<Profile> > regression_coefficient;
    
    //! The variance of the baseline for each moment
    mutable std::vector< Estimate<double> > moment_variance;



  };

}


#endif



