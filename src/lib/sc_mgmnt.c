/*
sc_mgmnt.c

Diese Quelltextdatei ist Bestandteil der FUGENSCHNITZER-Programmbibliothek.
Die FUGENSCHNITZER-Programmbibliothek untersteht der
GNU Lesser General Public Licence (Version 3):
http://www.gnu.org/licenses/lgpl-3.0.html
http://www.gnu.org/licenses/gpl-3.0.html
http://www.gnu.de/documents/lgpl-3.0.de.html
http://www.gnu.de/documents/gpl-3.0.de.html

Die FUGENSCHNITZER-Programmbibliothek ist eine Seam-Carving-Programmbibliothek.
FUGENSCHNITZER -- Seam Carving fuer jedermann.

Dieses Programm ist freie Software. Sie koennen es unter den Bedingungen der GNU
General Public License, wie von der Free Software Foundation veroeffentlicht,
weitergeben und/oder modifizieren, entweder gemaess Version 3 der Lizenz oder
(nach Ihrer Option) jeder spaeteren Version.
Die Veroeffentlichung dieses Programms erfolgt in der Hoffnung, dass es Ihnen von
Nutzen sein wird, aber OHNE IRGENDEINE GARANTIE, sogar ohne die implizite
Garantie der MARKTREIFE oder der VERWENDBARKEIT FUER EINEN BESTIMMTEN ZWECK.
Details finden Sie in der GNU General Public License.
Sie sollten ein Exemplar der GNU General Public License zusammen mit diesem
Programm erhalten haben. Falls nicht, siehe <http://www.gnu.org/licenses/>,
<http://www.gnu.de/documents/index.de.html>.


This source code file is a part of the FUGENSCHNITZER Program Library.
The FUGENSCHNITZER Program Library is subject to the
GNU Lesser General Public Licence (Version 3):
http://www.gnu.org/licenses/lgpl-3.0.html
http://www.gnu.org/licenses/gpl-3.0.html

The FUGENSCHNITZER Program Library is a Seam Carving program library.
FUGENSCHNITZER -- Seam Carving for everyone.

Copyright (C) 2008/9 David Eckardt

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/


#define SC_MGMNT_INLINE_INTERN

#include <stdlib.h> // realloc, abs
#include <string.h> // memcpy. memmove
#include "sc_defi.h"
#include "sc_core.h"
#include "sc_carve.h"
#include "sc_color.h"
#include "sc_mgmnt.h"
#include "sc_shift.h"

/******************************************************************************/

bool tryrealloc(void **v, const size_t s) {
	void *w = realloc(*v, s);
	bool e = ((w == NULL) && s);
    if (!e)
	    *v = w;
	return e;
}

void calc_info_sizes(struct info_s *info) {
	info->maxhw = max2(info->height, info->original_width);
	info->minhw = min2_int(info->height, info->original_width);
	info->pheight = zoom_div(info->height, info->zoom);
	info->pwidth = zoom_div(info->ext_width, info->zoom);
	info->original_pwidth = zoom_div(info->original_width, info->zoom);
}

void create_info(
	struct info_s *info, const long height, const long width, const long zoom
) {
	
	info->height = height;
	info->width = width;
	info->original_width = width;
	info->ext_width = width;
	info->zoom = zoom;
	info->si = 0;
	info->sc = 0;
	calc_info_sizes(info);
}

void set_comp_pointers(struct comp_s *image, const struct info_s *info) {
	comp_t *data = image->data;
   	for (int i = 0; i < info->height; i++) {
		image->px[i] = data;
		data += info->original_width;
	}
	image->tmp = data;
}

void set_intn_pointers(struct intn_s *image, const struct info_s *info) {
	intn_t *data = image->data;
   	for (int i = 0; i < info->height; i++) {
		image->px[i] = data;
		data += info->original_width;
	}
}

void set_mark_pointers(struct mark_s *image, const struct info_s *info) {
	mark_t *data = image->data;
   	for (int i = 0; i < info->height; i++) {
		image->px[i] = data;
		data += info->original_width;
	}
	image->tmp = data;
}

void set_diff_pointers(struct intn_s *diff_img, const struct info_s *info) {
	intn_t *data = diff_img->data;
   	for (int i = 0; i < info->height; i++) {
//		diff_img->px[-1] auf Unendlich setzen
		*data++ = INTN_INFTY;
		diff_img->px[i] = data;
		data += info->original_width;
//		diff_img->px[original_width] auf Unendlich setzen
		*data++ = INTN_INFTY;
	}
}

void set_seams_pointers(struct seams_s *seams, const struct info_s *info) {
	const size_t sz = info->height * sizeof(seam_t);
	seam_t *data = seams->data;
   	for (int k = 0; k < info->original_width; k++) {
		seams->coord[k] = data;
		data += info->height;
//		Verschiebung auf Null initialisieren
		seams->shift[k] = data;
		memset(data, 0, sz);
		data += info->height;	
	}
	seams->tmp = data;
}

bool create_comp_img(struct comp_s *image, const struct info_s *info) {
	bool e = false;

	const size_t n = info->height * info->original_width +		// px
					 info->maxhw;								// tmp
	e |= tryrealloc((void**)&image->data, n * sizeof(comp_t));
	if (e) return e;
	e |= tryrealloc((void**)&image->px, info->maxhw * sizeof(comp_t*));
	if (e) return e;
	
	set_comp_pointers(image, info);
	
	return e;
}

bool create_intn_img(struct intn_s *image, const struct info_s *info) {
	bool e = false;
	
	const size_t n = info->height * info->original_width;
	e |= tryrealloc((void**)&image->data, n * sizeof(intn_t));
	if (e) return e;
	e |= tryrealloc((void**)&image->px, info->maxhw * sizeof(intn_t*));
	if (e) return e;
	
	set_intn_pointers(image, info);

	return e;
}

extern size_t max2_sz(const size_t a, const size_t b);
inline size_t max2_sz(const size_t a, const size_t b)
	{return a > b? a: b;}

bool create_diff_img(
	void **heap, struct intn_s *diff_img, const struct info_s *info
) {
	bool e = false;
/*  Links und rechts je eine zusätzliche Spalte mit Maximalwert.
	Diese beiden Spalten werden bei der Fugensuche verwendet.*/
	const size_t sz = max2_sz(sizeof(intn_t),  sizeof(comp_t)),
				psz = max2_sz(sizeof(intn_t*), sizeof(comp_t*)),
				  n = info->height * info->original_width + info->maxhw * 2;
	e |= tryrealloc((void**)&diff_img->data, n * sz);
	*heap = diff_img->data;
	e |= tryrealloc((void**)&diff_img->px, info->maxhw * sizeof(intn_t*));

	set_diff_pointers(diff_img, info);

	return e;
}

bool create_mark_img(struct mark_s *mark_img, const struct info_s *info) {
	bool e = false;
	
	const size_t n = info->height * info->original_width + info->maxhw;
	e |= tryrealloc((void**)&mark_img->data, n * sizeof(mark_t));
	e |= tryrealloc((void**)&mark_img->px, info->maxhw * sizeof(mark_t*));

	set_mark_pointers(mark_img, info);
	
	return e;
}

bool create_seams(struct seams_s *seams, const struct info_s *info) {
	bool e = false;
	
	const size_t n = info->height * info->original_width * 2 +	// coord + shift
					 info->maxhw;								// tmp
	e |= tryrealloc((void**)&seams->data, n * sizeof(seam_t));
	e |= tryrealloc((void**)&seams->coord, info->maxhw * sizeof(seam_t*));
	e |= tryrealloc((void**)&seams->shift, info->maxhw * sizeof(seam_t*));

	set_seams_pointers(seams, info);

	return e;
}

/******************************************************************************/

/*
extend
transpose
load
eject
zoom
preview
fix => sc_carve
*/

void load_comp(
	struct comp_s *image, const comp_t *image_in, const struct info_s *info
) {
   	memcpy(
		image->data, image_in,
		(size_t)info->height * (size_t)info->original_width * sizeof(comp_t)
	);
}

void eject_comp(
	comp_t *img_out, const struct comp_s *image, const struct info_s *info
) {
	if (info->flags.transposed)
		for (int j = 0; j < info->ext_width; j++)
			for (int i = 0; i < info->height; i++)
				*img_out++ = image->px[i][j];
	else
		for (int i = 0; i < info->height; i++)
			for (int j = 0; j < info->ext_width; j++)
				*img_out++ = image->px[i][j];
}

void zoom_comp(
	comp_t *img_out, const struct comp_s *image, const struct info_s *info
) {
	const float d = info->zoom * info->zoom;
	if (info->flags.transposed)
		for (int j = 0; j < info->pwidth; j++)
			for (int i = 0; i < info->pheight; i++)
				*img_out++ = zoom_sample(i, j, d, image, info);
	else
		for (int i = 0; i < info->pheight; i++)
			for (int j = 0; j < info->pwidth; j++)
				*img_out++ = zoom_sample(i, j, d, image, info);
}

void preview_comp(
	comp_t *image_out, struct comp_s *image, const struct info_s *info
) {
	if (image_out == NULL) return;
	if (info->zoom > 1)
		zoom_comp(image_out, image, info);
	else
		eject_comp(image_out, image, info);
}

/*** mark: load, eject ********************************************************/

/*
extend
transpose
(unzoom, copy)
load
eject
zoom
preview
fix => sc_carve
*/

/*

void unzoom_mark_sample(
	const int i, const int j, const mark_t mark,
	struct mark_s *mark_img, const struct info_s *info
) {
	const int I = i * info->zoom,
			  J = j * info->zoom;
	for (int k = 0; k < info->zoom; k++)
		for (int l = 0; l < info->zoom; l++)
			mark_img->px[I + k][J + l] = mark;
}

void unzoom_mark(
	struct mark_s *image, const mark_t *img_in, const struct info_s *info) {
	if (!info->zoom) return;

//  Transposition umgekehrt: Warum?
	if (info->flags.transposed)
		for (int i = 0; i < info->pheight; i++)
			for (int j = 0; j < info->original_pwidth; j++)
				unzoom_mark_sample(
					i, j,
					img_in[i + info->pheight * j],
					image, info
				);
	else
		for (int i = 0; i < info->pheight; i++)
			for (int j = 0; j < info->original_pwidth; j++)
				unzoom_mark_sample(
					i, j,
					img_in[info->original_pwidth * i + j],
					image, info
				);
}

void copy_mark(
	struct mark_s *image, const mark_t *img_in, const struct info_s *info
) {
	if (info->flags.transposed)
		for (int i = 0; i < info->height; i++)
			for (int j = 0; j < info->original_width; j++)
				image->px[i][j] = img_in[i + j * info->height];
	else
		for (int i = 0; i < info->height; i++)
			for (int j = 0; j < info->original_width; j++)
				image->px[i][j] = img_in[i * info->original_width + j];
}
*/
/*
extern void copy_mark_alpha_px(
	mark_t *m, const comp_t *c, const comp_t bitmask, const int shift
);
inline void copy_mark_alpha_px(
	mark_t *m, const comp_t *c, const comp_t bitmask, const int shift
) {*m = ((*c >> shift) & bitmask);}

bool copy_mark_alpha(
	struct mark_s *image, const struct comp_s *comp_img, struct info_s *info
) {
	const int c = info->channels;

	const comp_t bitmask = info->channel[c - 1].bitmask;
	int shift = 0;
	for (int k = 0; k < c - 1; k++)
		shift += info->channel[k].width;
		
	if (info->flags.transposed)
		for (int i = 0; i < info->height; i++)
			for (int j = 0; j < info->original_width; j++)
				copy_mark_alpha_px(
					image->px[i] + j, comp_img->px[j] + i, bitmask, shift
				);
	else
		for (int i = 0; i < info->height; i++)
			for (int j = 0; j < info->original_width; j++)
				copy_mark_alpha_px(
					image->px[i] + j, comp_img->px[i] + j, bitmask, shift
				);

	info->flags.mark = IMARK_ALPHA;
	
	return false;
}
*/
/*
bool load_mark(
	struct mark_s *image, const mark_t *img_in, const struct comp_s *comp_img,
	struct info_s *info
) {
	if (info->zoom > 1)
		unzoom_mark(image, img_in, info);
	else
		copy_mark(image, img_in, info);
		
	info->flags.mark = IMARK_MARK;
	return false;
}

mark_t zoom_mark_sample(
	const int i, const int j,
	const struct mark_s *image, const struct info_s *info
) {
	int I = i * info->zoom,
	    J = j * info->zoom;

    mark_t mark = 0;
	for (int k = 0; k < info->zoom; k++)
		for (int l = 0; l < info->zoom; l++)
			mark |= image->px[I + k][J + l];
	return mark;

}

void zoom_mark(
	mark_t *pre_img, const struct mark_s *image, const struct info_s *info
) {
//	if (!info->zoom) return;
	if (info->zoom <= 1) return;

   	if (info->flags.transposed)
		for (int i = 0; i < info->pheight; i++)
			for (int j = 0; j < info->pwidth; j++)
				pre_img[i +  j * info->pheight] =
					zoom_mark_sample(i, j, image, info);
   	else
		for (int i = 0; i < info->pheight; i++)
			for (int j = 0; j < info->pwidth; j++)
				pre_img[i * info->pwidth + j] =
					zoom_mark_sample(i, j, image, info);
}

void eject_mark(
	mark_t *img_out, const struct mark_s *image, const struct info_s *info
) {
	if (info->flags.transposed)
		for (int i = 0; i < info->height; i++)
			for (int j = 0; j < info->ext_width; j++)
			    img_out[i + j * info->height] |= image->px[i][j];
	else
		for (int i = 0; i < info->height; i++)
			for (int j = 0; j < info->ext_width; j++)
			    img_out[i * info->ext_width + j] |= image->px[i][j];
}

void preview_mark(
	mark_t *pre_img, const struct mark_s *image, const struct info_s *info
) {
	if (
		(info->flags.mark != IMARK_MARK) ||
		info->si ||
		pre_img == NULL
	) return;

	if (info->zoom > 1)
		zoom_mark(pre_img, image, info);
	else
		eject_mark(pre_img, image, info);
}
*/
/*** transpose ****************************************************************/
/*
void transpose_mark(
	struct mark_s *image, void *heap, const struct info_s *info
) {
//	Transponieren ist nicht möglich, wenn das Bild bereits geändert wurde.
	if (!info->flags.mark || info->si || info->sc) return;

	mark_t *tmp = heap;
//	Bild nach heap kopieren, dabei Zählpriorität vertauschen
	for (int j = 0; j < info->original_width; j++)
		for (int i = 0; i < info->height; i++)
		    *(tmp++) = image->px[i][j];

//	Bild wieder zurückkopieren
	const size_t l = info->height * sizeof(mark_t);
	tmp = heap;

	for (int i = 0; i < info->original_width; i++) {
		image->px[i] = image->data + (i * info->height);
		memcpy(image->px[i], tmp, l);
		tmp += info->height;
	}
}
*/

bool transpose_comp(
	struct comp_s *image, void *heap, const struct info_s *info
) {
//	Transponieren ist nicht möglich, wenn das Bild bereits geändert wurde.
	if (info->sc || info->si) return true;
//	Bild nach heap kopieren, dabei Zählpriorität vertauschen
	comp_t *tmp = heap;
	for (int j = 0; j < info->original_width; j++)
		for (int i = 0; i < info->height; i++)
		    *(tmp++) = image->px[i][j];

//	Bild wieder zurückkopieren
	const size_t l = info->height * sizeof(comp_t);
	tmp = heap;

	for (int i = 0; i < info->original_width; i++) {
		image->px[i] = image->data + (i * info->height);
		memcpy(image->px[i], tmp, l);
		tmp += info->height;
	}

	return false;
}

bool transpose_images(
	struct comp_s *comp_img, struct mark_s *mark_img, struct intn_s *intn_img,
	struct intn_s *diff_img, struct seams_s *seams, struct info_s *info,
	void *heap
) {
	transpose_comp(comp_img, heap, info);
//	transpose_mark(mark_img, heap, info);

	info->flags.transposed = !info->flags.transposed;
	create_info(info, info->original_width, info->height, info->zoom);

	set_mark_pointers(mark_img, info);
	set_intn_pointers(intn_img, info);
	set_diff_pointers(diff_img, info);
	set_seams_pointers(seams, info);
	
	return false;
}

/*** extend *******************************************************************/
/*
bool extend_mark(
	struct mark_s *image, const struct info_s *info, const int ext_before
) {
	bool e = false;
//	Speicherbereich vergrößern
	size_t n = info->height * info->ext_width +
			   max2(info->height, info->ext_width);
	e |= tryrealloc((void**)&image->data, n * sizeof(mark_t));

	n = max2(info->maxhw, info->ext_width);
	e |= tryrealloc((void**)&image->px, n * sizeof(mark_t*));

	if (e) return e;

//	Zeilen verschieben
	const size_t l = info->ext_width * sizeof(mark_t);
    mark_t *s = image->data + (info->height - 1) * ext_before,
		  *t = image->data + (info->height - 1) * info->ext_width;

	for (int i = info->height - 1; i > 0; i--) {
		memmove(t, s, l);
		image->px[i] = t;
		t -= info->ext_width;
		s -= ext_before;
	}
	image->px[0] = image->data;
	image->tmp = image->data + (info->height * info->ext_width);

	return e;
}
*/
bool extend_comp(
	struct comp_s *image, const struct info_s *info, const int ext_before
) {
	bool e = false;
//	Bildspeicher vergrößern: original_width => ext_width
	size_t n = info->height * info->ext_width +				// px
			   max2(info->height, info->ext_width);			// tmp
	e |= tryrealloc((void**)&image->data, n * sizeof(comp_t));
//	Zeigerspeicher vergrößern

	n = max2(info->maxhw, info->ext_width);
	e |= tryrealloc((void**)&image->px, n * sizeof(comp_t*));
	
	if (e) return e;

//	Pixel zeilenweise nach hinten verschieben:
//	Initialisieren
	const size_t l = info->ext_width * sizeof(comp_t);
    comp_t *s = image->data + (info->height - 1) * ext_before,
		   *t = image->data + (info->height - 1) * info->ext_width;

	for (int i = info->height - 1; i > 0; i--) {
//		Verschieben
		memmove(t, s, l);
//		Zeilenzeiger setzen
		image->px[i] = t;
		t -= info->ext_width;
		s -= ext_before;
	}
//	Nullter Zeiger ist noch nicht gesetzt: Setzen
	image->px[0] = image->data;
//	tmp hat sich auch verschoben
	image->tmp = image->data + (info->height * info->ext_width);

	return e;
}

/*	
 *	Rückgabewert:
 *	0:	Das Bild wurde nicht erweitert.
 *	1:	Das Bild wurde erweitert.
 *	-1:	Fataler Fehler: Speicheranforderung fehlgeschlagen
 */

int extend_images(
	struct comp_s *comp_img, struct mark_s *mark_img,
	struct info_s *info, const int extend
) {
	const int ext_old = info->ext_width,
			  ext_new = info->original_width + minz(extend);

	if (ext_new <= ext_old) return 0;

	info->ext_width = ext_new;
	calc_info_sizes(info);

	if (extend_comp(comp_img, info, ext_old))
		return -1;
	
/*	if (extend_mark(mark_img, info, ext_old))
		return -1;*/

	return 1;
}

/*** fix **********************************************************************/

void fix_align_comp(
	struct comp_s *image, const struct info_s *info, const bool restore
) {
	const int w = restore?
		info->original_width:
		info->original_width - info->sc;
	const size_t l = w * sizeof(comp_t);

	comp_t *t = image->data + w;

	for (long int i = 1; i < info->height; i++) {
	    memmove(t, image->px[i], l);
	    image->px[i] = t;
	    t += w;
	}
}
/*
void fix_comp(
	struct comp_s *image, struct seams_s *seams, struct info_s *info,
	const bool restore
) {
//    fix_shift_comp(image, seams, info, restore);
	if (restore)
		if (info->sc > 0)
			undelete_comp(image, seams, info, 0);
		else
			uninsert_comp(image, seams, info, 0);

    fix_align_comp(image, info, restore);
}
*/
/*
void fix_align_mark(
	struct mark_s *image, const struct info_s *info, const bool restore
) {
	const int w = restore?
		info->original_width:
		info->original_width - info->sc;
	const size_t l = w * sizeof(mark_t);
	mark_t *t = image->data + w;

	for (int i = 1; i < info->height; i++) {
	    memmove(t, image->px[i], l);
	    image->px[i] = t;
	    t += w;
	}
}


void fix_mark(
	struct mark_s *image, struct seams_s *seams, const struct info_s *info,
	const bool restore
) {
//	if (!info->flags.mark) return;
	if (info->flags.mark != IMARK_MARK) return;
    
    fix_shift_mark(image, seams, info, restore);
    fix_align_mark(image, info, restore);
}
*/
/*
void fix_images(
	struct comp_s *comp_img, struct mark_s *mark_img,
	const bool restore,
	struct seams_s *seams, struct info_s *info
) {
	fix_comp(comp_img, seams, info, restore);
//	fix_mark(mark_img, seams, info, restore);
	
	if (restore)
		info->sc = 0;

	for (int i = 0; i < info->height; i++)
		clear_shift_ln(seams, info, i);
}
*/
