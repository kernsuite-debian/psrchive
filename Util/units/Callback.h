//-*-C++-*-

/* $Source: /cvsroot/psrchive/psrchive/Util/units/Callback.h,v $
   $Revision: 1.1 $
   $Date: 2004/11/22 09:31:49 $
   $Author: straten $ */

/* *************************************************************************

   Callback - a namespace implementing a means of sending signals

   Willem van Straten - February 2000 and May 2003

   ************************************************************************* */

#ifndef __Callback_Sender_h
#define __Callback_Sender_h

#include <vector>
#include "Functor.h"

//! Implements a callback
template<class Type>
class Callback : public Reference::Able {
    
public:

  //! Verbosity flag
  bool verbose;
  
  //! Default constructor
  Callback () { verbose = false; }
    
  //! Destructor
  virtual ~Callback () { }
    
  //! Call all registered methods, passing data as the argument
  void send (const Type& data)
  {
    clean ();
    for (unsigned irecv=0; irecv < recipients.size(); irecv++) {
      if (verbose) cerr << "Callback::send " << irecv 
			<< "/" << recipients.size() << endl;
      recipients[irecv] -> call (data);
    }
  }
  
  //! Add an instance and its method to be called during send
  template<class Class, typename Method>
  void connect (Class* instance, Method method)
  {
    change (instance, method, true);
  }
  
  //! Remove instance and its method
  template<class Class, typename Method>
  void disconnect (Class* instance, Method method)
  {
    change (instance, method, false);
  }
  
  template<class Class, typename Method>
  void connect (Reference::To<Class>& ref, Method method)
  {
    connect (ref.get(), method);
  }
  
  template<class Class, typename Method>
  void disconnect (Reference::To<Class>& ref, Method method)
  {
    disconnect (ref.get(), method);
  }
  
protected:
  
  vector< Reference::To<typename Functor<void(Type)>::Base> > recipients;

  
  template<class Class, typename Method>
  void change (Class* instance, Method method, bool add)
  {
    if (!instance || !method)
      return;
    
    typedef typename Functor<void(Type)>::template
      Method<Class, Method> ReceiverBase;
    
    ReceiverBase* receiver = 0;
    
    for (unsigned i=0; i < recipients.size(); i++) {
      
      receiver = dynamic_cast<ReceiverBase*>(recipients[i].ptr());
      
      if (receiver && receiver -> matches (instance, method)) {
	if (!add)
	  recipients.erase(recipients.begin()+i);
	return;
      }
      
    }
    
    if (add) {
      receiver = new ReceiverBase (instance, method);
      recipients.push_back (receiver);
    }
    
  }

  void clean ()
  {
    for (unsigned i=0; i < recipients.size(); )
      if (!recipients[i]->is_valid())
        recipients.erase(recipients.begin()+i);
      else
        i++;
  }

};

#endif // #ifndef __Callback_Callback_h
