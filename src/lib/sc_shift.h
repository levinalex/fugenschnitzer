/*
sc_shift.h

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

#ifndef SC_SHIFT_H
#define SC_SHIFT_H

#include "sc_defi.h"

#define INLINE_INTERN defined SC_SHIFT_INLINE_INTERN
#include "gnuinl99.h"

#ifdef EXTERN
EXTERN seam_t seam_coord(const struct seams_s *seams, const int k, const int i);
#endif

#ifdef INLINE
INLINE seam_t seam_coord(const struct seams_s *seams, const int k, const int i)
	{return seams->coord[k][i] + seams->shift[k][i];}
#endif

void clear_shift_ln(struct seams_s *seams, const struct info_s *info, const int i);

void restore_seams(struct seams_s *seams, const struct info_s *info);
void prepare_seams(struct seams_s *seams, const struct info_s *info);
void restore_seams_ln(struct seams_s *seams, const struct info_s *info, const int i);
void prepare_seams_ln(struct seams_s *seams, const struct info_s *info, const int i);

void rshift_seams_ln(
	struct seams_s *seams, const struct info_s *info,
	const int first, const int last, const int i
);
void lshift_seams_ln(
	struct seams_s *seams, const struct info_s *info,
	const int first, const int last, const int i
);

void irshift_seams_ln(
	struct seams_s *seams, const struct info_s *info,
	const int first, const int last, const int i
);

void sort_seams_ln_noshift(
	struct seams_s *seams, const int first, const int n, const int i
);

void sort_seams_ln_paral(
	struct seams_s *seams, seam_t *tmp, const seam_t end,
	const int first, const int n, const int i
);
#endif // SC_SHIFT_H
