#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "coord.h"
#include "f772c.h"

/* **********************************************************************
   str2coord - converts a string containing pulsar coordinates to
               ra and dec in radians.  The string may be of the form:

	       hhmm~ddmm    or  hhmmss~ddmmss
	       hh:mm~dd:mm  or  hh:mm:ss~dd:mm:ss

	       or any combination of mentalness, as long as each field
	       contains exactly two characters; or, a ':' delimits a
               maximum of two characters  (may contain one!
	       so "h:mm-dd:mm" will work), where:

	       hh  hour angle of ra
	       dd  degrees of declination
	       mm  minutes
	       ss  seconds
	       ~   + or - sign of the degrees of declination

	       returns a -1 on error, zero on success
   ********************************************************************** */

int str2coord (double *ra, double *dec, const char* coordstring) 
{
  char* coordstr = strdup (coordstring);
  char* rastr = coordstr;   /* beginning of RA */

  /* search for the +/- sign of the DEC */
  char* decstr = strchr (coordstr, '-');
  if (decstr == NULL) {
    decstr = strchr (coordstr, '+');
    if (decstr == NULL)
      decstr = strchr (coordstr, ' ');
    if (decstr == NULL) {
      fprintf (stderr, "No +,-,or space in coordinate string '%s'\n",coordstr);
      free (coordstr);
      return -1;
    }
  }

  if (str2ra (ra, rastr) < 0)
    return -1;

  return str2dec (dec, decstr);
}

int str2ra (double *ra, const char* rastring) 
{
  char* begra = strdup (rastring);
  char* rastr = begra;
  char* endstr = strchr (rastr, '\0');
  char* travel;

  double hr, min, sec;

  hr = min = sec = 0.0;

  /* parse RA hour */
  if (sscanf (rastr, "%2lf", &hr) != 1)
    return -1;

  travel = strchr (rastr, ':');
  if (travel && ((travel-rastr) <= 2))
    rastr = travel +1;
  else
    rastr += 2;

  if (rastr >= endstr)
    return -1;

  /* parse RA minutes (if specified) */
  if (sscanf (rastr, "%2lf", &min) != 1)
    return -1;

  travel = strchr (rastr, ':');
  if (travel && ((travel-rastr) <= 2))
    rastr = travel +1;
  else
    rastr += 2;

  /* parse RA seconds (if specified) */
  if (rastr < endstr)
    if (sscanf (rastr, "%lf", &sec) != 1)
      return -1;
  
  *ra = (hr + min/60.0 + sec/3600.0)*M_PI/12.0;

  /* printf ("RA %g:%g:%g parsed -> %g radians\n", hr, min, sec, *ra); */
  free (begra);
  return 0;
}


int str2dec (double *dec, const char* dectring) 
{
  char* begdec = strdup (dectring);
  char* decstr = begdec;
  char* endstr = strchr (decstr, '\0');
  char* travel;

  int   decsign = 0;
  double deg, min, sec;

  deg = min = sec = 0.0;

  decsign = 1;

  /* rte modif to skip leading spaces */
  while (*decstr==' ')
    decstr++;

  if (decstr[0] == '-') {
    decsign = -1;
    decstr ++;
  }
  else if (decstr[0] == '+') {
    decsign = 1;
    decstr ++;
  }

  /* parse DEC degrees */
  if (sscanf (decstr, "%2lf", &deg) != 1)
    return -1;

  travel = strchr (decstr, ':');
  if (travel && ((travel-decstr) <= 2))
    decstr = travel +1;
  else
    decstr += 2;

  if (decstr >= endstr)
    return -1;

  /* parse DEC minutes (if specified) */
  if (sscanf (decstr, "%2lf", &min) != 1)
    return -1;

  travel = strchr (decstr, ':');
  if (travel && ((travel-decstr) <= 2))
    decstr = travel +1;
  else
    decstr += 2;
    
  /* parse DEC seconds (if specified) */
  if (decstr < endstr)
    if (sscanf (decstr, "%lf", &sec) != 1)
      return -1;

  *dec = (double) decsign * (deg + min/60.0 + sec/3600.0) * M_PI/180.0;

  /* printf ("DEC %g:%g:%g parsed -> %g radians\n", deg, min, sec, *dec); */
  free (begdec);
  return 0;
} 

/* takes an RA and DEC in radians and converts it to an hour angle and
   degree format, then outputs this in a 13 digit string */
int coord2name (char* pulsar, double ra, double dec)
{
  int hr, rmin, rsec, deg, dmin, dsec;
  int isign = dec / fabs(dec);
  char sign = (isign > 0)?'+':'-';

  dec *= 180.0 / M_PI;
  ra  *= 12.0 / M_PI;

  hr = (int) ra;
  ra -= (double) hr;
  ra *= 60;
  rmin = (int) ra;
  ra -= (double) rmin;
  ra *= 60;
  rsec = (int) ra;

  dec = fabs (dec);

  deg = (int) dec;
  dec -= (double) deg;
  dec *= 60;
  dmin = (int) dec;
  dec -= (double) dmin;
  dec *= 60;
  dsec = (int) dec;

  sprintf (pulsar, "%02d%02d%02d%c%02d%02d%02d", hr,rmin,rsec, sign,
	   deg,dmin,dsec);
  return 0;
}

/* **********************************************************************

   telescope_coords - given a TEMPO telescope code, returns the 
                      geodetic latitude and east longitude in degrees
		      as well as the elevation in metres.  If not 
		      interested in any of the values, simply pass NULL
		      in the call.

   ********************************************************************** */

int telescope_coords (int telescope, float* latitude, 
		      float* longitude, float* elevation)
{
  float latd=-999, longd=-999, elev=-1e9;
  
  switch (telescope) {

  case 2:
    /* ATCA Narrabri */
    latd = -30.31;
    longd = 149.57;
    elev = 217.0;
    break;

  case 3:
    /* Arecibo */
    latd = 18.345;
    longd = 293.247;
    elev = 496.0;
    break;
    
  case 5:
    /* Urumqi */
    latd = 43.47;
    longd = 87.1778;
    elev = 2029.3;

  case 6:
    /* Tidbinbilla */
    latd = -35.4;
    longd = 148.98;
    elev = 670.0;
    break;

  case 0: 
    fprintf(stderr, "telescope_coords: 0 -- assuming Parkes!\n");
  case 7:
    /* Parkes */
    latd = -33.0;
    longd = 148.2617;
    elev = 392.0;
    break;

  default:
    fprintf (stderr, "telescope_coords: unknown telescope code %d\n",
	     telescope);
    return -1;
  }

  if (latitude)
    *latitude = latd;
  if (longitude)
    *longitude = longd;
  if (elevation)
    *elevation = elev;

  return 0;
}

/* **********************************************************************

   az_zen_para - given the J2000 ra and dec, local sidereal time, and
                 latitude, returns the azimuth, zenith, and parallactic
		 angles of the telescope.

		 ra and dec are given in radians
		 lst is given in hours
		 latitude is given in degrees

		 para_angle, tel_az and tel_zen are returned in degrees

   Uses: SLALIB SUBROUTINE sla_ALTAZ (HA, DEC, PHI,
                           AZ, AZD, AZDD, EL, ELD, ELDD, PA, PAD, PADD)

   ********************************************************************** */

void F772C(sla_altaz) (double*, double*, double*, double*, double*, double*,
		 double*, double*, double*, double*, double*, double*);

int az_zen_para (double ra, double dec, float lst, float latitude,
		 float* tel_az, float* tel_zen, float* para_angle)
{
  double altitude, azimuth, PA;
  double ignore;

  double HA = lst * M_PI/12.0 - ra;     /* hour angle */
  double rad2deg = 180.0 / M_PI;
  double dlat = latitude / rad2deg;

  if (lst < 0.0 || lst > 24.0)
    return -1;

  F772C(sla_altaz) (&HA, &dec, &dlat, 
	      &azimuth, &ignore, &ignore,
	      &altitude,  &ignore, &ignore,
	      &PA,      &ignore, &ignore);

  *para_angle = (float) PA  * rad2deg;
  *tel_zen = 90.0 - (float) altitude * rad2deg;
  *tel_az = (float) azimuth * rad2deg;

  return 0;
}

