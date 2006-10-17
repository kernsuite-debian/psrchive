//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2002 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

/* $Source: /cvsroot/psrchive/psrchive/Base/Classes/Pulsar/Agent.h,v $
   $Revision: 1.1 $
   $Date: 2006/10/17 14:54:25 $
   $Author: straten $ */

#ifndef __Pulsar_Agent_h
#define __Pulsar_Agent_h

#include "Pulsar/Archive.h"

#ifndef SWIG

namespace Pulsar {

  /* This abstract base class should be inherited through the
     Advocate template. */
  class Archive::Agent : public Reference::Able {

  public:
    
    // Destructor
    virtual ~Agent ();
    
    // Advocate the use of the derived class to interpret filename
    virtual bool advocate (const char* filename) = 0;
    
    // Return the name of the derived class
    virtual std::string get_name () = 0;
    
    // Return a description of the derived class
    virtual std::string get_description () = 0;
    
    // Return the revision of the Archive base class definition
    virtual std::string get_revision () = 0;
    
    // Return a null-constructed instance of the derived class
    virtual Archive* new_Archive () = 0;
    
    // Return the name of the plugins directory
    static std::string get_plugin_path (const char* shell_variable);
    
    // Report to cerr on the status of the Registry (and plugins)
    static void report ();

    // Print the list of agents to the FILE I/O stream
    static std::string get_list ();

  protected:

    // Agents registered for creating derived classes in Archive::load
    static Registry::List<Agent> registry;
      
    // Declare friends with Registry::Entry<Agent> so it can access registry
    friend class Registry::Entry<Agent>;

    // The path from which plugin code will be loaded
    static std::string plugin_path;

    // Declare friends with Archive so Archive::load can access registry
    friend class Archive;

    // Flag that plugin_load has been called
    static bool loaded;

    // Initialization function calls plugin_load if dynamic linkage enabled
    static bool init ();

    // Load plugins from the plugin_path
    static void plugin_load ();

    // Ensure that Advocate classes are linked into executables
    static void static_load ();

    // Report to cerr on the status of the plugins
    static void plugin_report ();

    // Verify that the plugin revision strings match those of the base class
    static void verify_revisions ();

  };

  /* This template base class must be inherited in order to register
     plugins for use in the Archive::load factory.  */
  template<class Plugin>
    class Archive::Advocate : public Agent {

  public:

    // Constructor ensures that template entry is instantiated
    Advocate () { entry.get(); }

    // Return a new instance of the Archive derived class
    Archive* new_Archive () { return new Plugin; }

    // Return the revision number of the Archive base class definition
    /*! This string is automatically generated by CVS.  Do not edit. */
    std::string get_revision () 
      { return Archive::get_revision (PULSAR_ARCHIVE_REVISION); }

    // ensure that the Advocate is linked into static binaries
    static void ensure_linkage () { entry.get(); }

  private:

    // Enter template constructor adds Advocate<Plugin> to Agent::registry
    static Registry::List<Archive::Agent>::Enter<typename Plugin::Agent> entry;

  };

#ifdef _PSRCHIVE_DYNAMIC
  template<class Plugin>
    Registry::List<Archive::Agent>::Enter<typename Plugin::Agent> 
    Archive::Advocate<Plugin>::entry;
#endif

#endif // ! defined SWIG

}

#endif
