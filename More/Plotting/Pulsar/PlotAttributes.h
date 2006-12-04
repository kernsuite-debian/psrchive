//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2006 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

/* $Source: /cvsroot/psrchive/psrchive/More/Plotting/Pulsar/PlotAttributes.h,v $
   $Revision: 1.4 $
   $Date: 2006/12/04 01:08:18 $
   $Author: straten $ */

#ifndef __Pulsar_PlotAttributes_h
#define __Pulsar_PlotAttributes_h

#include "TextInterface.h"

namespace Pulsar {

  //! Stores the properties of the plot frame
  class PlotAttributes : public Reference::Able {

  public:

    //! Default constructor
    PlotAttributes ();

    //! Destructor
    ~PlotAttributes ();

    // Text interface to the PlotAttributes class
    class Interface : public TextInterface::To<PlotAttributes> {
    public:
      Interface (PlotAttributes* = 0);
    };

    //! Set the character height
    virtual void set_character_height (float h) { character_height = h; }
    float get_character_height () const { return character_height; }

    //! Set the character font
    virtual void set_character_font (int font) { character_font = font; }
    int get_character_font () const { return character_font; }

    //! Set the line width
    virtual void set_line_width (int width) { line_width = width; }
    int get_line_width () const { return line_width; }

    //! Set publication quality character height and font and line width
    virtual void set_publication_quality (bool flag = true);
    bool get_publication_quality () const { return false; }

    //! Set to false to disable all labels
    virtual void set_plot_labels (bool flag = true);
    bool get_plot_labels () const { return true; }

    //! Apply a named set of commands (a shortcut)
    virtual void apply_set (const std::string& name);
    std::string get_applied () const { return "none"; }

  protected:

    float character_height;
    int character_font;
    int line_width;

  };

}

#endif
