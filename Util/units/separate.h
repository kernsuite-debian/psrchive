//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2004 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

/* $Source: /cvsroot/psrchive/psrchive/Util/units/separate.h,v $
   $Revision: 1.3 $
   $Date: 2006/10/06 21:13:55 $
   $Author: straten $ */

#ifndef __UTILS_UNITS_SEPARATE_H
#define __UTILS_UNITS_SEPARATE_H

#include <vector>
#include <string>

//! Return true if the input is enclosed in bracket
bool braced (const std::string&);

//! Separate a list of delimiter-separated commands into a vector of strings
void separate (std::string text, std::vector<std::string>& list,
	       const std::string& delimiters = " \t\n");

void standard_separation (std::vector<std::string>& list, const std::string&);

#endif

