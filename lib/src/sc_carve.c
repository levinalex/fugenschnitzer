/*
sc_carve.c

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

#define SC_CARVE_INLINE_INTERN

#include <string.h>		// memcpy, memmove
#include "sc_defi.h"
#include "sc_color.h"
#include "sc_carve.h"
#include "sc_shift.h"

void delete_intn_ln(
	struct intn_s *intn_img, struct intn_s *diff_img, struct mark_s *mark_img,
	const seam_t s, const struct info_s *info, const long int i
) {
	diff_img->px[i][info->width] = INTN_INFTY;

	intn_t *ipx = intn_img->px[i];

	const int l = (info->width - s);

	memmove(
		ipx + s,
		ipx + s + 1,
		l * sizeof(intn_t)
	);
	if (info->flags.mark) {
		mark_t *mpx = mark_img->px[i];
		memmove(
			mpx + s,
			mpx + s + 1,
			l * sizeof(mark_t)
		);
	}
}

/******************************************************************************/

void save_px(
	struct comp_s *image, comp_t *tmp,
	const struct seams_s *seams,
	const long int first, const long int n, const long int i
) {
	for (int k = 0; k < n; k++) {
		const seam_t s = seam_coord(seams, k + first, i);
		const int t = n - k - 1;
		tmp[t] = image->px[i][s];
	}
}

void restore_px(
	struct comp_s *image, comp_t *tmp,
	const struct seams_s *seams,
	const int first, const int n, const int i
) {
	for (int k = 0; k < n; k++) {
		const seam_t s = seam_coord(seams, k + first, i);
		const int t = n - k - 1;
		image->px[i][s] = tmp[t];
	}
}

/******************************************************************************/

void delete_comp_ln(
	struct comp_s *image, struct seams_s *seams, const struct info_s *info,
	const long int i, const long int last, const bool origin,
	comp_t *image_tmp, seam_t *seams_tmp
) {
	const int sc = origin? 0: info->sc,
			   n = last - sc,
			 end = info->original_width - last;

//	px: image line
	comp_t *px = image->px[i];
//	back up pixel to delete
	save_px(image, image_tmp, seams, sc, n, i);
//	sort
	sort_seams_ln_paral(seams, seams_tmp, info->original_width - sc, sc, n, i);
//	initialize
	seam_t *seam = seams_tmp;

	for (int k = 0; k < n; k++) {
//		calculate source and target
		const seam_t s = *seam + 1,
					 t = *seam - k,
					 l = *(seam + 1) - s;
//		move
		memmove(px + t, px + s, l * sizeof(comp_t));
		seam++;
	}
//	append backed up pixels
	memcpy(px + end, image_tmp, n * sizeof(comp_t));
//	left-shift seams
	lshift_seams_ln(seams, info, sc, last, i);
}

void undelete_comp_ln(
	struct comp_s *image, struct seams_s *seams, const struct info_s *info,
	const long int i, const long int first, const bool origin,
	comp_t *image_tmp, seam_t *seams_tmp
) {
	const int first_sc = origin? 0: first,
					 n = info->sc - first_sc,
				   end = info->original_width - info->sc;

//	px: image line
	comp_t *px = image->px[i];
//	back up pixel to delete
	memcpy(image_tmp, px + end, n * sizeof(comp_t));
//	sort
	sort_seams_ln_paral(seams, seams_tmp, end, first_sc, n, i);
//	initialize
	seam_t *seam = seams_tmp + n;

	for (int k = n; k > 0; k--) {
		seam--;
//		calculate source and target
		const seam_t s = *seam,
					 t = s + k,
					 l = *(seam + 1) - s;
//		move
		memmove(px + t, px + s, l * sizeof(comp_t));
	}
//	right-shift seams back
	rshift_seams_ln(seams, info, first_sc, info->sc, i);
//	insert backed up pixels
	restore_px(image, image_tmp, seams, first_sc, n, i);
}

//	linear interpol. for (s != 0) && info->flags.interpol else copy value
extern comp_t interpol_px(comp_t *c, const seam_t s, const struct info_s *info);
inline comp_t interpol_px(comp_t *c, const seam_t s, const struct info_s *info)
	{return (s && info->flags.interpol)? comp_interpol(c + s, info): c[s];}

void insert_comp_ln(
	struct comp_s *image, struct seams_s *seams, const struct info_s *info,
	const long int i, const long int last, const bool origin,
	comp_t *image_tmp, seam_t *seams_tmp
) {
//	caution: info->sc is negative.
	const int sc = origin? 0: info->sc,
			   n = last + sc,
			 end = info->original_width + last - 1;
//	px: image line
	comp_t *px = image->px[i];
//	sort
	sort_seams_ln_paral(seams, seams_tmp, info->original_width - sc, -sc, n, i);
// initialize
	seam_t *seam = seams_tmp + n;

	for (int k = n; k > 0; k--) {
//		calculate source and target
		seam--;
		const seam_t s = *seam,
					 t = s + k,
					 l = *(seam + 1) - s;

//		interpolate
		const comp_t c = interpol_px(px, s, info);
//		move
		memmove(px + t, px + s, l * sizeof(comp_t));
//		insert
		px[t - 1] = c;
	}
//	right-shift seams
	rshift_seams_ln(seams, info, -sc, last, i);
}

void uninsert_comp_ln(
	struct comp_s *image, struct seams_s *seams, const struct info_s *info,
	const long int i, const long int first, const bool origin,
	comp_t *image_tmp, seam_t *seams_tmp
) {
//	caution: info->sc is negative.
	const int first_sc = origin? 0: first,
					 n = -info->sc - first_sc;
//	px: image line
	comp_t *px = image->px[i];
//	sort
	sort_seams_ln_paral(seams, seams_tmp, info->original_width - info->sc, first_sc, n, i);
//	initialize
	seam_t *seam = seams_tmp;
	for (int k = 0; k < n; k++) {
//		calculate source and target
		const seam_t s = *seam + 1,
					 t = *seam - k,
					 l = *(seam + 1) - s;
//		move
		memmove(px + t, px + s, l * sizeof(comp_t));
		seam++;
	}
//	left-shift seams back
	lshift_seams_ln(seams, info, first_sc, -info->sc, i);
}

/******************************************************************************/

/*
void undelete_mark_ln(
	struct mark_s *image, struct seams_s *seams, const struct info_s *info,
	const int i, const bool restore
) {

	const bool ext = (info->sc < 0);
	const int first = (restore || ext)? 0: info->sc,
			   last = minz(info->sc),
				  n = info->si - first;

	mark_t *px = image->px[i];

//	Fugen sc -> si
	if (ext)
		clear_shift_ln(seams, info, i);
	lshift_seams_ln(seams, info, last, info->si, i);


//	Markierung si -> ((restore || ext)? 0: sc)
//	const int first = (restore || ext)? 0: info->sc,
//				  n = info->si - first;

	seams->tmp[n] = info->original_width - last;
	sort_seams_ln(seams, first, n, i);
	seam_t *seam = seams->tmp + n;
	for (int k = n; k > 0; k--) {
		seam--;
//		Quelle und Ziel ausrechnen
		const seam_t s = *seam,
					 t = s + k,
					 l = *(seam + 1) - s;
//		Verschieben
		memmove(px + t, px + s, l * sizeof(mark_t));
		px[t - 1] = 0;
	}

//	Fugen si -> sc
	if (ext) {
		clear_shift_ln(seams, info, i);
		irshift_seams_ln(seams, info, 0, -info->sc, i);
	} else
		rshift_seams_ln(seams, info, last, info->si, i);
}

void insert_mark_ln(
	struct mark_s *image, struct seams_s *seams, const struct info_s *info,
	const int i, const bool restore
) {
	if ((info->sc >= 0) || restore) return;
//	Achtung: info->sc ist negativ!
	const int n = -info->sc;
//	px: Bildzeile
	mark_t *px = image->px[i];
//	Sortieren
	sort_seams_ln_noshift(seams, 0, n, i);
//	Rechter Rand
    seams->tmp[n] = info->original_width;

// Initialisieren
	seam_t *seam = seams->tmp + n;

	for (int k = n; k > 0; k--) {
//		Quelle und Ziel ausrechnen
		seam--;
		const seam_t s = *seam,
					 t = s + k,
					 l = *(seam + 1) - s;
//		Verschieben
		memmove(px + t, px + s, l * sizeof(mark_t));
		px[t - 1] = 0;
	}
}
*/
/******************************************************************************/
/*
void fix_shift_comp(
	struct comp_s *image, struct seams_s *seams, const struct info_s *info,
	const bool restore
) {
	if (!info->sc || !restore)
			return;

	for (int i = 0; i < info->height; i++)
		if (info->sc > 0) {
			undelete_comp_ln(image, seams, info, i, 0);
			lshift_seams_ln(seams, info, 0, info->sc, i);
		} else {
			uninsert_comp_ln(image, seams, info, i, 0);
			irshift_seams_ln(seams, info, 0, -info->sc, i);
		}
}

void fix_shift_mark(
	struct mark_s *image, struct seams_s *seams, const struct info_s *info,
	const bool restore
) {
	if (!info->flags.mark || !info->sc || !restore) return;

    for (int i = 0; i < info->height; i++) {
		undelete_mark_ln(image, seams, info, i, restore);
		insert_mark_ln(image, seams, info, i, restore);
	}
}
*/

/******************************************************************************/

typedef void (*prevw_py_rgb_func_t)(
	uint8_t *img_out, const struct comp_s *image, const struct info_s *info,
	const long int i, const int ch, const int d
);

// The preview functions are in sc_color.

typedef void (*carve_func_t)(
	struct comp_s *image, struct seams_s *seams, const struct info_s *info,
	const long int i, const long int last, const bool origin,
	comp_t *image_tmp, seam_t *seams_tmp
);

const carve_func_t carve_func[2][2] = {
	{delete_comp_ln, undelete_comp_ln},
	{insert_comp_ln, uninsert_comp_ln}
};

void carve_comp_part(
	struct comp_s *image, struct seams_s *seams, struct info_s *info,
	const long int last, const bool origin, const bool insert, const bool reverse,
	const long int vfirst, const int long vlast,
	comp_t *image_tmp, seam_t *seams_tmp, int *progress
) {
	carve_func_t        carve_ln = carve_func[insert][reverse],
					  restore_ln = carve_func[!insert][true];

	for (int i = vfirst; i < vlast; i++) {
			if (origin)
				restore_ln(image, seams, info, i, 0, origin, image_tmp, seams_tmp);
			carve_ln(image, seams, info, i, last, origin, image_tmp, seams_tmp);
		(*progress)++;
	}
}

long int carve_comp_finish(struct info_s *info, const long int last, const bool insert)
	{return -(info->sc = insert? -last: last);}

void carve_comp(
	struct comp_s *image, struct seams_s *seams, struct info_s *info,
	const long int last, const bool insert, const bool reverse, int *progress
) {
	carve_comp_part(
		image, seams, info, last, false, insert, reverse, 0, info->height,
		image->tmp, seams->tmp, progress
	);
	carve_comp_finish(info, last, insert);
}

void carve_comp_py_rgb_part(
	uint8_t *img_out, struct comp_s *image, struct seams_s *seams,
	struct info_s *info, const bool zoom, const long int last, const bool origin,
	const bool insert, const bool reverse, const long int vfirst, const long int vlast,
	comp_t *image_tmp, seam_t *seams_tmp, int *progress, const bool rgb32
) {
	const int
		ch = 3 + rgb32,
		z = zoom? info->zoom: 1,
		d = z * z;

	carve_func_t        carve_ln = carve_func[insert][reverse],
					  restore_ln = carve_func[!insert][true];
	prevw_py_rgb_func_t prevw_ln = zoom?
		zoom_comp_ln_py_rgb: eject_comp_ln_py_rgb;

	for (int I = vfirst; I < vlast; I++) {
		const int pln = I * z;
		for (int i = pln; i < pln + z; i++) {
			if (origin)
				restore_ln(image, seams, info, i, 0, origin, image_tmp, seams_tmp);
			carve_ln(image, seams, info, i, last, origin, image_tmp, seams_tmp);
		}

		prevw_ln(img_out, image, info, I, ch, d);
		(*progress)++;
	}
}

long int carve_comp_py_rgb_finish(
	struct comp_s *image, struct seams_s *seams, struct info_s *info,
	const bool zoom, const long int last, const bool origin,
	const bool insert, const bool reverse
) {
	const int z = zoom? info->zoom: 1;
	carve_func_t        carve_ln = carve_func[insert][reverse],
					  restore_ln = carve_func[!insert][true];

	if (zoom && info->zoom)
		for (int i = info->pheight * z; i < info->height; i++) {
			if (origin)
				restore_ln(image, seams, info, i, 0, origin, image->tmp, seams->tmp);
			carve_ln(image, seams, info, i, last, origin, image->tmp, seams->tmp);
		}

	return -(info->sc = insert? -last: last);
}

void carve_comp_py_rgb(
	uint8_t *img_out, struct comp_s *image, struct seams_s *seams,
	struct info_s *info, const bool zoom,
	const long int last, const bool origin, const bool insert, const bool reverse,
	int *progress, const bool rgb32
) {
	carve_comp_py_rgb_part(
		img_out, image, seams, info, zoom, last, origin, insert, reverse,
		0, zoom? info->pheight: info->height, image->tmp, seams->tmp,
		progress, rgb32
	);
	carve_comp_py_rgb_finish(
		image, seams, info, zoom, last, origin, insert, reverse
	);
}