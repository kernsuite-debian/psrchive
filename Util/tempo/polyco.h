//-*-C++-*-

/* $Source: /cvsroot/psrchive/psrchive/Util/tempo/polyco.h,v $
   $Revision: 1.11 $
   $Date: 2001/02/27 05:37:52 $
   $Author: straten $ */

#ifndef __POLY_H
#define __POLY_H

#include <string>
#include <vector>

#include "Phase.h"
#include "MJD.h"
#ifdef MPI
#include "mpi.h"
#endif

class polynomial {

 public:
 
  string psrname;
  string date;
  string utc;
  bool tempov11;         // these parameters exist only when polyco 
                         // generated by versions of tempo > 11.0 (?)
  double doppler_shift;
  double log_rms_resid;
  Phase ref_phase;
  double f0;
  int telescope;
  double freq;
  bool binary;           // these parameters exist only for binaries
  double binph;
  double binfreq;
  float dm;
  MJD reftime;
  double nspan_mins;
  vector<double> coefs;

public:
  polynomial();
  polynomial(const polynomial &);
  ~polynomial(){};
  void init();

  polynomial & operator = (const polynomial &);

  int load (string* instr);
  int unload (string *outstr) const;
  int unload (FILE* fptr) const;

  void   prettyprint  () const;

  // Member functions to perform polyco calculations
  double period            (const MJD &t) const;
  Phase  phase             (const MJD &t) const;
  Phase  phase             (const MJD &t, float obs_freq) const;
  MJD    iphase            (const Phase& p) const;
  double frequency         (const MJD &t) const;
  double chirp             (const MJD &t) const; // d/dt frequency (s^-2)
  double accel             (const MJD &t) const // apparrent accel (m s^-2)
  { return chirp(t)/frequency(t)*2.9979e8; }    // toward observer
  bool   is_tempov11       () const {return (tempov11);};
  int    get_telescope     () const {return (telescope);};
  double get_freq          () const {return (freq);};
  MJD    get_reftime       () const {return (reftime);};
  double get_nspan         () const {return (nspan_mins);};
  float  get_dm            () const {return (dm);};
  int    get_ncoeff        () const {return (int) coefs.size();};
  double get_doppler_shift () const {return doppler_shift / 1e4;};

  MJD start_time () const
    { return reftime - nspan_mins * 60.0/2.0; };
  MJD end_time () const 
    { return reftime + nspan_mins * 60.0/2.0; };

  Phase start_phase () const
    { return phase (start_time()); };
  Phase end_phase () const
    { return phase (end_time()); };
  
  friend int operator == (const polynomial &, const polynomial &);
  friend int operator != (const polynomial &, const polynomial &);

  // MPI functions
#ifdef MPI
  friend int mpiPack_size (const polynomial&, MPI_Comm comm, int* size);
  friend int mpiPack (const polynomial&, void* outbuf, int outcount,
		      int* position, MPI_Comm comm);
  friend int mpiUnpack (void* inbuf, int insize, int* position, 
			polynomial*, MPI_Comm comm);
#endif

};

class polyco {

 protected:
  static string anyPsr;
  static MJD    today;

 public:
  static bool verbose;
  static bool debug;
  static double precision;

  vector<polynomial> pollys;
  // null initializer
  polyco();                    
  polyco(const polyco &);

  // Load in polycos
  polyco(const char * id);
  polyco(const string id);
  polyco & operator= (const polyco &);

  ~polyco(){};

  // these functions return the number of polynomials successfully loaded
  int load (const char* filename, size_t nbytes=0);
  int load (const string& filename, size_t nbytes=0)
	{ return load (filename.c_str(), nbytes); }
  int load (FILE * fp, size_t nbytes=0);
  int load (string* instr);

  // these functions return -1 upon error
  int unload (const char* filename) const ;
  int unload (const string& filename) const
        { return unload (filename.c_str()); }

  // these functions return the number of bytes unloaded (-1 on error)
  int unload (string *outstr) const;
  int unload (FILE* fptr) const;

  void  prettyprint  () const;
  const polynomial* nearest (const MJD &t, 
			     const string& psrname = anyPsr) const;

  const polynomial& best (const MJD &t, const string& psr = anyPsr) const;
  const polynomial& best (const Phase &p, const string& psr = anyPsr) const;

  int i_nearest (const MJD &t,   const string& psrname = anyPsr) const;
  int i_nearest (const Phase &p, const string& psrname = anyPsr) const;

  double doppler_shift (const MJD& t, const string& psr = anyPsr) const
    { return best(t, psr).get_doppler_shift(); };

  Phase phase (const MJD& t, const string& psr = anyPsr) const
    { return best(t, psr).phase(t); };

  Phase phase (const MJD& t, float obs_freq, const string& psr = anyPsr) const
    { return best(t, psr).phase(t, obs_freq); };

  MJD iphase (const Phase& phase, const string& psr = anyPsr) const
    { return best(phase, psr).iphase(phase); };

  double period(const MJD& t, const string& psr = anyPsr) const
    { return best(t, psr).period(t); };

  double frequency(const MJD& t, const string& psr = anyPsr) const
    { return best(t, psr).frequency(t); };

  double chirp(const MJD& t, const string& psr = anyPsr) const
    { return best(t, psr).chirp(t); };

  double accel(const MJD& t, const string& psr = anyPsr) const
    { return best(t, psr).accel(t); };

  // bool   is_tempov11   () const { return pollys.front().is_tempov11(); };
  int    get_telescope () const { return pollys.front().get_telescope(); };
  double get_freq      () const { return pollys.front().get_freq(); };
  MJD    get_reftime   () const { return pollys.front().get_reftime(); };
  double get_nspan     () const { return pollys.front().get_nspan(); };
  float  get_dm        () const { return pollys.front().get_dm(); };
  int    get_ncoeff    () const { return pollys.front().get_ncoeff(); };

  bool is_tempov11() const;

  MJD  start_time () const { return pollys.front().start_time(); };
  MJD  end_time ()   const { return pollys.back().end_time(); };

  Phase start_phase () const { return pollys.front().start_phase(); };
  Phase end_phase ()   const { return pollys.back().end_phase(); };

  friend int operator == (const polyco &, const polyco &);
  friend int operator != (const polyco &, const polyco &);

#ifdef MPI
  friend int mpiPack_size (const polyco&, MPI_Comm comm, int* size);
  friend int mpiPack   (const polyco&, void* outbuf, int outcount,
			int* position, MPI_Comm comm);
  friend int mpiUnpack (void* inbuf, int insize, int* position, 
			polyco*, MPI_Comm comm);
#endif

};

inline ostream& operator<< (ostream& ostr, const polyco& p) {
  string out; p.unload(&out); return ostr << out; }

#endif

