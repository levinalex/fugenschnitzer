/*
sc_defi.h

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

#ifndef SC_DEFI_H
#define SC_DEFI_H

#define SC_BUILDING

#include <stdbool.h>
#include <math.h>		// rint, abs, fabs, INFINITY
#include "seamcarv.h"

// GNU-spezifisches: Funktionsattribute 

#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 2)
#	define GCC_HOT  __attribute__ ((hot))
#	define GCC_COLD __attribute__ ((cold))
#else
#	define GCC_HOT
#	define GCC_COLD
#endif

// intn_t - Ganzzahl oder Flieﬂkomma: Anpassen von Funktionen und Konstanten

#ifdef INTN_INT
#	include <limits.h>
#	define INTN_T long int	// <=
#	define RINT lrintf
#	define ABS labs
#	define WEIGHT_CORNER 2l
#	define INTN_INFTY LONG_MAX
#	define _COMP_WEIGHT_R_ 76.54f
#	define _COMP_WEIGHT_G_ 150.27f
#	define _COMP_WEIGHT_B_ 29.18f
#else
#	define INTN_T float		// <=
#	define RINT
#	define ABS fabs
#	define WEIGHT_CORNER 2.0f
#	define INTN_INFTY INFINITY
#	define _COMP_WEIGHT_R_ .299f
#	define _COMP_WEIGHT_G_ .587f
#	define _COMP_WEIGHT_B_ .114f
#endif

// Typdefinitionen

typedef INTN_T intn_t;
typedef long int seam_t;

// Bitfields mit Nicht-int-Typen sind eine GNU-Erweiterung.

#ifdef __GNUC__
#	define BIFI_BOOL __extension__ bool
#	define BIFI_ENUM __extension__ enum
#else
#	define BIFI_BOOL unsigned int
#	define BIFI_ENUM unsigned int
#endif

enum imark_e {IMARK_NONE, IMARK_MARK, IMARK_ALPHA};

struct flags_s {
	BIFI_BOOL	   transposed: 1,
					 interpol: 1;
	BIFI_ENUM imark_e    mark: 2;
	BIFI_ENUM color_e	color: 4;	// => seamcarv.h
};

struct info_s {
	long int height, width, original_width, ext_width,
		pheight, pwidth, original_pwidth,
		maxhw, minhw;
	int	si, sc, zoom;
	struct flags_s flags;
};

struct comp_s {comp_t *data, **px, *tmp;};
struct intn_s {intn_t *data, **px;};
struct mark_s {mark_t *data, **px, *tmp;};
struct seams_s {seam_t *data, **coord, **shift, *tmp;};

#endif // SC_DEFI_H
