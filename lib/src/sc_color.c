/*
sc_color.c

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

#include <limits.h>
#include <stdlib.h>
#include <math.h>		// lrintf
#include "sc_defi.h"
#include "sc_mgmnt.h"
#include "sc_shift.h"
#include "sc_color.h"

const int CHANNELS_RGB      = 3,
		  CHANNELS_RGB32    = 4,
		  CHANNEL_WIDTH_RGB = 8;

const comp_t BITMASK_RGB    = 0xFF;

#include <stdbool.h>
// http://www.ibm.com/developerworks/aix/library/au-endianc/index.html
bool is_littlee(void) {
	const int n = 1;
	return *(char*) &n;
}

/*** py_rgb: load *************************************************************/

comp_t load_comp_py_px_reverse_24(const uint8_t **image_in) {
	*image_in += CHANNELS_RGB;
	comp_t c = 0;
	for (int j = 0; j < CHANNELS_RGB; j++) {
		c <<= CHANNEL_WIDTH_RGB;
		c |= *(--(*image_in));
	}
	*image_in += CHANNELS_RGB;
	return c;
}

comp_t load_comp_py_px_reverse(const uint8_t **image_in) {
	*image_in += CHANNELS_RGB;
	comp_t c = 0;
	for (int j = 0; j < CHANNELS_RGB; j++) {
		c <<= CHANNEL_WIDTH_RGB;
		c |= *((*image_in)--);
	}
	*image_in += CHANNELS_RGB;
	return c;
}

comp_t load_comp_py_px_progressive(const uint8_t **image_in) {
	comp_t c = 0;
	for (int j = 0; j < CHANNELS_RGB; j++) {
		c <<= CHANNEL_WIDTH_RGB;
		c |= *((*image_in)++);
	}
	return c;
}

typedef comp_t (*load_comp_py_px_func_t)(const uint8_t **image_in);

const load_comp_py_px_func_t load_comp_py_px_func[] = {
	load_comp_py_px_reverse,
	load_comp_py_px_progressive
};

void load_comp_py_rgb32(
	struct comp_s *image, const uint8_t *image_in, const struct info_s *info
) {
	const long int l = info->height * info->original_width;
	const load_comp_py_px_func_t load_comp_py_px = load_comp_py_px_func[is_littlee()];
	comp_t *d = image->data;

	for (long int i = 0; i < l; i++) {
		*d++ = load_comp_py_px(&image_in) | 0xFF000000;
		image_in++;
	}
}

void load_comp_py_rgb24(
	struct comp_s *image, const uint8_t *image_in, const struct info_s *info
) {
	const long int l = info->height * info->original_width;
	comp_t *d = image->data;

	for (long int i = 0; i < l; i++)
		*d++ = load_comp_py_px_reverse_24(&image_in) | 0xFF000000;
}

void load_comp_py_rgb(
	struct comp_s *image, const uint8_t *image_in, const struct info_s *info,
	const bool rgb32
) {
	if (rgb32)
		load_comp_py_rgb32(image, image_in, info);
	else
		load_comp_py_rgb24(image, image_in, info);
}

/*** py_rgb: eject, zoom, preview *********************************************/

void py_rgb_px(uint8_t *img_out, comp_t px, const int ch) {
	for (int k = 0; k < ch; k++) {
		img_out[k] = (uint8_t)px;
		px >>= CHANNEL_WIDTH_RGB;
	}
}

void zoom_comp_ln_py_rgb(
	uint8_t *img_out, const struct comp_s *image, const struct info_s *info,
	const long int i, const int ch, const int d
) {
	const int a = (info->flags.transposed? 1: info->pwidth) * ch,
			  b = (info->flags.transposed? info->pheight: 1) * ch;

	img_out += i * a;
	for (int j = 0; j < info->pwidth; j++) {
		py_rgb_px(img_out, zoom_sample(i, j, d, image, info), ch);
		img_out += b;
	}
}

void zoom_comp_py_rgb(
	uint8_t *img_out, const struct comp_s *image, const struct info_s *info,
	const bool rgb32
) {
	const int ch = rgb32? 4: 3,
			   d = info->zoom * info->zoom;

	for	(int i = 0; i < info->pheight; i++)
		zoom_comp_ln_py_rgb(img_out, image, info, i, ch, d);
}

// d only for the function prototype to meet zoom_comp_ln_py_rgb:

void eject_comp_ln_py_rgb(
	uint8_t *img_out, const struct comp_s *image, const struct info_s *info,
	const long int i, const int ch, const int d
) {
	const int a = (info->flags.transposed? 1: info->ext_width) * ch,
			  b = (info->flags.transposed? info->height: 1) * ch;

	const comp_t *px = image->px[i];

	img_out += i * a;
	for (int j = 0; j < info->ext_width; j++) {
		py_rgb_px(img_out, *px++, ch);
		img_out += b;
	}
}

void eject_comp_py_rgb(
	uint8_t *img_out, const struct comp_s *image, const struct info_s *info,
	const bool rgb32
) {
	const int ch = rgb32? 4: 3;

	for	(int i = 0; i < info->height; i++)
		eject_comp_ln_py_rgb(img_out, image, info, i, ch, 0);
}

void preview_comp_py_rgb(
	uint8_t *image_out, struct comp_s *image, const struct info_s *info,
	const bool zoom, const bool rgb32
) {
	if (image_out == NULL) return;

	if (zoom && info->zoom > 1)
		zoom_comp_py_rgb(image_out, image, info, rgb32);
	else
		eject_comp_py_rgb(image_out, image, info, rgb32);
}

/*** py_rgb: display_seams ****************************************************/

void display_seam_py_rgb(
	uint8_t *image_out, const long int k,
	const struct seams_s *seams, const struct info_s *info, const bool zoom,
	const uint8_t color[], const long int ch, const int d
) {
	const int z = zoom? info->zoom: 1;
	const long int l = info->flags.transposed? info->height: info->ext_width,
				   b = zoom_div(l, z);

	for (long int i = 0; i < info->height; i++) {
		const long int y = zoom_div(i, z),
					   x = zoom_div(seam_coord(seams, k, i), z);
		uint8_t *p = image_out +
			(info->flags.transposed? ((b * x) + y): ((b * y) + x)) * ch;
		for (int c = 0; c < ch; c++)
			*p++ = color[c];
	}
}

void display_seams_py_rgb(
	uint8_t *image_out, const struct seams_s *seams, const struct info_s *info,
	const bool zoom,
	const int first, const int last, const uint8_t color[], const bool rgb32
) {
	const int sc = abs(info->sc),
			  ch = 3 + rgb32;

	for (int k = first; k < last; k++)
		display_seam_py_rgb(image_out, k, seams, info, zoom, color, ch, 0);
}

/******************************************************************************/


extern comp_t bitmask(const int width);
inline comp_t bitmask(const int width)
	{return ((comp_t)1 << width) - 1;}

intn_t rgb_to_intn_px(
	const comp_t *comp_ln, const struct info_s *info
) {
	const float COMP_WEIGHT_BGR[] =
		{_COMP_WEIGHT_B_, _COMP_WEIGHT_G_, _COMP_WEIGHT_R_};	//	sc_defi.h
	comp_t rgba = *comp_ln;
	float ipx = 0;
	for (int k = 0; k < CHANNELS_RGB; k++) {
		ipx += (float)(rgba & BITMASK_RGB) * COMP_WEIGHT_BGR[k];
		rgba >>= CHANNEL_WIDTH_RGB;
	}
	return ipx;
}

void rgb_to_intn_ln(
	intn_t *intn_ln, const comp_t *comp_ln,
	const int l, const struct info_s *info
) {
	for (int j = 0; j < l; j++)
		*intn_ln++ = rgb_to_intn_px(comp_ln++, info);
}

void rgb_to_intn(
	struct intn_s *intn_img, const struct comp_s *comp_img,
	const struct info_s *info
) {
	for (int i = 0; i < info->height; i++)
		rgb_to_intn_ln(intn_img->px[i], comp_img->px[i], info->width, info);
}

extern void sum_sample(comp_t si, float *rgba, const struct info_s *info);
inline void sum_sample(comp_t si, float *rgba, const struct info_s *info) {
	for (int c = 0; c < COMP_CHANNELS_MAX; c++) {
		rgba[c] += si & BITMASK_RGB;
		si >>= CHANNEL_WIDTH_RGB;
	}
}

comp_t avg_sample(const float d, const float *rgba, const struct info_s *info) {
	comp_t so = 0;
	for (int c = COMP_CHANNELS_MAX - 1; c >= 0; c--) {
		so <<= CHANNEL_WIDTH_RGB;
		so |= (comp_t)lrintf(rgba[c] / d) & BITMASK_RGB;
	}
	return so;
}

comp_t zoom_sample(
	const int i, const int j, const float d,
	const struct comp_s *image, const struct info_s *info
) {
	const int I = i * info->zoom,
			  J = j * info->zoom;

	float rgba[COMP_CHANNELS_MAX] = {0};

	for (int k = 0; k < info->zoom; k++)
		for (int l = 0; l < info->zoom; l++)
			sum_sample(image->px[I + k][J + l], rgba, info);

	return avg_sample(d, rgba, info);
}

GCC_HOT void sum_sample_2(
	comp_t si, comp_t *rgba, const struct info_s *info
) {
	for (int c = 0; c < CHANNELS_RGB; c++) {
		rgba[c] += si & BITMASK_RGB;
		si >>= CHANNEL_WIDTH_RGB;
	}
}

GCC_HOT comp_t avg_sample_2(const comp_t *rgba,	const struct info_s *info) {
	comp_t so = 0;
	for (int c = CHANNELS_RGB - 1; c >= 0; c--) {
		so <<= CHANNEL_WIDTH_RGB;
		so |= (rgba[c] / 2) & BITMASK_RGB;
	}
	return so;
}

GCC_HOT comp_t comp_interpol(const comp_t *c, const struct info_s *info) {
	comp_t rgba[COMP_CHANNELS_MAX] = {0};
	sum_sample_2(*c, rgba, info);
 	sum_sample_2(*(c - 1), rgba, info);
    return avg_sample_2(rgba, info);
}