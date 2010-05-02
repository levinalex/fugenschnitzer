/*
sc_color.h

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

#ifndef SC_COLOR_H
#define SC_COLOR_H

#include "sc_defi.h"

void load_comp_py_rgb(
	struct comp_s *image, const uint8_t *image_in, const struct info_s *info,
	const bool rgb32
);

void zoom_comp_ln_py_rgb(
	uint8_t *img_out, const struct comp_s *image, const struct info_s *info,
	const int i, const int ch, const int d
);

void eject_comp_ln_py_rgb(
	uint8_t *img_out, const struct comp_s *image, const struct info_s *info,
	const int i, const int ch, const int d
);

void preview_comp_py_rgb(
	uint8_t *image_out, struct comp_s *image, const struct info_s *info,
	const bool zoom, const bool rgb32
);

void eject_comp_py_rgb(
	uint8_t *img_out, const struct comp_s *image, const struct info_s *info,
	const bool rgb32
);

void display_seams_py_rgb(
	uint8_t *image_out, const struct seams_s *seams, const struct info_s *info,
	const bool zoom,
	const int first, const int last, const uint8_t color[], const bool rgb32
);

void py_rgb_px(uint8_t *img_out, comp_t px, const int ch);

void rgb_to_intn(
	struct intn_s *intn_img, const struct comp_s *comp_img,
	const struct info_s *info
);

comp_t comp_interpol(const comp_t *c, const struct info_s *info);

comp_t zoom_sample(
	const int i, const int j, const float d,
	const struct comp_s *image, const struct info_s *info
);

#endif // SC_COLOR_H
