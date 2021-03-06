/***************************************************************************
 *
 *   Copyright (C) 2009 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include "MEAL/ComplexRVM.h"
#include "MEAL/RotatingVectorModel.h"

#include "MEAL/ChainRule.h"
#include "MEAL/VectorRule.h"
#include "MEAL/Gain.h"
#include "MEAL/ComplexCartesian.h"
#include "MEAL/ScalarMath.h"

using namespace std;

void MEAL::ComplexRVM::init ()
{
  if (verbose)
    cerr << "MEAL::ComplexRVM::init" << endl;

  rvm = new RotatingVectorModel;

  ScalarMath N = *(rvm->get_north());
  ScalarMath E = *(rvm->get_east ());

  ScalarMath Q = N*N - E*E;
  ScalarMath U = 2.0 * N * E;

  ChainRule<Complex>* phase = new ChainRule<Complex>;

  // Set up a complex phase function with phase equal to RVM P.A.
  phase->set_model( new ComplexCartesian );
  phase->set_constraint( 0, Q.get_expression() );
  phase->set_constraint( 1, U.get_expression() );

  gain = new VectorRule<Complex>;

  // gain->set_verbose (true);

  add_model (gain);
  add_model (phase);
}

class MEAL::ComplexRVM::State
{
public:
  double phase_radians;
  Reference::To< Gain<Complex> > gain;

  State ()
  { 
    phase_radians = 0;
    gain = new Gain<Complex>;
  }
};

MEAL::ComplexRVM::ComplexRVM ()
{
  init ();
}

//! Copy constructor
MEAL::ComplexRVM::ComplexRVM (const ComplexRVM& copy)
{
  init ();
  operator = (copy);
}

//! Assignment operator
MEAL::ComplexRVM& MEAL::ComplexRVM::operator = (const ComplexRVM& copy)
{
  rvm->copy (copy.rvm);
  return *this;
}

MEAL::ComplexRVM::~ComplexRVM ()
{
}

//! Return the name of the class
string MEAL::ComplexRVM::get_name () const
{
  return "ComplexRVM";
}

//! Return the rotating vector model
MEAL::RotatingVectorModel* MEAL::ComplexRVM::get_rvm ()
{
  return rvm;
}

//! Add a state: phase in turns, L is first guess of linear polarization
void MEAL::ComplexRVM::add_state (double phase, double L)
{
  State s;
  s.phase_radians = phase;
  s.gain->set_gain (L);

  state.push_back( s );
  gain->push_back( s.gain );
}

//! Get the number of states
unsigned MEAL::ComplexRVM::get_nstate () const
{
  return state.size();
}

//! Set the current state for which the model will be evaluated
void MEAL::ComplexRVM::set_state (unsigned i)
{
  check (i, "set_state");
  gain->set_index (i);
  rvm->set_abscissa (state[i].phase_radians);
}

//! Set the phase of the ith state
void MEAL::ComplexRVM::set_phase (unsigned i, double phase)
{
  check (i, "set_phase");
  state[i].phase_radians = phase;
}

//! Get the phase of the ith state
double MEAL::ComplexRVM::get_phase (unsigned i) const
{
  check (i, "get_phase");
  return state[i].phase_radians;
}

//! Set the linear polarization of the ith state
void MEAL::ComplexRVM::set_linear (unsigned i, const Estimate<double>& L)
{
  check (i, "set_linear");
  state[i].gain->set_gain(L);
}

//! Get the linear polarization of the ith state
Estimate<double> MEAL::ComplexRVM::get_linear (unsigned i) const
{
  check (i, "get_linear");
  return state[i].gain->get_gain();
}

void MEAL::ComplexRVM::renormalize (double renorm)
{
  // MEAL::Function::verbose = true;

  for (unsigned i=0; i<state.size(); i++)
  {
    double L = get_linear(i).get_value();

    set_state(i);
    complex<double> result = evaluate ();

    // cerr << "L=" << L << " abs=" << std::abs(result);

    double new_L = renorm * L * L / abs(result);

    set_linear( i, new_L );

    result = evaluate ();

    // cerr << " : new L=" << new_L << " abs=" << abs(result) << endl;
  }
}

void MEAL::ComplexRVM::check (unsigned i, const char* method) const
{
  if (i >= state.size())
    throw Error (InvalidParam, string("MEAL::ComplexRVM::") + method,
		 "istate=%u >= nstate=%u", i, state.size());
}
