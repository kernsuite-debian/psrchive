#include "convert_endian.h"

void
bandChangeEndian(struct band *b)
{
	changeEndian(&b->lo1,8);
	changeEndian(&b->lo2,8);
	changeEndian(&b->loUP,8);
	changeEndian(&b->loDOWN,8);
	changeEndian(&b->centrefreq,8);
	changeEndian(&b->bw,8);
	changeEndian(&b->flux_A,4);
	changeEndian(&b->inv_mode,4);
	changeEndian(&b->auto_atten,4);
	changeEndian(&b->correlator_mode,4);
	changeEndian(&b->f_atten_A,4);
	changeEndian(&b->f_atten_B,4);
	changeEndian(&b->polar,4);
	changeEndian(&b->feed_offset,4);
	changeEndian(&b->nlag,4);
	changeEndian(&b->flux_B,4);
	changeEndian(&b->flux_err,4);
	changeEndian(&b->npol,4);
}

void timerChangeEndian(struct timer * hdr){
	changeEndian(&hdr->version,4);
	changeEndian(&hdr->minorversion,4);
	changeEndian(&hdr->tape_number,4);
	changeEndian(&hdr->file_number,4);
	changeEndian(&hdr->fracmjd,8);
	changeEndian(&hdr->mjd,4);
	changeEndian(&hdr->number_of_ticks,4);
	changeEndian(&hdr->offset,8);
	changeEndian(&hdr->lst_start,8);
	changeEndian(&hdr->ra,8);
	changeEndian(&hdr->dec,8);
	changeEndian(&hdr->l,4);
	changeEndian(&hdr->b,4);
	changeEndian(&hdr->nominal_period,8);
	changeEndian(&hdr->dm,4);
	changeEndian(&hdr->fold_true_ratio,4);
	changeEndian(&hdr->nperiods_long,4);
	changeEndian(&hdr->nperiods_short,4);
	changeEndian(&hdr->nbin,4);
	changeEndian(&hdr->tsmp,4);
	changeEndian(&hdr->sub_int_time,4);
	changeEndian(&hdr->ndump_sub_int,4);
	changeEndian(&hdr->narchive_int,4);
	changeEndian(&hdr->junk,4);
	changeEndian(&hdr->nsub_int,4);
	changeEndian(&hdr->junk2,4);
	changeEndian(&hdr->dump_time,4);
	changeEndian(&hdr->nfreq,4);
	changeEndian(&hdr->nsub_band,4);
	changeEndian(&hdr->feedmode,4);
	changeEndian(&hdr->nspan,4);
	changeEndian(&hdr->ncoeff,4);
	changeEndian(&hdr->nbytespoly,4);
	changeEndian(&hdr->nbytesephem,4);
	bandchangeEndian(&hdr->banda);
	bandchangeEndian(&hdr->bandb);
	changeEndian(&hdr->rotm,4);
	changeEndian(&hdr->rmi,4);
	changeEndian(&hdr->pnterr,4);
	changeEndian(&hdr->ibeam,4);
	changeEndian(&hdr->pos_angle,4);
	changeEndian(&hdr->headerlength,4);
	changeEndian(&hdr->corrected,4);
	changeEndian(&hdr->calibrated,4);
	changeEndian(&hdr->obstype,4);
	changeEndian(&hdr->wts_and_bpass,4);
	changeEndian(&hdr->wtscheme,4);
}

void 
miniChangeEndian(struct mini * m){
	changeEndian(&m->junk,4);
	changeEndian(&m->junk2,4);
	changeEndian(&m->junk3,4);
	changeEndian(&m->mjd,4);
	changeEndian(&m->fracmjd,8);
	changeEndian(&m->lst_start,8);
	changeEndian(&m->pfold,8);
	changeEndian(&m->tel_zen,4);
	changeEndian(&m->tel_az,4);
	changeEndian(&m->feed_ang,4);
	changeEndian(&m->para_angle,4);
	changeEndian(&m->version,4);
	changeEndian(&m->pulse_phase,4);
	changeEndian(&m->flux_A,4);
	changeEndian(&m->flux_B,4);
	changeEndian(&m->integration,8);
}
