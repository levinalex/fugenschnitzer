/*
sc_shift.c

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

#define SC_SHIFT_INLINE_INTERN

#include <stdlib.h>	// qsort: sort_seams_ln
#include "sc_shift.h"

// Verschiebt alle nachfolgenden rechten oder gleichen Fugen um eins nach rechts:
// right-shifts all folling right or equal seams by one to the right:
void restore_seam_ln(
	struct seams_s *seams, const struct info_s *info, const int n, const int i
) {
	const seam_t s = seams->coord[n][i];
	for (int k = n + 1; k < info->si; k++) {
		seam_t *t = seams->coord[k] + i;
		*t += (*t >= s);
	}
}

void restore_seams_ln(
	struct seams_s *seams, const struct info_s *info, const int i
) {
//	Fugen zurueckschieben auf Position im Originalbild
//	shift seams back to original image position
	for (int n = info->si - 1; n >= 0; n--)
		restore_seam_ln(seams, info, n, i);
//	Fugen verschieben auf Position im momentan veraenderten Bild
//	shift seams to actual image position
	if (info->sc >= 0)
		lshift_seams_ln(seams, info, 0, info->sc, i);
	else
//		irshift_seams_ln(seams, info, 0, -info->sc, i);
		rshift_seams_ln(seams, info, 0, -info->sc, i);
}

void restore_seams(struct seams_s *seams, const struct info_s *info) {
	for (int i = 0; i < info->height; i++)
		restore_seams_ln(seams, info, i);
}

// Verschiebt alle nachfolgenden rechten oder gleichen Fugen um eins nach links
// und loescht die Verschiebung:
// left-shifts all following right or equal seams by one to the left and clears
// shift:
void prepare_seam_ln(
	struct seams_s *seams, const struct info_s *info, const int n, const int i
) {
//	Verschiebung loeschen
//	clear shift
	seams->shift[n][i] = 0;
//	Anmerkung: Nachfolgende gleiche Fugen sind nicht moeglich.
//	Note: Following equal seams are not possible.
	const seam_t s = seams->coord[n][i];
	for (int k = n + 1; k < info->si; k++) {
		seam_t *t = seams->coord[k] + i;
		*t -= (*t >= s);
	}
}

void prepare_seams_ln(
	struct seams_s *seams, const struct info_s *info, const int i
) {
	for (int n = 0; n < info->si; n++)
		prepare_seam_ln(seams, info, n, i);
}

void prepare_seams(struct seams_s *seams, const struct info_s *info) {
	for (int i = 0; i < info->height; i++)
		prepare_seams_ln(seams, info, i);
}

/******************************************************************************/

int compare(const void *a, const void *b)
	{return (*(int*)a - *(int*)b);}

/******************************************************************************/

void seam_tmp_paral(
	struct seams_s *seams, seam_t *tmp, const seam_t end,
	const int start, const int n, const int i
) {
	for (int k = 0; k < n; k++)
		tmp[k] = seam_coord(seams, k + start, i);
	tmp[n] = end;
}

void sort_seams_ln_paral(
	struct seams_s *seams, seam_t *tmp, const seam_t end,
	const int first, const int n, const int i
) {
	seam_tmp_paral(seams, tmp, end, first, n, i);
	qsort(tmp, n, sizeof(seam_t), compare);
}

/******************************************************************************/

void seam_tmp_noshift(
	struct seams_s *seams, const int start, const int n, const int i
) {
	for (int k = 0; k < n; k++)
		seams->tmp[k] = seams->coord[k + start][i];
}

void sort_seams_ln_noshift(
	struct seams_s *seams, const int first, const int n, const int i
) {
	seam_tmp_noshift(seams, first, n, i);
	qsort(seams->tmp, n, sizeof(seam_t), compare);
}

void clear_shift_ln(
	struct seams_s *seams, const struct info_s *info, const int i
) {
	for (int k = 0; k < info->original_width; k++)
		seams->shift[k][i] = 0;
}

// Fugenverschiebung beim Entfernen von Fuge n:
// Verschiebt in Zeile i fuer Fuge n
// alle anderen groesseren Fugen um eins nach links.
// Seams shift when removing seam n:
// Shifts in line i for seam n all other greater seams by one to the left.
void lshift_seam_ln(
	struct seams_s *seams, const struct info_s *info,
	const int n, const int i
) {
	const seam_t s = seams->coord[n][i];
	for (int k = 0; k < info->si; k++)
		seams->shift[k][i] -= (seams->coord[k][i] > s);
}

// Fugenverschiebung beim Entfernen der Fugen first bis (last - 1):
// Verschiebt in Zeile i fuer jede Fuge aus [first bis (last - 1)]
// alle anderen groesseren Fugen um eins nach links.
// Seams shift when removing seams first to (last - 1):
// Shifts in line i for seam n for all seams of [first bis (last - 1)]
// all other seams by one to the left.
void lshift_seams_ln(
	struct seams_s *seams, const struct info_s *info,
	const int first, const int last, const int i
) {
	for (int k = first; k < last; k++)
		lshift_seam_ln(seams, info, k, i);
}

// Fugenverschiebung beim Wiedereinfuegen von Fuge n:
// Verschiebt in Zeile i fuer Fuge n
// alle anderen groesseren Fugen um eins nach rechts.
// Seams shift when re-inserting seam n:
// Shifts in line i for seam n all other greater seams by one to the right.
void rshift_seam_ln(
	struct seams_s *seams, const struct info_s *info,
	const int n, const int i
) {
	const seam_t s = seams->coord[n][i];
	for (int k = 0; k < info->si; k++)
		seams->shift[k][i] += (seams->coord[k][i] > s);
}

// Fugenverschiebung beim Wiedereinfuegen der Fugen first bis (last - 1):
// Verschiebt in Zeile i fuer jede Fuge aus [first bis (last - 1)]
// alle anderen groesseren Fugen um eins nach rechts.
// Seams shift when re-inserting seams first to (last - 1):
// Shifts in line i for seam n for all seams of [first bis (last - 1)]
// all other seams by one to the right.
void rshift_seams_ln(
	struct seams_s *seams, const struct info_s *info,
	const int first, const int last, const int i
) {
	for (int k = first; k < last; k++)
		rshift_seam_ln(seams, info, k, i);
}

/*
// Fugenverschiebung beim Wiederentfernen der Fugen first bis (last - 1):
// Verschiebt in Zeile i fuer jede Fuge aus [first bis (last - 1)]
// alle anderen groesseren Fugen und die Fuge selbst um eins nach links.
void ilshift_seams_ln(
	struct seams_s *seams, const struct info_s *info,
	const int first, const int last, const int i
) {
	for (int k = first; k < last; k++) {
		lshift_seam_ln(seams, info, k, i);
		seams->shift[k][i]--;
	}
}*/

// Fugenverschiebung beim Einfuegen der Fugen first bis (last - 1):
// Verschiebt in Zeile i fuer jede Fuge aus [first bis (last - 1)]
// alle anderen groesseren Fugen und die Fuge selbst um eins nach rechts.
// Seams shift when inserting seams first to (last - 1):
// Shifts in line i for seam n for all seams of [first bis (last - 1)]
// all other greater seams and the seam itself by one to the right.
void irshift_seams_ln(
	struct seams_s *seams, const struct info_s *info,
	const int first, const int last, const int i
) {
	for (int k = first; k < last; k++) {
		rshift_seam_ln(seams, info, k, i);
		seams->shift[k][i]++;
	}
}