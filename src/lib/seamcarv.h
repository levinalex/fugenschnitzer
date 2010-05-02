/*
seamcarv.h

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


#ifndef SEAM_CARV_DLL_H
#define SEAM_CARV_DLL_H

// http://gcc.gnu.org/wiki/Visibility
#if defined WIN32 || defined _WIN32 || defined __CYGWIN__
	#ifdef BUILDING_LIB
		#define LIB_PUBLIC __declspec(dllexport)
	#else
		#define LIB_PUBLIC __declspec(dllimport)
	#endif
	#define LIB_LOCAL
#else
	#if __GNUC__ >= 4
		#define LIB_PUBLIC __attribute__ ((visibility("default")))
		#define LIB_LOCAL  __attribute__ ((visibility("hidden")))
	#else
		#define LIB_PUBLIC
		#define LIB_LOCAL
	#endif
#endif

#include <stdbool.h>
#include <limits.h> // CHAR_BIT
#include <stdint.h> // uint32_t, uint8_t, UINT32_MAX

#define COMP_CHANNELS_MAX 4
#define COMP_BITMASK_FULL UINT32_MAX
#define COMP_MAX_BITS (int)(CHAR_BIT * sizeof(comp_t))

typedef uint32_t comp_t;
typedef uint8_t  mark_t;

enum  color_e {
		COLOR_RGB24 = 0,
		COLOR_RGB32 = 1,		
		COLOR_GRAY  = 2,
		COLOR_YC    = 3
};

enum   mark_e {MARK_KEEP, MARK_LOAD, MARK_ALPHA, MARK_CLEAR};

LIB_PUBLIC void sc_init(void);
LIB_PUBLIC void sc_close(void);

LIB_PUBLIC bool sc_load(
	const comp_t *image, long int *width, long int *height, int zoom
);

LIB_PUBLIC bool sc_load_py_rgb(
	const uint8_t *image, long int *width, long int *height, const int zoom,
	const bool rgb32
);

LIB_PUBLIC void sc_eject(comp_t *image);
LIB_PUBLIC void sc_eject_py_rgb(uint8_t *image, const bool rgb32);

LIB_PUBLIC void sc_preview(comp_t *image);
LIB_PUBLIC void sc_preview_py_rgb(
	uint8_t *image, const bool zoom, const bool rgb32
);

/*	
 *	Rückgabewert:
 *	0:	Das Bild wurde nicht erweitert.
 *	1:	Das Bild wurde erweitert.
 *	-1:	Fataler Fehler: Speicheranforderung fehlgeschlagen
 */
LIB_PUBLIC int sc_extend(
	const int extend,
	long int *width, long int *height, long int *pwidth, long int *pheight
);

LIB_PUBLIC bool sc_fix(
//	comp_t *image, mark_t *mark,
	const bool restore,
	long int *width, long int *height, long int *pwidth, long int *pheight
);

/*
 *	Rückgabewert:
 *
 *	0:	Das Bild wurde nicht erweitert und die Bilddaten nicht verschoben.
 *		Die Maske wurde geladen, falls angefordert.
 *	1:	Das Bild wurde erweitert und die Bilddaten verschoben.
 *	2:	Obwohl angefordert, wurde die Maske nicht geladen. Nur möglich bei
 *		Alpha-Maske.
 *	3:	1 + 2
 *	-1:	Fataler Fehler: Speicheranforderung fehlgeschlagen
 */

LIB_PUBLIC int sc_prepare(
	const bool vertical, const int extend, const bool interpol,
	const enum mark_e mmode, const mark_t *mark,
	long int *width, long int *height, long int *pwidth, long int *pheight
);

LIB_PUBLIC bool sc_seam(long int last);
LIB_PUBLIC long int sc_seam_progress(void);
LIB_PUBLIC void sc_seam_cancel(void);
/*
LIB_PUBLIC long int sc_carve(
	long int nom,
	long int *width, long int *height, long int *pwidth, long int *pheight
);
*/
LIB_PUBLIC long int sc_carve_py_rgb(
	uint8_t *img_out, long int nom,
	long int *width, long int *height, long int *pwidth, long int *pheight,
	const bool zoom, const bool rgb32
);

LIB_PUBLIC long int sc_carve_progress(void);

/*
LIB_PUBLIC void sc_show_mark(mark_t *mark);
LIB_PUBLIC void sc_eject_mark(mark_t *mark);
LIB_PUBLIC void sc_clear_mark(void);
*/

LIB_PUBLIC void sc_display_seams_py_rgb(
	uint8_t *image, int first, int last, const bool zoom,
	const uint8_t color[], const bool rgb32
);

LIB_PUBLIC int sc_carve_py_rgb_paral_init(
	int nom, const int threads, const bool zoom, const bool rgb32
);
LIB_PUBLIC void sc_carve_py_rgb_paral(uint8_t *image, const int thread);
LIB_PUBLIC long int sc_carve_py_rgb_paral_finish(
	uint8_t *image,
	long int *width, long int *height, long int *pwidth, long int *pheight
);


LIB_PUBLIC void sc_seam_paral_init(void);
LIB_PUBLIC int  sc_seam_paral_start(long int samples, int threads);
LIB_PUBLIC void sc_seam_paral_diff(const int part, const int thread);
LIB_PUBLIC bool sc_seam_paral_accu(const int part);
LIB_PUBLIC long int sc_seam_paral_finish(void);
LIB_PUBLIC void sc_seam_paral_close(void);

#endif // SEAM_CARV_DLL_H
