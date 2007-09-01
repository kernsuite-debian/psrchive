//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2006 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

/* $Source: /cvsroot/psrchive/psrchive/More/General/Pulsar/ColdPlasma.h,v $
   $Revision: 1.7 $
   $Date: 2007/09/01 02:40:24 $
   $Author: straten $ */

#ifndef __Pulsar_ColdPlasma_h
#define __Pulsar_ColdPlasma_h

#include "Pulsar/Transformation.h"
#include "Pulsar/Integration.h"
#include "Pulsar/Archive.h"
#include "Pulsar/Profile.h"
#include "Physical.h"

namespace Pulsar {

  //! Corrects dielectric effects in cold plasma
  /*! Corrections are performed with respect to a reference frequency.
    By default, this is the centre frequency of the Integration;
    however, it is possible to correct with respect to an arbitrary
    frequency or wavelength (including zero wavelength = infinite
    frequency).

    The main point to this template base class is the standardization
    of dispersion and Faraday rotation corrections.  The data to be
    corrected may have already been corrected once before, with a
    different dispersion/rotation measure and/or refererence
    wavelength.  This class ensures that past corrections are treated
    properly, through use of a History class (an Integration::Extension).

  */
  template<class Corrector, class History>
  class ColdPlasma : public Transformation<Integration> {

  public:

    //! Default constructor
    ColdPlasma () { name = "ColdPlasma"; }

    //! Derived classes must return the measure to be passed to the Corrector
    virtual double correction_measure (const Integration*) = 0;

    //! Derived classes must define the identity
    virtual typename Corrector::Return get_identity () = 0;

    //! Derived classes must define how to apply the correction
    virtual void apply (Integration*, unsigned channel) = 0;

    //! Execute the correction for an entire Pulsar::Archive
    virtual void execute (Archive*);

    //! The default correction
    void transform (Integration*);

    //! Set up internal variables before execution
    /* \post reference_frequency = Integration::get_centre_frequency. */
    void setup (const Integration*);

    //! Correct internal variables before execution
    /* \post reference_frequency = Integration::get_centre_frequency. */
    void correct (const Integration*);

    //! Calls setup then correct
    virtual void set (const Integration* data)
    { setup (data); correct (data); }

    //! Set the frequency for which the correction will be computed
    virtual void set_Profile (const Profile* data)
    { corrector.set_frequency( data->get_centre_frequency () ); }

    //! Execute the correction
    /* \post All data will be corrected to the reference frequency */
    void execute1 (Integration*);

    //! Set the reference wavelength in metres
    void set_reference_wavelength (double metres);
    //! Get the reference wavelength
    double get_reference_wavelength () const;

    //! Set the reference frequency in MHz
    void set_reference_frequency (double MHz);
    //! Get the reference frequency in MHz
    double get_reference_frequency () const;

    //! Set the correction measure
    void set_measure (double measure);
    //! Get the correction measure
    double get_measure () const;

    //! Set the correction due to a change in reference wavelength
    void set_delta (const typename Corrector::Return& d) { delta = d; }
    //! Get the correction due to a change in reference wavelength
    typename Corrector::Return get_delta () const { return delta; }

  protected:

    friend class Integration;

    //! Execute the correction on the selected range
    void range (Integration*, unsigned ichan, unsigned jchan);

    //! The dielectric effect corrector
    Corrector corrector;

    //! The correction due to a change in reference wavelength
    typename Corrector::Return delta;

    //! The name to be used in verbose messages
    std::string name;

    //! The name of the correction measure
    std::string val;

  private:
    double backup_measure;
  };

}


template<class C, class H>
void Pulsar::ColdPlasma<C,H>::setup (const Integration* data)
{
  set_reference_frequency( data->get_centre_frequency() );
  set_measure( correction_measure(data) );
}
 
template<class C, class H>
void Pulsar::ColdPlasma<C,H>::transform (Integration* data) try
{
  setup (data);
  execute1 (data);
}
catch (Error& error) {
  throw error += "Pulsar::" + name + "::transform";
}

template<class C, class H>
void Pulsar::ColdPlasma<C,H>::execute (Archive* arch)
{
  for (unsigned i=0; i<arch->get_nsubint(); i++)
    execute1( arch->get_Integration(i) );
}

template<class C, class H>
void Pulsar::ColdPlasma<C,H>::set_reference_frequency (double MHz)
{
  corrector.set_reference_frequency (MHz);
}

template<class C, class H>
double Pulsar::ColdPlasma<C,H>::get_reference_frequency () const
{
  return corrector.get_reference_frequency ();
}

template<class C, class H>
void Pulsar::ColdPlasma<C,H>::set_reference_wavelength (double metres)
{
  corrector.set_reference_wavelength (metres);
}

template<class C, class H>
double Pulsar::ColdPlasma<C,H>::get_reference_wavelength () const
{
  return corrector.get_reference_wavelength ();
}

template<class C, class H>
void Pulsar::ColdPlasma<C,H>::set_measure (double measure)
{
  corrector.set_measure (measure);
}

template<class C, class H>
double Pulsar::ColdPlasma<C,H>::get_measure () const
{
  return corrector.get_measure ();
}

template<class C, class History>
void Pulsar::ColdPlasma<C,History>::correct (const Integration* data) try
{
  backup_measure = get_measure();
  const History* corrected = data->template get<History>();
 
  if ( corrected ) {

    double corrected_measure = corrected->get_measure();
    double lambda = corrected->get_reference_wavelength();

    if (corrected_measure == backup_measure &&
	lambda == get_reference_wavelength())
      {
	if (Integration::verbose)
	  std::cerr << "Pulsar::" + name + "::execute1 data are corrected."
	    " measure=" << corrected_measure << " lambda=" << lambda 
		    << std::endl;
	return;
      }

    // calculate the correction due to the new centre frequency, if any
    corrector.set_wavelength( lambda );
    delta = corrector.evaluate();

    // set the effective correction measure to the difference
    set_measure( backup_measure - corrected_measure );

  }
  else
    delta = get_identity();
}
catch (Error& error) {
  throw error += "Pulsar::"+name+"::correct";
}


template<class C, class History>
void Pulsar::ColdPlasma<C,History>::execute1 (Integration* data) try
{
  correct (data);

  if (Integration::verbose)
    std::cerr << "Pulsar::"+name+"::execute1"
      " effective "+val+"=" << get_measure() <<
      " reference wavelength=" << get_reference_wavelength() << std::endl;

  range (data, 0, data->get_nchan());

  // restore the original correction measure
  set_measure( backup_measure );

  History* corrected = new History;
  corrected->set_measure( backup_measure );
  corrected->set_reference_wavelength( get_reference_wavelength() );
  data->add_extension( corrected );

}
catch (Error& error) {
  throw error += "Pulsar::"+name+"::execute1";
}


/*! This worker method performs the correction on a specified range
    of frequency channels

    \param ichan the first channel to be corrected
    \param kchan one more than the last channel to be corrected

    \pre the measure and reference_wavelength attributes will
    have been set prior to calling this method

    \pre the delta attribute will have been properly set or reset
*/
template<class C, class H>
void Pulsar::ColdPlasma<C,H>::range (Integration* data,
				     unsigned ichan, unsigned kchan) try
{

  if (Integration::verbose)
    std::cerr << "Pulsar::"+name+"::range "+val+"=" << get_measure()
	 << " lambda_0=" << get_reference_wavelength() << " m" 
         << " delta=" << delta << std::endl;

  if (get_measure() == 0 && delta == get_identity())
    return;

  if (ichan >= data->get_nchan())
    throw Error (InvalidRange, "Pulsar::"+name+"::range",
                 "start chan=%d >= nchan=%d", ichan, data->get_nchan());

  if (kchan > data->get_nchan())
    throw Error (InvalidRange, "Pulsar::"+name+"::range",
                 "end chan=%d > nchan=%d", kchan, data->get_nchan());

  for (unsigned jchan=ichan; jchan < kchan; jchan++) {
    corrector.set_frequency( data->get_centre_frequency (jchan) );
    apply (data, jchan);
  }

}
catch (Error& error) {
  throw error += "Pulsar::"+name+"::range";
}

#endif
