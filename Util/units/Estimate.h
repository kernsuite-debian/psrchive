//-*-C++-*-

/* $Source: /cvsroot/psrchive/psrchive/Util/units/Estimate.h,v $
   $Revision: 1.31 $
   $Date: 2005/03/22 06:13:46 $
   $Author: straten $ */

#ifndef __Estimate_h
#define __Estimate_h

#include "PromoteTraits.h"
#include "Traits.h"

#include <iostream>
#include <math.h>


// forward declarations
template <typename T, typename U=T> class MeanEstimate;
template <typename T, typename U=T> class MeanRadian;

//! Estimates with a value, \f$ x \f$, and a variance, \f$ \sigma^2 \f$
/*!
  Where \f$ y = f (x_1, x_2, ... x_n) \f$, then
  \f$ \sigma_y^2 = \sum_{i=1}^n ({\partial f \over \partial x_i})^2\sigma_i^2 \f$

  See http://mathworld.wolfram.com/ErrorPropagation.html
*/
template <typename T, typename U=T>
class Estimate
{

 public:

  //! The value, \f$ x \f$
  T val;
  //! The variance of the value, \f$ \sigma_x^2 \f$
  U var;

  //! Construct from a value, \f$ x \f$, and its variance, \f$ \sigma_x^2 \f$
  Estimate (T _val=0, U _var=0) { val=_val; var=_var; }

  //! Construct from another Estimate
  template <typename V, typename W>
  Estimate (const Estimate<V,W>& d) { val=d.val; var=d.var; }

  //! Construct from MeanEstimate
  template <typename V, typename W>
  Estimate (const MeanEstimate<V,W>& m) { operator=( m.get_Estimate() ); }

  //! Construct from MeanRadian
  template <typename V, typename W>
  Estimate (const MeanRadian<V,W>& m) { operator=( m.get_Estimate() ); }

  //! Assignment operator
  const Estimate& operator= (const Estimate& d)
  { val=d.val; var=d.var; return *this; }

  //! Set the value
  void set_value (const T& t) { val = t; }

  //! Get the value
  T get_value () const { return val; }

  //! Set the variance
  void set_variance (const U& u) { var = u; }

  //! Get the variance
  U get_variance () const { return var; }

  //! Set the error
  void set_error (const U& u) { var = u*u; }

  //! Get the error
  U get_error () const { return sqrt(var); }

  //! Array access to value
  T& operator [] (unsigned n) { return val; }

  //! Array access to value
  T operator [] (unsigned n) const { return val; }

  //! Addition operator
  const Estimate& operator+= (const Estimate& d)
  { val += d.val; var += d.var; return *this; }

  //! Subtraction operator
  const Estimate& operator-= (const Estimate& d)
  { val -= d.val; var += d.var; return *this; }

  //! Multiplication operator
  /*! Where \f$ r=x*y \f$, \f$\sigma^2_r = y^2\sigma^2_x + x^2\sigma^2_y\f$ */
  const Estimate& operator*= (const Estimate& d)
  { T v=val; val*=d.val; var=v*v*d.var+d.val*d.val*var; return *this; }

  //! Division operator
  const Estimate& operator/= (const Estimate& d)
  { return operator *= (d.inverse()); }

  //! Equality operator
  bool operator == (const Estimate& d) const
  { return val == d.val; }

  //! Inequality operator
  bool operator != (const Estimate& d) const
  { return ! operator == (d); }

  //! Comparison operator
  bool operator < (const Estimate& d) const
  { return val < d.val; }

  //! Comparison operator
  bool operator > (const Estimate& d) const
  { return val > d.val; }

  //! Inversion operator
  /*! Where \f$ r=1/x \f$, \f$ \sigma_r=r^2\sigma_x/x^2 = \sigma_x/x^4 \f$ */
  const Estimate inverse () const
  { T v=1.0/val; return Estimate (v,var*v*v*v*v); }

  friend const Estimate operator + (Estimate a, const Estimate& b)
  { return a+=b; }

  friend const Estimate operator - (Estimate a, const Estimate& b)
  { return a-=b; }
  
  friend const Estimate operator * (Estimate a, const Estimate& b)
  { return a*=b; }
  
  friend const Estimate operator / (Estimate a, const Estimate& b)
  { return a/=b; }

  //! Negation operator
  friend const Estimate operator - (Estimate a)
  { return Estimate (-a.val, a.var); }

  //! See http://mathworld.wolfram.com/ErrorPropagation.html Equation (15)
  friend const Estimate exp (const Estimate& u)
  { T val = ::exp (u.val); return Estimate (val, val*val*u.var); }

  //! See http://mathworld.wolfram.com/ErrorPropagation.html Equation (17)
  friend const Estimate log (const Estimate& u)
  { return Estimate (::log (u.val), u.var/(u.val*u.val)); }

  //! \f$ \left({\partial\sin x\over\partial x}\right)^2 = (1-\sin^2x) \f$
  friend const Estimate sin (const Estimate& u)
  { T val = ::sin (u.val); return Estimate (val, (1-val*val)*u.var); }

  //! \f$ \left({\partial\cos x\over\partial x}\right)^2 = (1-\cos^2x) \f$
  friend const Estimate cos (const Estimate& u)
  { T val = ::cos (u.val); return Estimate (val, (1-val*val)*u.var); }

  //! \f$ {\partial\over\partial x} \tan^-1 (x) = (1+x^2)^{-1} \f$
  friend const Estimate atan2 (const Estimate& s, const Estimate& c)
  { T c2 = c.val*c.val;  T s2 = s.val*s.val;  T sc2 = c2+s2;
    return Estimate (::atan2 (s.val, c.val),(c2*s.var+s2*c.var)/(sc2*sc2)); }
  
  //! \f$ {\partial\over\partial x} x^{1\over2} = {1\over2}x^{-{1\over2}} \f$
  friend const Estimate sqrt (const Estimate& u)
  { return Estimate (::sqrt (u.val), 0.25*u.var/fabs(u.val)); }

};


//! Enable Estimate class to be passed to certain template functions
template <class T, class U> struct DatumTraits< Estimate<T,U> >
{
  ElementTraits<T> element_traits;
  static inline unsigned ndim () { return 1; }
  static inline T& element (Estimate<T,U>& t, unsigned idim) 
  { return t.val; }
  static inline const T& element (const Estimate<T,U>& t, unsigned idim)
  { return t.val; }
};

#ifdef PROMOTE_TRAITS_SPECIALIZE

template <class T, class U, class V, class W>
class PromoteTraits< Estimate<T,U>, Estimate<V,W> >
{
  public:
    typedef Estimate< typename PromoteTraits<T,V>::promote_type, 
                      typename PromoteTraits<U,W>::promote_type > promote_type;
};

#endif


//! Useful for quickly printing the values
template<typename T, typename U>
std::ostream& operator<< (std::ostream& ostr, const Estimate<T,U>& estimate)
{
  return ostr << "(" << estimate.val << "\261" << sqrt(estimate.var) << ")";
}


/*!
  \f$ {\bar{x} over \bar{\sigma}^2} = \sum_{i=1}^n {x_i \over \sigma_i^2} \f$

  See http://mathworld.wolfram.com/MaximumLikelihood.html (Eqs. 16 and 19)
*/
template <typename T, typename U>
class MeanEstimate
{
 public:
  //! The value, normalized by its variance
  T norm_val;
  //! The inverse of its variance
  U inv_var;

  //! Construct from a value (normalized by / and) the inverse of its variance
  MeanEstimate (T _val=0, U _var=0) { norm_val=_val; inv_var=_var; }

  //! Copy Constructor
  MeanEstimate (const MeanEstimate& d)
  { norm_val=d.norm_val; inv_var=d.inv_var; }

  //! Construct from an Estimate
  MeanEstimate (const Estimate<T,U>& d)
  { norm_val = 0; inv_var = 0; operator += (d); }

  //! Assignment operator
  const MeanEstimate& operator= (const MeanEstimate& d)
  { norm_val=d.norm_val; inv_var=d.inv_var; return *this; }

  //! Addition operator
  const MeanEstimate& operator+= (const MeanEstimate& d)
  { norm_val += d.norm_val; inv_var += d.inv_var; return *this; }

  //! Addition operator
  const MeanEstimate& operator+= (const Estimate<T,U>& d)
  { if (d.var){ U v=1.0/d.var; norm_val+=d.val*v; inv_var+=v; } return *this; }

  //! Equality operator
  bool operator == (T _norm_val) const
  { return norm_val == _norm_val; }

  Estimate<T,U> get_Estimate () const
  { U var=0.0; if (inv_var != 0.0) var = 1.0/inv_var; 
    return Estimate<T,U> (norm_val*var, var); }

};

//! Useful for quickly printing the values
template<typename T, typename U>
std::ostream& operator<< (std::ostream& ostr, const MeanEstimate<T,U>& mean)
{
  return ostr << mean.get_Estimate();
}

//! Calculates the mean of a value in radians
/*! This class stores the mean value of the sine and cosine of each angle
    added to the mean, and returns the inverse tangent of their ratio. */
template <typename T, typename U>
class MeanRadian
{

 public:

  //! Default constructor
  MeanRadian () { }

  //! Copy constructor
  MeanRadian (const MeanRadian& mean)
  { operator = (mean); }

  //! Assignment operator
  const MeanRadian& operator= (const MeanRadian& mean)
  { cosine = mean.cosine; sine = mean.sine; return *this; }

  //! Construct from an Estimate
  MeanRadian (const Estimate<T,U>& d)
  { operator = (d); }

  //! Assignment operator, Estimate represents a value in radians
  const MeanRadian& operator= (const Estimate<T,U>& d)
  { cosine = cos(d); sine = sin(d); return *this; }

  //! Addition operator
  const MeanRadian& operator+= (const MeanRadian& d)
  { cosine += d.cosine; sine += d.sine; return *this; }

  //! Addition operator, Estimate represents a value in radians
  const MeanRadian& operator+= (const Estimate<T,U>& d)
  { cosine += cos(d); sine += sin(d); return *this; }

  Estimate<T,U> get_Estimate () const
  { if (sine.norm_val==0 && cosine.norm_val==0) return Estimate<T,U>(0,0);
    return atan2 (sine.get_Estimate(), cosine.get_Estimate()); }
  
 protected:
  //! The average cosine
  MeanEstimate<T,U> cosine;

  //! The average sine
  MeanEstimate<T,U> sine;

};

//! Useful for quickly printing the values
template<typename T, typename U>
std::ostream& operator<< (std::ostream& ostr, const MeanRadian<T,U>& mean)
{
  return ostr << mean.get_Estimate();
}

#endif

