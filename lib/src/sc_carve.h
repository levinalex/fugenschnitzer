/*
sc_carve.h

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

#ifndef SC_CARVE_H
#define SC_CARVE_H

#include "sc_defi.h"
#include "sc_mgmnt.h"

#define INLINE_INTERN defined SC_CARVE_INLINE_INTERN
#include "gnuinl99.h"

/*
#ifdef EXTERN
EXTERN int minz(const int i);
#endif

#ifdef INLINE
INLINE int minz(const int i)
	{return (i >= 0)? i: 0;}
#endif
*/
void delete_intn_ln(
	struct intn_s *intn_img, struct intn_s *diff_img, struct mark_s *mark_img,
	const seam_t s, const struct info_s *info, const long int i
);

/*
void fix_shift_comp(
	struct comp_s *image, struct seams_s *seams, struct info_s *info,
	const bool restore
);

void fix_shift_mark(
	struct mark_s *image, struct seams_s *seams, const struct info_s *info,
	const bool restore
);
*/
void carve_comp(
	struct comp_s *image, struct seams_s *seams, struct info_s *info,
	const long int last, const bool insert, const bool reverse, int *progress
);

void carve_comp_part(
	struct comp_s *image, struct seams_s *seams, struct info_s *info,
	const long int last, const bool origin, const bool insert, const bool reverse,
	const long int vfirst, const long int vlast,
	comp_t *image_tmp, seam_t *seams_tmp, int *progress
);

long int carve_comp_finish(struct info_s *info, const long int last, const bool insert);

void carve_comp_py_rgb(
	uint8_t *img_out, struct comp_s *image, struct seams_s *seams,
	struct info_s *info, const bool zoom,
	const long int last, const bool origin, const bool insert, const bool reverse,
	int *progress, const bool rgb32
);

void carve_comp_py_rgb_part(
	uint8_t *img_out, struct comp_s *image, struct seams_s *seams,
	struct info_s *info, const bool zoom,
	const long int last, const bool origin, const bool insert, const bool reverse,
	const long int vfirst, const long int vlast,
	comp_t *image_tmp, seam_t *seams_tmp, int *progress, const bool rgb32
);

long int carve_comp_py_rgb_finish(
	struct comp_s *image, struct seams_s *seams, struct info_s *info,
	const bool zoom, const long int last, const bool origin,
	const bool insert, const bool reverse
);

#endif // SC_CARVE_H