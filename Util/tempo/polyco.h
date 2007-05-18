//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 1999 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

/* $Source: /cvsroot/psrchive/psrchive/Util/tempo/polyco.h,v $
   $Revision: 1.41 $
   $Date: 2007/05/18 21:25:43 $
   $Author: straten $ */

#ifndef __POLY_H
#define __POLY_H

#include "Predictor.h"

#include <string>
#include <vector>

#ifdef HAVE_MPI
#include <mpi.h>
#endif

class polynomial {
  
  friend class polyco;
  
protected:
  
  //! The pulsar name
  std::string psrname;
  
  //! The calendar date string
  std::string date;

  //! The UTC string
  std::string utc;

  //! The pulsar phase at reftime
  Phase ref_phase;

  //! Reference rotation frequency, F0 (Hz)
  double ref_freq;

  //! The epoch to which the polynomial is referenced
  MJD ref_time;

  //! TEMPO telescope id code
  char telescope;

  //! Observing frequency
  double freq;

  //! true if the pulsar is in a binary system
  bool binary;

  //! if binary, binary phase
  double binph;

  //! if binary, binary frequency
  double binfreq;

  //! dispersion measure
  float dm;

  //! number of minutes spanned by polynomial
  double nspan_mins;

  //! Flag that the polyco was created using TEMPO v.11 or greater
  bool tempov11;

  //! Doppler shift due to Earth's motion along line of sight
  double doppler_shift;

  //! log of the r.m.s residual between polynomial and model
  double log_rms_resid;

  //! polynomial coefficients
  std::vector<double> coefs;

  //! intializes all values to null
  void init();

public:

  //! Interface for setting attributes
  class Expert;
  friend class Expert;

  static double precision;

  //! null constructor
  polynomial() { init(); }

  //! copy constructor
  polynomial (const polynomial& poly) { operator = (poly); }

  //! destructor
  ~polynomial() {}

  polynomial& operator = (const polynomial& poly);

  int load (std::string* instr);
  int unload (std::string *outstr) const;
  int unload (FILE* fptr) const;

  void prettyprint  () const;

  //! Returns the pulse period (in seconds) at the given time
  double period (const MJD &t) const;

  //! Returns the pulse phase (in turns) at the given time
  Phase phase (const MJD &t) const;

  //! Returns the time at the given pulse phase
  MJD iphase (const Phase& p, const MJD* = 0) const;

  //! Returns the spin frequency (in Hz) at the given time
  long double frequency (const MJD &t) const;

  //! Return the phase correction for dispersion delay
  Phase dispersion (const MJD &t, long double MHz) const;

  //! Returns the spin frequency derivative (in Hz/s) at the given time 
  double chirp (const MJD &t) const;

  //! Returns the apparent acceleration toward observer (m s^-2)
  double accel (const MJD &t) const { return chirp(t)/frequency(t)*2.9979e8; }

  bool   is_tempov11       () const {return tempov11; }
  char   get_telescope     () const {return telescope; }
  double get_freq          () const {return freq; }
  MJD    get_reftime       () const {return ref_time; }
  Phase  get_refphase      () const {return ref_phase; }
  double get_reffrequency  () const {return ref_freq; }
  double get_nspan         () const {return nspan_mins; }
  float  get_dm            () const {return dm; }
  int    get_ncoeff        () const {return (int) coefs.size(); }
  double get_doppler_shift () const {return doppler_shift / 1e4; }
  std::string get_psrname  () const {return psrname; }
  bool   get_binary        () const {return binary; } 
  double get_binph         () const {return binph; }

  static double flexibility;

  MJD start_time () const
  { return ref_time - nspan_mins * (1.0+flexibility) * 60.0/2.0; };
  MJD end_time () const 
  { return ref_time + nspan_mins * (1.0+flexibility) * 60.0/2.0; };

  Phase start_phase () const
  { return phase (start_time()); };
  Phase end_phase () const
  { return phase (end_time()); };
  
  friend int operator == (const polynomial &, const polynomial &);
  friend int operator != (const polynomial &, const polynomial &);

  // MPI functions
#ifdef HAVE_MPI
  friend int mpiPack_size (const polynomial&, MPI_Comm comm, int* size);
  friend int mpiPack (const polynomial&, void* outbuf, int outcount,
		      int* position, MPI_Comm comm);
  friend int mpiUnpack (void* inbuf, int insize, int* position, 
			polynomial*, MPI_Comm comm);
#endif

};

class polynomial::Expert {

public:
  Expert (polynomial* p) { instance = p; }

  void set_tempov11      (bool v) { instance->tempov11 = v; }
  void set_telescope     (char v) { instance->telescope = v; }
  void set_freq          (double v) { instance->freq = v; }
  void set_reftime       (const MJD& v) { instance->ref_time = v; }
  void set_refphase      (const Phase& v) { instance->ref_phase = v; }
  void set_reffrequency  (double v) { instance->ref_freq = v; }
  void set_nspan         (double v) { instance->nspan_mins = v; }
  void set_dm            (float v) { instance->dm = v; }
  void set_ncoef         (unsigned v) { instance->coefs.resize(v); }
  void set_doppler_shift (double v) { instance->doppler_shift = v * 1e4; }
  void set_psrname       (const std::string& v) { instance->psrname = v; }
  void set_binary        (bool v) { instance->binary = v; }
  void set_binph         (double v) { instance->binph = v; }

  double* get_coefs () { return &(instance->coefs[0]); }

  polynomial* instance;
};


class polyco : public Pulsar::Predictor {

public:
  static bool debug;

  //! The polynomial sets
  std::vector<polynomial> pollys;

  //! null initializer
  polyco () {}
  polyco (const polyco& poly) { operator = (poly); }

  //
  // Pulsar::Predictor implementation
  //

  //! Return a new, copy constructed instance of self
  Predictor* clone () const;

  //! Add the information from the supplied predictor to self
  void insert (const Predictor*);

  //! Set the observing frequency at which predictions will be made
  void set_observing_frequency (long double MHz);

  //! Get the observing frequency at which phase and epoch are related
  long double get_observing_frequency () const;

  //! Return the phase, given the epoch
  Phase phase (const MJD& t) const
  { return best(t).phase(t); }

  //! Return the phase plus the dispersion delay
  Phase phase (const MJD& t, long double MHz) const
  { const polynomial& b = best(t); return b.phase(t) + b.dispersion(t,MHz); }

  //! Return the epoch, given the phase
  MJD iphase (const Phase& phase, const MJD* guess = 0) const
  { return best(phase).iphase(phase, guess); }

  //! Return the spin frequency, given the epoch
  long double frequency (const MJD& t) const
  { return best(t).frequency(t); }

  //! Return the phase correction for dispersion delay
  Phase dispersion (const MJD &t, long double MHz) const
  { return best(t).dispersion(t,MHz); }

  //! Load from an open stream
  void load (FILE*);

  //! Unload to an open stream
  void unload (FILE*) const;

  //
  // the rest
  //

  //! Load in polycos
  polyco (const std::string& id);
  polyco& operator = (const polyco& poly);

  virtual ~polyco() {}

  //! these functions return the number of polynomials successfully loaded
  int load (const std::string& filename);
  int load (std::string* instr);

  // these functions return -1 upon error
  int unload (const std::string& filename) const;

  // these functions return the number of bytes unloaded (-1 on error)
  int unload (std::string *outstr) const;

  void append (const polyco& poly);

  void  prettyprint () const;

  const polynomial* nearest (const MJD &t) const;

  const polynomial& best (const MJD &t) const;
  const polynomial& best (const Phase &p) const;

  virtual int i_nearest (const MJD &t) const;
  virtual int i_nearest (const Phase &p) const;

  double doppler_shift (const MJD& t) const
  { return best(t).get_doppler_shift(); };

  double period(const MJD& t) const
  { return best(t).period(t); };

  double chirp(const MJD& t) const
  { return best(t).chirp(t); };

  double accel(const MJD& t) const
  { return best(t).accel(t); };

  char   get_telescope () const;
  double get_freq      () const;
  MJD    get_reftime   () const;
  double get_refperiod () const;
  double get_nspan     () const;
  float  get_dm        () const;
  int    get_ncoeff    () const;
  std::string get_psrname   () const;

  bool is_tempov11() const;

  MJD  start_time () const { return pollys.front().start_time(); };
  MJD  end_time ()   const { return pollys.back().end_time(); };

  Phase start_phase () const { return pollys.front().start_phase(); };
  Phase end_phase ()   const { return pollys.back().end_phase(); };

  friend int operator == (const polyco &, const polyco &);
  friend int operator != (const polyco &, const polyco &);

#ifdef HAVE_MPI
  friend int mpiPack_size (const polyco&, MPI_Comm comm, int* size);
  friend int mpiPack   (const polyco&, void* outbuf, int outcount,
			int* position, MPI_Comm comm);
  friend int mpiUnpack (void* inbuf, int insize, int* position, 
			polyco*, MPI_Comm comm);
#endif

};

inline std::ostream& operator<< (std::ostream& ostr, const polyco& p)
{
  std::string out; p.unload(&out); return ostr << out;
}

#endif

