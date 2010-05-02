/*
sc_mgmnt.h

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

#ifndef SC_MGMNT_H
#define SC_MGMNT_H

#include <stdbool.h>	// tryrealloc
#include <stddef.h>		// size_t: tryrealloc
#include "sc_defi.h"

#define INLINE_INTERN defined SC_MGMNT_INLINE_INTERN
#include "gnuinl99.h"


#ifdef EXTERN
EXTERN int max2(const int a, const int b);
EXTERN int min2_int(const int a1, const int a2);
EXTERN int zoom_mul(const int x, const int zoom);
EXTERN int zoom_div(const int x, const int zoom);
#endif

#ifdef INLINE
INLINE int max2(const int a, const int b)
	{return a > b? a: b;}

INLINE int min2_int(const int a1, const int a2)
	{return (a1 < a2)? a1: a2;}

INLINE int zoom_mul(const int x, const int zoom)
	{return zoom? x * zoom: x;}

INLINE int zoom_div(const int x, const int zoom)
	{return zoom? x / zoom: x;}
#endif

bool tryrealloc(void **v, const size_t s);

void create_info(
	struct info_s *info,
	const long height, const long width, const long zoom
);
void calc_info_sizes(struct info_s *info);

bool create_comp_img(struct comp_s *image, const struct info_s *info);
bool create_intn_img(struct intn_s *image, const struct info_s *info);
bool create_diff_img(void **heap, struct intn_s *diff_img, const struct info_s *info);
bool create_mark_img(struct mark_s *mark_img, const struct info_s *info);
bool create_seams(struct seams_s *seams, const struct info_s *info);

void load_comp(
	struct comp_s *image, const comp_t *image_in, const struct info_s *info
);

/******************************************************************************/

void eject_comp(
	comp_t *img_out, const struct comp_s *image, const struct info_s *info
);

void preview_comp(
	comp_t *image_out, struct comp_s *image, const struct info_s *info
);

/******************************************************************************/
/*
bool load_mark(
	struct mark_s *image, const mark_t *img_in, const struct comp_s *comp_img,
	struct info_s *info
);

void preview_mark(
	mark_t *image_out, const struct mark_s *image, const struct info_s *info
);
void eject_mark(
	mark_t *image_out, const struct mark_s *image, const struct info_s *info
);
*/
/*	
 *	Rückgabewert:
 *	0:	Das Bild wurde nicht erweitert.
 *	1:	Das Bild wurde erweitert.
 *	-1:	Fataler Fehler: Speicheranforderung fehlgeschlagen
 */

int extend_images(
	struct comp_s *comp_img, struct mark_s *mark_img, struct info_s *info,
	const int extend
);

bool transpose_images(
	struct comp_s *comp_img, struct mark_s *mark_img, struct intn_s *intn_img,
	struct intn_s *diff_img, struct seams_s *seams, struct info_s *info,
	void *heap
);

void fix_align_comp(
	struct comp_s *image, const struct info_s *info, const bool restore
);

/*
void fix_images(
	struct comp_s *comp_img, struct mark_s *mark_img,
	const bool restore,
	struct seams_s *seams, struct info_s *info
);
*/
int mark_width(const struct mark_s *mark_img, const struct info_s *info);

#endif // SC_MGMNT_H
