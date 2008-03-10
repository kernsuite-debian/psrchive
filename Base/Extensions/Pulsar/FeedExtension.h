//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2005 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

/* $Source: /cvsroot/psrchive/psrchive/Base/Extensions/Pulsar/FeedExtension.h,v $
   $Revision: 1.8 $
   $Date: 2008/03/10 23:12:43 $
   $Author: nopeer $ */

#ifndef __FeedExtension_h
#define __FeedExtension_h

#include "Pulsar/Archive.h"
#include "Jones.h"

namespace Pulsar
{

  //! Stores a known feed transformation
  /*! The FeedExtension class is used to add an arbitrary transformation to
    a PolnCalibrator instance.  The transformation is included between the
    backend and the receiver platform. */
  class FeedExtension : public Archive::Extension
  {

  public:

    //! Default constructor
    FeedExtension ();

    //! Copy constructor
    FeedExtension (const FeedExtension&);

    //! Operator =
    const FeedExtension& operator= (const FeedExtension&);

    //! Destructor
    ~FeedExtension ();

    //! Clone method
    FeedExtension* clone () const { return new FeedExtension( *this ); }

    //! Get the a text interface
    TextInterface::Parser* get_interface();

    // Text interface to a FeedExtension
    class Interface : public TextInterface::To<FeedExtension>
    {
      public:
	Interface( FeedExtension *s_instance  = NULL );
    };

    //! Return the transformation
    Jones<double> get_transformation () const { return transformation; }

    //! Set the transformation
    void set_transformation (const Jones<double>& x) { transformation = x; }

  protected:

    Jones<double> transformation;

  };


}

#endif
