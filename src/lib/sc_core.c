/*
sc_core.c

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

#include <math.h>		// fabs: diff_margin
#include "sc_defi.h"
#include "sc_carve.h"
#include "sc_core.h"

const float weight_margin = 4.0f/3.0f;
const intn_t weight_corner = WEIGHT_CORNER;

extern          intn_t diff_margin(
	const struct intn_s *intn_img,
	const int ax, const int ay,
	const int b1x, const int b1y,
	const int b2x, const int b2y,
	const int b3x, const int b3y
);
inline GCC_COLD intn_t diff_margin(
	const struct intn_s *intn_img,
	const int ax, const int ay,
	const int b1x, const int b1y, const int b2x,
	const int b2y, const int b3x, const int b3y
) {
	const intn_t s = intn_img->px[ax][ay];
	return RINT((
		fabs(s - intn_img->px[b1x][b1y]) +
		fabs(s - intn_img->px[b2x][b2y]) +
		fabs(s - intn_img->px[b3x][b3y])
	) * weight_margin);
}

extern          intn_t diff_corner(
	struct intn_s *diff_img, const struct intn_s *intn_img,
	const int ax, const int ay,
	const int b1x, const int b1y, const int b2x, const int b2y
);
inline GCC_COLD intn_t diff_corner(
	struct intn_s *diff_img, const struct intn_s *intn_img,
	const int ax, const int ay,
	const int b1x, const int b1y, const int b2x, const int b2y
) {
	const intn_t d = ((
		ABS(intn_img->px[ax][ay] - intn_img->px[b1x][b1y]) +
		ABS(intn_img->px[ax][ay] - intn_img->px[b2x][b2y])
	) * weight_corner);
	diff_img->px[ax][ay] = d;
	return d;
}

void diff_margins(
	struct intn_s *diff_img, const struct intn_s *intn_img,
	const struct info_s *info
) {
	const int hlast = info->width  - 1,
			  vlast = info->height - 1;
//	Rand horizontal: oben
	for (int j = 1; j < hlast; j++) {
		diff_img->px[0][j] = diff_margin(intn_img,
			0, j,
			0, j - 1,
			1, j,
			0, j + 1
		);
//		unten
		diff_img->px[vlast][j] = diff_margin(intn_img,
			vlast,     j,
			vlast,     j - 1,
			vlast - 1, j,
			vlast,     j + 1
		);
	}
//	Rand vertikal: links
	for (int i = 1; i < vlast; i++) {
		diff_img->px[i][0] = diff_margin(intn_img,
			i,     0,
			i - 1, 0,
			i,     1,
			i + 1, 0
		);
//		rechts
		diff_img->px[i][hlast] = diff_margin(intn_img,
			i,     hlast,
			i - 1, hlast,
			i,     hlast - 1,
			i + 1, hlast
		);
//		hinterm rechten Rand +inf
		diff_img->px[i][hlast + 1] = INTN_INFTY;
	}
//	Ecken.
	diff_corner(diff_img, intn_img,
		0, 0,
		0, 1,
		1, 0
	);
	diff_corner(diff_img, intn_img,
		0, hlast,
		0, hlast - 1,
		1, hlast
	);
	diff_corner(diff_img, intn_img,
		vlast,     0,
		vlast - 1, 0,
		vlast,     1
	);
	diff_corner(diff_img, intn_img,
		vlast,     hlast,
		vlast - 1, hlast,
		vlast,     hlast - 1
	);
} // diff_margins

extern         intn_t diff_sample(intn_t **px, const int i, const int j);
inline GCC_HOT intn_t diff_sample(intn_t **px, const int i, const int j) {
	const intn_t s = px[i][j];
	return (
		ABS(s - px[i][j + 1]) +
		ABS(s - px[i - 1][j]) +
		ABS(s - px[i][j - 1]) +
		ABS(s - px[i + 1][j])
	);
}

void GCC_HOT diff_ln(
	struct intn_s *diff_img, const struct intn_s *intn_img,
	const int i, const int hfirst, const int hlast
) {
	intn_t *d = diff_img->px[i] + hfirst;
	for (int j = hfirst; j < hlast; j++)
		*(d++) = diff_sample(intn_img->px, i, j);
}

void diff(
	struct intn_s *diff_img, const struct intn_s *intn_img,
	const struct info_s *info
) {
	const int vlast = info->height - 1,
			  hlast = info->width - 1;

	diff_margins(diff_img, intn_img, info);

	for (int i = 1; i < vlast; i++)
	    diff_ln(diff_img, intn_img, i, 1, hlast);
}

extern         void mark_sample(const mark_t *m, intn_t *d);
inline GCC_HOT void mark_sample(const mark_t *m, intn_t *d) {
	if (*m & 1)
		*d = INTN_INFTY;
}

void GCC_HOT mark_ln(
	struct intn_s *diff_img, const struct mark_s *mark_img,
	const int i, const int hfirst, const int hlast
) {
	mark_t *m = mark_img->px[i];
	intn_t  *d = diff_img->px[i];

	for (int j = hfirst; j < hlast; j++)
		mark_sample(m++, d++);
}

void mark(
	struct intn_s *diff_img, struct mark_s *mark_img, 
	const struct info_s *info
) {
	if (info->flags.mark)
		for (int i = 0; i < info->height; i++)
			mark_ln(diff_img, mark_img, i, 0, info->width);
}

extern intn_t min2(const intn_t a1, const intn_t a2);
extern intn_t min3(const intn_t a1, const intn_t a2, const intn_t a3);
extern intn_t accu_margin(intn_t *px, const int d);
extern intn_t accu_sample(intn_t *px);

inline GCC_HOT intn_t min2(const intn_t a1, const intn_t a2)
	{return (a1 < a2)? a1: a2;}

inline GCC_HOT intn_t min3(const intn_t a1, const intn_t a2, const intn_t a3)
	{return min2(min2(a1, a2), a3);}

inline GCC_COLD intn_t accu_margin(intn_t *px, const int d)
	{return min2(*px, *(px + d));}

inline GCC_HOT intn_t accu_sample(intn_t *px)
	{return min3(*(px - 1), *px, *(px + 1));}

void accu_ln(struct intn_s *diff_img, const int i, const int hlast) {
	intn_t *s = diff_img->px[i],
		   *t = diff_img->px[i + 1];

	*t += accu_margin(s, 1);

	for (int j = 1; j < hlast; j++)
		*++t += accu_sample(++s);

	*++t += accu_margin(++s, -1);
}

void accu(struct intn_s *diff_img, const struct info_s *info) {
	const int hlast = info->width - 1,
			  vlast = info->height - 1;

	for (int i = 0; i < vlast; i++)
		accu_ln(diff_img, i, hlast);
}

const seam_t imin[8] = {0, -1, 0, -1, 1, 1, 0, 0};

extern seam_t imin3(const intn_t *d);
inline seam_t imin3(const intn_t *d) {
	const intn_t dc = *d,
				 dl = *(d - 1),
				 dr = *(d + 1);
	return imin[(dl < dc) + ((dc < dr) << 1) + ((dr < dl) << 2)];
//	return imin[(dl < dc) & ((dc < dr) << 1) & ((dr < dl) << 2)];
}


void trace_seam(
	struct intn_s *diff_img, struct intn_s *intn_img, struct mark_s *mark_img,
	seam_t *seam, struct info_s *info
) {
/*  intn_img->px[i][-1] und intn_img->px[i][width] sind gültige Elemente
	und besitzen den Wert INTN_INFTY.
	Siehe create_intn_img(), set_intn_pointers() und diff_margins().*/
	int i = info->height - 1;
	seam_t s = 0;
//	Minimum in der letzten Zeile des Differenzenbilds suchen
	intn_t least = INTN_INFTY;
	for (int j = 0; j < info->width; j++)
		if (diff_img->px[i][j] < least) {
			least = diff_img->px[i][j];
            s = j;
		}
		
	info->width--;
	info->si++;

	seam[i] = s;
	delete_intn_ln(intn_img, diff_img, mark_img, s, info, i);

//	Fuge ermitteln
	while (i--) {
		s += imin3(diff_img->px[i] + s);
		seam[i] = s;
		delete_intn_ln(intn_img, diff_img, mark_img, s, info, i);
	}
}

/*
void trace_seam_rc(
	struct intn_s *diff_img, struct intn_s *intn_img, struct mark_s *mark_img,
	seam_t *seam, struct info_s *info
) {
	const int last = info->height - 1;
	intn_t least = INTN_INFTY;
	
	seam_t s = 0;

//	Minimum in der letzten Zeile des Differenzenbilds suchen
	for (int j = 0; j < info->width; j++)
		if (diff_img->px[last][j] < least) {
			least = diff_img->px[last][j];
            s = j;
		}
		
	info->width--;
	info->si++;

	seam[last] = s;

	delete_intn_ln(
		intn_img->px[last],
		diff_img->px[last],
		mark_img->px[last],
		s, info
	);


//	Fuge ermitteln
	for (int i = last - 1; i >= 0; i--) {
		
		intn_t *a  = diff_img->px[i] + s,
			   *al = a - 1,
			   *ah = a + 1;
			   
		seam_t d = imin3(*al, *a, *ah);
		s += d;
		seam[i] = s;
		delete_intn_ln(
			intn_img->px[i],
			diff_img->px[i],
			mark_img->px[i],
			s, info
		);
	}
}*/
