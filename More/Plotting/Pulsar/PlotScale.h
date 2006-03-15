//-*-C++-*-

/* $Source: /cvsroot/psrchive/psrchive/More/Plotting/Pulsar/PlotScale.h,v $
   $Revision: 1.4 $
   $Date: 2006/03/15 11:39:06 $
   $Author: straten $ */

#ifndef __Pulsar_PlotScale_h
#define __Pulsar_PlotScale_h

#include "Reference.h"

#include <utility>

namespace Pulsar {

  class Archive;

  //! Plots the position angle of the linearly polarized radiation
  class PlotScale : public Reference::Able {

  public:

    //! Default constructor
    PlotScale ();

    //! Set the minimum and maximum value in the data
    virtual void set_minmax (float min, float max);

    //! Rescale min and max according to current attribute settings
    virtual void get_range (const Archive*, float& min, float& max) const;

    //! Set the world-normalized range on the axis
    void set_range_norm (const std::pair<float,float>& f) { range_norm = f; }
    //! Get the world-normalized range on the axis
    std::pair<float,float> get_range_norm () const { return range_norm; }

    //! Convenience interface
    void set_range_norm (float min, float max) 
    { set_range_norm (std::pair<float,float>(min, max)); }

    //! Set the world-normalized buffer space on either side of the axis
    void set_buf_norm (float f) { buf_norm = f; }
    //! Get the world-normalized buffer space on either side of the axis
    float get_buf_norm () const { return buf_norm; }


  protected:

    float minval;
    float maxval;
    std::pair<float,float> range_norm;
    float buf_norm;

  };

  
}

#endif
