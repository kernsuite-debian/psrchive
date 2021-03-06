/***************************************************************************
 *
 *   Copyright (C) 2006 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include "Pulsar/PhaseScale.h"
#include "Pulsar/Archive.h"
#include "Pulsar/Integration.h"

using namespace std;

Pulsar::PhaseScale::PhaseScale ()
{
  units = Phase::Turns;
  origin_norm = 0;
}

void Pulsar::PhaseScale::init (const Archive* data)
{
  if (!data)
    return;

  // PlotScale::num_indeces
  num_indeces = data->get_nbin();

  float scale = get_scale (data);

  float min = origin_norm;
  float max = 1 + origin_norm;

  min *= scale;
  max *= scale;

  set_minmax (min, max);
}

void Pulsar::PhaseScale::get_indeces (const Archive* data, 
				      unsigned& min, unsigned& max) const
{
  bool cyclic = true;
  PlotScale::get_indeces (data->get_nbin(), min, max, cyclic);
}


void Pulsar::PhaseScale::get_ordinates (const Archive* data,
					std::vector<float>& axis) const
{
  float scale = 1.0;
 
  if (data != 0) {
    axis.resize (data->get_nbin());
    scale = get_scale (data);
  }

  for (unsigned ibin = 0; ibin < axis.size(); ibin++)
    axis[ibin] = scale * (origin_norm + (float(ibin) + 0.5) / axis.size());
}


float Pulsar::PhaseScale::get_scale (const Archive* data) const
{
  double period_in_seconds = 1.0;
  if (data)
    period_in_seconds = data->get_Integration(0)->get_folding_period();

  switch (units)
    {
    case Phase::Turns:
      return 1.0;
      
    case Phase::Degrees:
      return 360.0;

    case Phase::Bins:
      return data->get_nbin();

    case Phase::Milliseconds:
      return period_in_seconds * 1e3;

    case Phase::Seconds:
      return period_in_seconds;

    case Phase::Minutes:
      return period_in_seconds / 60.0;

    case Phase::Hours:
      return period_in_seconds / 3600.0;

    case Phase::Radians:
      return 2.0 * M_PI;

    default:
      return 1.0;
    }
}

//! Get the default label for the x axis
std::string Pulsar::PhaseScale::get_label () const
{
  switch (units) {
  case Phase::Turns: return "Pulse Phase";
  case Phase::Degrees: return "Phase (deg.)";
  case Phase::Bins: return "Bin Number";
  case Phase::Milliseconds: return "Time (ms)";
  case Phase::Seconds: return "Time (s)";
  case Phase::Minutes: return "Time (min.)";
  case Phase::Hours: return "Time (hr.)";
  case Phase::Radians: return "Phase (rad.)";
  default: return "Unknown";
  }
}

