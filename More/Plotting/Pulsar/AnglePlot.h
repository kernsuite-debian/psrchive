//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2006 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

/* $Source: /cvsroot/psrchive/psrchive/More/Plotting/Pulsar/AnglePlot.h,v $
   $Revision: 1.12 $
   $Date: 2006/12/21 15:07:48 $
   $Author: straten $ */

#ifndef __Pulsar_AnglePlot_h
#define __Pulsar_AnglePlot_h

#include "Pulsar/PhasePlot.h"
#include "Pulsar/PlotIndex.h"

#include "Estimate.h"

namespace Pulsar {

  //! Plots an angle and its error as a function of pulse phase
  class AnglePlot : public PhasePlot {

  public:

    //! Default constructor
    AnglePlot ();

    //! Get the text interface to the configuration attributes
    TextInterface::Class* get_interface ();

    // Text interface to the AnglePlot class
    class Interface : public TextInterface::To<AnglePlot> {
    public:
      Interface (AnglePlot* = 0);
    };

    //! Derived types compute the angles to be plotted
    virtual void get_angles (const Archive*) = 0;

    //! return the minimum and maximum value in degrees
    void prepare (const Archive*);

    //! draw the angle as a function of pulse phase
    void draw (const Archive*);

    std::string get_flux_label (const Archive* data);

    //! Set the sub-integration to plot (where applicable)
    void set_subint (const PlotIndex& _isubint) { isubint = _isubint; }
    PlotIndex get_subint () const { return isubint; }
    
    //! Set the frequency channel to plot (where applicable)
    void set_chan (const PlotIndex& _ichan) { ichan = _ichan; }
    PlotIndex get_chan () const { return ichan; }

    //! will draw the error bar on each point
    void set_error_bars (bool bars) { error_bars = bars; }
    bool get_error_bars () const { return error_bars; }

    //! will draw only those points with linear > threshold * sigma
    void set_threshold (float t) { threshold = t; }
    float get_threshold () const { return threshold; }

    //! span of value in degrees
    void set_span (float t) { span = t; }
    float get_span () const { return span; }

  protected:

    //! Draw error bars
    bool error_bars;

    //! Noise threshold
    float threshold;

    //! Span of values in degrees
    float span;

    //! The angles
    std::vector< Estimate<double> > angles;

    PlotIndex ichan;
    PlotIndex isubint;
  };

}

#endif
