/* $Source: /cvsroot/psrchive/psrchive/Base/Formats/Timer/convert_endian.h,v $
   $Revision: 1.3 $
   $Date: 1999/11/02 09:58:21 $
   $Author: straten $ */

#include "machine_endian.h"

#include "timer.h"
#include "band.h"
#include "mini.h"

#ifdef __cplusplus
extern "C" {
#endif
void timerChangeEndian (struct timer * hdr);
void bandChangeEndian  (struct band *b);
void miniChangeEndian  (struct mini * m);
#ifdef __cplusplus
}
#endif

#ifdef MACHINE_LITTLE_ENDIAN
#define timer_toBigEndian(p)       timerChangeEndian(p)
#define timer_toLittleEndian(p)
#define timer_fromBigEndian(p)     timerChangeEndian(p)
#define timer_fromLittleEndian(p)

#define mini_toBigEndian(p)        miniChangeEndian(p)
#define mini_toLittleEndian(p)
#define mini_fromBigEndian(p)      miniChangeEndian(p)
#define mini_fromLittleEndian(p)

#define band_toBigEndian(p)        bandChangeEndian(p)
#define band_toLittleEndian(p)
#define band_fromBigEndian(p)      bandChangeEndian(p)
#define band_fromLittleEndian(p)
#else
#define timer_toBigEndian(p)
#define timer_toLittleEndian(p)    timerChangeEndian(p)
#define timer_fromBigEndian(p)
#define timer_fromLittleEndian(p)  timerChangeEndian(p)

#define mini_toBigEndian(p)
#define mini_toLittleEndian(p)     miniChangeEndian(p)
#define mini_fromBigEndian(p)
#define mini_fromLittleEndian(p)   miniChangeEndian(p)

#define band_toBigEndian(p)
#define band_toLittleEndian(p)     bandChangeEndian(p)
#define band_fromBigEndian(p)
#define band_fromLittleEndian(p)   bandChangeEndian(p)
#endif

