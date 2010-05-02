/*
sc_core.h

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

#ifndef SC_CORE_H
#define SC_CORE_H

#include <stdbool.h>
#include "sc_defi.h"

void diff_ln(
	struct intn_s *diff_img, const struct intn_s *intn_img,
	const int i, const int hfirst, const int hlast
);

void diff(
	struct intn_s *diff_img, const struct intn_s *intn_img,
	const struct info_s *info
);
void diff_margins(
	struct intn_s *diff_img, const struct intn_s *intn_img,
	const struct info_s *info
);

void mark(
	struct intn_s *diff_img, struct mark_s *mark_img, const struct info_s *info
);
void mark_ln(
	struct intn_s *diff_img, const struct mark_s *mark_img,
	const int i, const int hfirst, const int hlast
);
void accu(struct intn_s *intn_img, const struct info_s *info);
void accu_ln(struct intn_s *diff_img, const int ln, const int hlast);

void trace_seam(
	struct intn_s *diff_img, struct intn_s *intn_img, struct mark_s *mark_img,
	seam_t *seam, struct info_s *info
);

#endif // SC_CORE_H