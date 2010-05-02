/*
seamcarv.c

Diese Quelltextdatei ist Bestandteil der FUGENSCHNITZER-Programmbibliothek.

Die FUGENSCHNITZER-Programmbibliothek ist eine Seam-Carving-Programmbibliothek.
FUGENSCHNITZER -- Seam Carving fuer jedermann.
http://fugenschnitzer.sourceforge.net
Copyright (C) 2008/9 David Eckardt

Dieses Programm ist freie Software. Sie koennen es unter den Bedingungen
der GNU Lesser General Public License, wie von der Free Software
Foundation veroeffentlicht, weitergeben und/oder modifizieren, entweder
gemaess Version 3 der Lizenz oder (nach Ihrer Option) jeder spaeteren
Version.
Die Veroeffentlichung dieses Programms erfolgt in der Hoffnung, dass es
Ihnen von Nutzen sein wird, aber OHNE IRGENDEINE GARANTIE, sogar ohne
die implizite Garantie der MARKTREIFE oder der VERWENDBARKEIT FUER EINEN
BESTIMMTEN ZWECK. Details finden Sie in der GNU General Public License.
Sie sollten ein Exemplar der GNU General Public License zusammen mit
diesem Programm erhalten haben. Falls nicht, siehe
http://www.gnu.org/licenses/lgpl-3.0.html
http://www.gnu.org/licenses/gpl-3.0.html
http://www.gnu.de/documents/lgpl-3.0.de.html
http://www.gnu.de/documents/gpl-3.0.de.html
.

This source code file is a part of the FUGENSCHNITZER Program Library.

The FUGENSCHNITZER Program Library is a Seam Carving program library.
FUGENSCHNITZER -- Seam Carving for everyone.
http://fugenschnitzer.sourceforge.net
Copyright (C) 2008/9 David Eckardt

This program is free software: you can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.
This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
Public License for more details.
You should have received a copy of the GNU General Public License along
with this program. If not, see
http://www.gnu.org/licenses/lgpl-3.0.html
http://www.gnu.org/licenses/gpl-3.0.html
.

*/

#include <stdlib.h>		// free: sc_close
#include <stdbool.h>
#include "sc_defi.h"
#include "seamcarv.h"
#include "sc_mgmnt.h"
#include "sc_carve.h"
#include "sc_shift.h"
#include "sc_core.h"
#include "sc_color.h"

static struct info_s info;
static struct intn_s intn_img, diff_img;
static struct comp_s comp_img;
static struct mark_s mark_img;
static struct seams_s seams;

static intn_t *intn_pal;
static void *heap;
static bool halted;

LIB_PUBLIC void sc_init(void) {
	intn_img.px = NULL;
	intn_img.data = NULL;
	diff_img.px = NULL;
	diff_img.data = NULL;
	mark_img.px = NULL;
	mark_img.data = NULL;
	seams.data = NULL;
	seams.coord = NULL;
	seams.shift = NULL;
	comp_img.px = NULL;
	comp_img.data = NULL;
}

LIB_PUBLIC void sc_close(void) {
	free(intn_img.px);
	free(intn_img.data);
	free(diff_img.px);
	free(diff_img.data);

	free(mark_img.px);
	free(mark_img.data);

	free(seams.coord);
	free(seams.shift);
	free(seams.data);

	free(comp_img.px);
	free(comp_img.data);

	sc_init();
}

LIB_PUBLIC bool sc_load(
	const comp_t *image, long int *width, long int *height, const int zoom
) {
	info.flags.transposed = false;
	info.flags.mark = IMARK_NONE;

	create_info(&info, *height, *width, zoom? zoom: 1);
	*height = info.pheight;
	*width = info.pwidth;

	bool e = false;
	e |= create_intn_img(&intn_img, &info);
	e |= create_diff_img(&heap, &diff_img, &info);
	e |= create_mark_img(&mark_img, &info);
	e |= create_seams(&seams, &info);
	e |= create_comp_img(&comp_img, &info);

	load_comp(&comp_img, image, &info);

	return e;
}

LIB_PUBLIC bool sc_load_py_rgb(
	const uint8_t *image, long int *width, long int *height, const int zoom,
	const bool rgb32
) {
	info.flags.transposed = false;
	info.flags.mark = IMARK_NONE;

	create_info(&info, *height, *width, zoom? zoom: 1);

	*height = info.pheight;
	*width = info.pwidth;

	bool e = false;
	e |= create_intn_img(&intn_img, &info);
	e |= create_diff_img(&heap, &diff_img, &info);
	e |= create_mark_img(&mark_img, &info);
	e |= create_seams(&seams, &info);
	e |= create_comp_img(&comp_img, &info);

	load_comp_py_rgb(&comp_img, image, &info, rgb32);

	return e;
}

LIB_PUBLIC void sc_preview(comp_t *image) {
	preview_comp(image, &comp_img, &info);
}

LIB_PUBLIC void sc_preview_py_rgb(
	uint8_t *image, const bool zoom, const bool rgb32
) {
	preview_comp_py_rgb(image, &comp_img, &info, zoom, rgb32);
}

/*
 *	Return value:
 *
 *	0:	The image has not been extended and image data have not been moved.
 *		The mask has been loaded if requested.
 *	1:	The image has been extended and image data have been moved.
 *	2:	Although requestedm the mas has not been loaded. Only possible with
 *		alpha mask.
 *	3:	1 + 2
 *	-1:	Fatal error: Memory allocation failed.
 */

LIB_PUBLIC int sc_prepare(
	const bool vertical, const long int extend, const bool interpol,
	const enum mark_e mmode, const mark_t *mark,
	long int *width, long int *height, long int *pwidth, long int *pheight
) {
	bool mark_ret = false;
/*	switch (mmode) {
		default: break;
		case MARK_LOAD:
		case MARK_ALPHA:
			mark_ret = load_mark(
				&mark_img, mark, &comp_img, &info
			); break;
		case MARK_CLEAR:
            info.flags.mark = IMARK_NONE; break;
	}*/

	info.flags.interpol = interpol;

    halted = false;
	const bool transpose = ((vertical != false) ^ info.flags.transposed) & 1;

	if (transpose)
        transpose_images(
			&comp_img, &mark_img, &intn_img, &diff_img,
			&seams, &info, heap
		);

	const int e = extend_images(&comp_img, &mark_img, &info, extend);
	set_dimensions(&info, height, width, pheight, pwidth);

	if (!info.si)
		rgb_to_intn(&intn_img, &comp_img, &info);

	return e >= 0? (e | ((int)mark_ret * 2)): e;
}

/*
 *	Return value:
 *	 0: The image has not been extended.
 *	 1: The image has been extended.
 *	-1: Fatal error: Memory allocation failed.
 */
LIB_PUBLIC int sc_extend(
	const long int extend,
	long int *width, long int *height, long int *pwidth, long int *pheight
) {
	if (!info.si) return true;

	int e = extend_images(&comp_img, &mark_img, &info, extend);
	set_dimensions(&info, height, width, pheight, pwidth);

	return e;
}


LIB_PUBLIC bool sc_seam(long int last) {
//	Bildgroessenaenderung um max. Originalgroesse - 3 Pixel
	last = labs(last);
	const int seams_max = info.original_width - 3;
	const int n = min2_int(last, seams_max) - info.si;
	if (n <= 0) return (n < 0);
	halted = false;

	prepare_seams(&seams, &info);
	for (int k = 0; k < n; k++) {
		if (halted) break;
		diff(&diff_img, &intn_img, &info);
		mark(&diff_img, &mark_img, &info);
		accu(&diff_img, &info);
		trace_seam(&diff_img, &intn_img, &mark_img, seams.coord[info.si], &info);
	}
	restore_seams(&seams, &info);

	halted |= (last >= seams_max);
	return (halted);
}

LIB_PUBLIC long int sc_seam_progress(void) {
	return halted? -info.si: info.si;
}

LIB_PUBLIC void sc_seam_cancel(void) {
	halted = true;
}

static int c_progress;

LIB_PUBLIC long int sc_carve_progress(void)
	{return c_progress;}


LIB_PUBLIC long int sc_carve(
	long int nom,
	long int *width, long int *height, long int *pwidth, long int *pheight
) {
//	enlarging impossible if image is not extended
	if ((nom > 0) && (info.original_width == info.ext_width))
		nom = 0;
//	resizing at most as wide as seams computed
	if (abs(nom) > info.si)
	    nom = (nom > 0)? info.si: -info.si;

	const int act = -info.sc;

	if (nom == act) return nom; // nothing to do

	const bool enlarge = (nom > act),
				extend = ((nom > 0) || (act > 0)),
				  orig = (abs(nom - act) > max2(abs(nom), abs(act)));
//	orig: resizing from shrinked to enlarged or opposite

	if (orig) {
			carve_comp(
				&comp_img, &seams, &info, 0, !enlarge, true, &c_progress
			);
			carve_comp(
				&comp_img, &seams, &info, enlarge? +nom: -nom,
				enlarge, false, &c_progress
			);
	} else
		carve_comp(
			&comp_img, &seams, &info, extend? +nom: -nom,
			extend, extend ^ enlarge, &c_progress
		);

	set_dimensions_nom(&info, height, width, pheight, pwidth, nom);

	return nom;
}

LIB_PUBLIC long int sc_carve_py_rgb(
	uint8_t *image, long int nom,
	long int *width, long int *height, long int *pwidth, long int *pheight,
	const bool zoom, const bool rgb32
) {
//	enlarging impossible if image is not extended
	if ((nom > 0) && (info.original_width == info.ext_width))
		nom = 0;
//	resizing at most as wide as seams computed
	if (abs(nom) > info.si)
	    nom = (nom > 0)? info.si: -info.si;

	const int act = -info.sc;

	if (nom == act) return nom;  // nothing to do

	const bool enlarge = (nom > act),
				extend = ((nom > 0) || (act > 0)),
				origin = (abs(nom - act) > max2(abs(nom), abs(act)));
//	orig: resizing from shrinked to enlarged or opposite

	if (origin)
			carve_comp_py_rgb(
				image, &comp_img, &seams, &info, zoom && info.zoom,
				enlarge? +nom: -nom, origin,
				enlarge, false, &c_progress, rgb32
			);
	else
		carve_comp_py_rgb(
			image, &comp_img, &seams, &info, zoom && info.zoom,
			extend? +nom: -nom, origin, extend, enlarge ^ extend,
			&c_progress, rgb32
		);
	set_dimensions_nom(&info, height, width, pheight, pwidth, nom);

 	c_progress = -1;

	return nom;
}

LIB_PUBLIC bool sc_fix(
//	comp_t *image, mark_t *mark,
	const bool restore,
	long int *width, long int *height, long int *pwidth, long int *pheight
) {
	if (!info.si) return false;

	if (restore) {
		int p = 0;
		carve_comp(&comp_img, &seams, &info, 0, info.sc < 0, true, &p);
	}

    fix_align_comp(&comp_img, &info, restore);

	create_info(&info, info.height, info.original_width - info.sc, info.zoom);

	bool e = false;
	e |= create_intn_img(&intn_img, &info);
	e |= create_diff_img(&heap, &diff_img, &info);
	e |= create_mark_img(&mark_img, &info);
	e |= create_seams(&seams, &info);

	set_dimensions_nom(&info, height, width, pheight, pwidth, 0);

	return e;
}

LIB_PUBLIC void sc_eject_py_rgb(uint8_t *image, const bool rgb32) {
    eject_comp_py_rgb(image, &comp_img, &info, rgb32);
}

LIB_PUBLIC void sc_eject(comp_t *image) {
    eject_comp(image, &comp_img, &info);
}

LIB_PUBLIC void sc_display_seams_py_rgb(
	uint8_t *image, int first, int last, const bool zoom,
	const uint8_t color[], const bool rgb32
) {
	first = labs(first);
	last = labs(last);
	display_seams_py_rgb(image, &seams, &info, zoom, first, last, color, rgb32);
}

/*
LIB_PUBLIC void sc_show_mark(mark_t *mark) {
    preview_mark(mark, &mark_img, &info);
}

LIB_PUBLIC void sc_eject_mark(mark_t *mark) {
    eject_mark(mark, &mark_img, &info);
}

LIB_PUBLIC void sc_clear_mark(void) {
	info.flags.mark = IMARK_NONE;
}
*/
/******************************************************************************/

struct {
	int nom, lines, rest, plines, prest, threads, *progress;
	bool enlarge, extend, origin, zoom, rgb32;
} cm;

LIB_PUBLIC int sc_carve_paral_init(long int nom, const int threads)
	{return sc_carve_py_rgb_paral_init(nom, threads, false, false);}

LIB_PUBLIC void sc_carve_paral(const int thread) {
	const int vfirst =  cm.lines * thread;

	comp_t *image_tmp = malloc(info.maxhw * sizeof(comp_t));
	seam_t *seams_tmp = malloc(info.maxhw * sizeof(seam_t));

	if (cm.origin)
		carve_comp_part(
			&comp_img, &seams, &info,
			cm.enlarge? +cm.nom: -cm.nom,
			cm.origin, cm.enlarge, false,
			vfirst, vfirst + cm.lines,
			image_tmp, seams_tmp, cm.progress + thread
		);
	else
		carve_comp_part(
			&comp_img, &seams, &info,
			cm.extend? +cm.nom: -cm.nom,
			cm.origin, cm.extend, cm.enlarge ^ cm.extend,
			vfirst, vfirst + cm.lines,
			image_tmp, seams_tmp, cm.progress + thread
		);

	free(image_tmp);
	free(seams_tmp);
}

LIB_PUBLIC long int sc_carve_paral_finish(
	long int *width, long int *height, long int *pwidth, long int *pheight
) {

	const int vfirst =  cm.lines * cm.threads;

	if (cm.origin) {
		carve_comp_part(
			&comp_img, &seams, &info,
			cm.enlarge? +cm.nom: -cm.nom,
			cm.origin, cm.enlarge, false,
			vfirst, vfirst + cm.rest,
			comp_img.tmp, seams.tmp, cm.progress
		);
		carve_comp_finish(&info, cm.enlarge? +cm.nom: -cm.nom, cm.enlarge);
	} else {
		carve_comp_part(
			&comp_img, &seams, &info,
			cm.extend? +cm.nom: -cm.nom,
			cm.origin, cm.extend, cm.enlarge ^ cm.extend,
			vfirst, vfirst + cm.rest,
			comp_img.tmp, seams.tmp, cm.progress
		);
		carve_comp_finish(&info, cm.extend? +cm.nom: -cm.nom, cm.extend);
	}
	cm.threads = 0;
	free(cm.progress);

	long int nom = -info.sc;

	set_dimensions_nom(&info, height, width, pheight, pwidth, nom);

	return nom;
}

LIB_PUBLIC long int sc_carve_paral_progress(void) {
	if (!cm.threads)
		return -1;

	int p = 0;
	for (int i = 0; i < cm.threads; i++)
		p += cm.progress[i];

	return p;
}

LIB_PUBLIC int sc_carve_py_rgb_paral_init(
	long int nom, const int threads, const bool zoom, const bool rgb32
) {
//	enlarging impossible if image is not extended
	if ((nom > 0) && (info.original_width == info.ext_width))
		nom = 0;
//	resizing at most as wide as seams computed
	if (abs(nom) > info.si)
	    nom = (nom > 0)? info.si: -info.si;

	const int act = -info.sc;

//	orig: resizing from shrinked to enlarged or opposite
	const bool enlarge = (nom > act),
				extend = ((nom > 0) || (act > 0)),
				origin = (abs(nom - act) > max2(abs(nom), abs(act)));

	cm.enlarge  = enlarge;
	cm.extend   = extend;
	cm.origin   = origin;
	cm.zoom     = zoom && info.zoom;
	cm.rgb32    = rgb32;
	cm.nom      = nom;
	cm.threads  = threads;
	cm.plines   = (cm.zoom? info.pheight: info.height) / cm.threads;
	cm.lines    = cm.zoom? (cm.plines * info.zoom): cm.plines;
	cm.prest    = (cm.zoom? info.pheight: info.height) % cm.threads;
	cm.rest     = cm.zoom? (cm.prest * info.zoom): cm.prest;
	cm.progress = malloc(cm.threads * sizeof(*cm.progress));
	for (int i = 0; i < cm.threads; i++)
		cm.progress[i] = 0;

	return cm.zoom? info.pheight: info.height;
}

LIB_PUBLIC void sc_carve_py_rgb_paral(uint8_t *image, const int thread) {
	const int vfirst =  cm.lines * thread,
			 pvfirst = cm.plines * thread;

	comp_t *image_tmp = malloc(info.maxhw * sizeof(comp_t));
	seam_t *seams_tmp = malloc(info.maxhw * sizeof(seam_t));

	if (cm.origin)
		carve_comp_py_rgb_part(
			image, &comp_img, &seams, &info,
			cm.zoom, cm.enlarge? +cm.nom: -cm.nom,
			cm.origin, cm.enlarge, false,
			pvfirst, pvfirst + cm.plines,
			image_tmp, seams_tmp, cm.progress + thread, cm.rgb32
		);
	else
		carve_comp_py_rgb_part(
			image, &comp_img, &seams, &info,
			cm.zoom, cm.extend? +cm.nom: -cm.nom,
			cm.origin, cm.extend, cm.enlarge ^ cm.extend,
			pvfirst, pvfirst + cm.plines,
			image_tmp, seams_tmp, cm.progress + thread, cm.rgb32
		);

	free(image_tmp);
	free(seams_tmp);
}

LIB_PUBLIC long int sc_carve_py_rgb_paral_finish(
	uint8_t *image,
	long int *width, long int *height, long int *pwidth, long int *pheight
) {

	const int vfirst =  cm.lines * cm.threads,
			 pvfirst = cm.plines * cm.threads;

	if (cm.origin) {
		carve_comp_py_rgb_part(
			image, &comp_img, &seams, &info,
			cm.zoom, cm.enlarge? +cm.nom: -cm.nom,
			cm.origin, cm.enlarge, false,
			pvfirst, pvfirst + cm.prest,
			comp_img.tmp, seams.tmp, cm.progress, cm.rgb32
		);
		carve_comp_py_rgb_finish(
			&comp_img, &seams, &info,
			cm.zoom, cm.enlarge? +cm.nom: -cm.nom,
			cm.origin, cm.enlarge, false
		);
	} else {
		carve_comp_py_rgb_part(
			image, &comp_img, &seams, &info,
			cm.zoom, cm.extend? +cm.nom: -cm.nom,
			cm.origin, cm.extend, cm.enlarge ^ cm.extend,
			pvfirst, pvfirst + cm.prest,
			comp_img.tmp, seams.tmp, cm.progress, cm.rgb32
		);
		carve_comp_py_rgb_finish(
			&comp_img, &seams, &info,
			cm.zoom, cm.extend? +cm.nom: -cm.nom,
			cm.origin, cm.extend, cm.enlarge ^ cm.extend
		);
	}
	cm.threads = 0;
	free(cm.progress);

	long int nom = -info.sc;

	set_dimensions_nom(&info, height, width, pheight, pwidth, nom);

	return nom;
}

/******************************************************************************/

static int parts, sm_lines, sm_vrest,
	sm_threads, sm_cols,  sm_hrest,
	sm_vlast, sm_hlast;

LIB_PUBLIC void sc_seam_paral_init(void) {
	prepare_seams(&seams, &info);
}

LIB_PUBLIC int sc_seam_paral_start(long int samples, int threads) {
	if (!samples)
	    samples = 0x10000;

	sm_lines = (samples / info.width);
	parts = info.height / sm_lines;
	sm_vrest = info.height % sm_lines;
	if (!sm_vrest)
	    sm_lines--;

	if (!threads)
	    threads = 1;
	sm_threads = threads;

	sm_cols  = info.width / sm_threads;
	sm_hrest = info.width % sm_threads;

	sm_vlast = info.height - 1;
	sm_hlast = info.width - 1;

	diff_margins(&diff_img, &intn_img, &info);

	return parts + 1;
}

// #include <stdio.h>

LIB_PUBLIC void sc_seam_paral_diff(const int part, const int thread) {
	const bool vend = (part >= parts),
			   hend = ((thread + 1) >= sm_threads);

	int vfirst = part * sm_lines,
		 vlast = vfirst + (vend? (sm_vrest - 1): sm_lines),

		hfirst = thread? (thread * sm_cols): 1,
		 hlast = hfirst + sm_cols - ((thread == 0) || (!sm_hrest && hend));

	vfirst += !part;
	hlast += !hend;

//	printf("%s: %d %d\n", __func__, vfirst, vlast);

	for (int i = vfirst; i < vlast; i++)
		diff_ln(&diff_img, &intn_img, i, hfirst, hlast);
}

void sc_seam_diff_hrest(const long int vfirst, const long int vlast) {
	const int hfirst = sm_cols * sm_threads + (sm_cols == 0);

	if (hfirst >= sm_hlast) return;

   	for (int i = vfirst; i < vlast; i++)
	   	diff_ln(&diff_img, &intn_img, i, hfirst, sm_hlast);
}

LIB_PUBLIC bool sc_seam_paral_accu(const int part) {
	const bool start = (part == 0),
				 end = (part >= parts);
	int vfirst = part * sm_lines,
		 vlast = end? sm_vlast: vfirst + sm_lines;

	sc_seam_diff_hrest(vfirst + start, vlast);

	vfirst -= !start;
	vlast  -= !end;

	if (info.flags.mark)
		for (int i = vfirst; i < vlast; i++) {
			mark_ln(&diff_img, &mark_img, i, 0, info.width);
			accu_ln(&diff_img, i, sm_hlast);
		}
	else
		for (int i = vfirst; i < vlast; i++)
			accu_ln(&diff_img, i, sm_hlast);

	return (parts == part);
}

LIB_PUBLIC long int sc_seam_paral_finish(void) {
	trace_seam(&diff_img, &intn_img, &mark_img, seams.coord[info.si], &info);
	return info.si;
}

LIB_PUBLIC void sc_seam_paral_close(void) {
	restore_seams(&seams, &info);
}