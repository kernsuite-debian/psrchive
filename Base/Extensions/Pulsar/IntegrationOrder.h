//-*-C++-*-
                                                                                
/* $Source: /cvsroot/psrchive/psrchive/Base/Extensions/Pulsar/IntegrationOrder.h,v $
   $Revision: 1.1 $
   $Date: 2003/12/07 06:35:49 $
   $Author: ahotan $ */
                                                                                
#ifndef __IntegrationOrder_h
#define __IntegrationOrder_h
                                                                                
#include "Pulsar/Archive.h"
#include "Estimate.h"
                                                                                
namespace Pulsar {
  
  /*! This IntegrationOrder class provides a means for developers to
    re-index the way Integrations are stored. It is a virtual base class
    that must be inherited by developers who wish to define a new way to
    index the Integrations in their Archive */
  
  class IntegrationOrder : public Pulsar::Archive::Extension {
                                                                                
  public:
    
    // These are the standard Archive::Extension methods:

    //! Default constructor
    IntegrationOrder ();
    
    //! Copy constructor
    IntegrationOrder (const IntegrationOrder& extension);
    
    //! Operator =
    const IntegrationOrder& operator= (const IntegrationOrder& extension);
    
    //! Destructor
    virtual ~IntegrationOrder ();
    
    //! Clone method
    virtual IntegrationOrder* clone () const = 0;
    
    //! Return a string describing the type of indexing used
    string get_IndexState () const;
    
    //! Set the custom index value associated with an Integration
    void set_Index (unsigned subint, Estimate<double> i);
    
    //! Get the custom index value associated with an Integration
    Estimate<double> get_Index (unsigned subint) const;

    // These are the virtual base methods of the IntegrationOrder class:

    //! Re-order the Integrations into the desired state
    virtual void organise (Archive*) = 0;
    
    //! Add two Archive instances that have the same IntegrationOrder
    virtual void append (Archive*, const Archive*) = 0;
    
    /*! Scrunch together this many consecutive Integrations in an Archive 
      with this IntegrationOrder */
    virtual void combine (Archive*, unsigned) = 0;
    
  protected:

    //! An index state identifier string
    string IndexState;
    
    //! Storage for the actual Integration indicies
    vector<Estimate<double> > indices;                                                    

  };                                                                            
                                                                                
}
                                                                                
#endif
