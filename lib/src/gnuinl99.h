/*
gnuinl99.h

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


 /*	Diese Makros sorgen fuer die richtige Semantik fuer inline-Funktionen unter
 *	GNU C ISO C99. Mehr dazu weiter unten.
 *	Wird ein anderer Compiler als GCC verwendet, erzeugen die Makros die
 *	ISO-C99-Semantik. Standalone-Code wird in jedem Fall erzeugt.
 *
 *	Verwendung:
 *    - Jede C-Quellcodedatei, deren Header Inline-Funktionen beinhaltet, muss
 *      ein eigenes Makro definieren und ihren Header einfuegen.
 *   -  Ihr Header muss zuerst das Makro INLINE_INTERN definieren und sofort
 *      danach gnuinl99.h einfuegen. INLINE_INTERN muss genau dann "wahr"
 *      (<=> ungleich Null) sein, wenn das eigene Makro nicht definiert ist.
 *      Hinweis: gnuinl99.h entdefiniert (#undef) INLINE_INTERN wieder.
 *   -  gnuinl99.h definiert die Makros EXTERN und INLINE, die zum einen dafuer
 *      sorgen, dass die Inline-Funktionsdeklarationen und -definitionen
 *      a) an der richtigen Stelle auftreten,
 *      b) die richtigen Attribute tragen.
 *    - Die Inline-Funktionsdeklarationen und -definitionen erfolgen im Header
 *      nach dem Einfuegen von gnuinl99.h unter Verwendung der Makros EXTERN und
 *      INLINE. Siehe dazu folgendes Beispiel.
 **** beispiel.c **************************************************************
 * // Definiere eigenes Makro BEISPIEL_INLINE_INTERN und fuege Header ein.
 * #define BEISPIEL_INLINE_INTERN
 * #include beispiel.h
 *
 * (...)
 **** beispiel.h **************************************************************
 * // Definiere INLINE_INTERN und fuege inline.h ein.
 * #define INLINE_INTERN defined BEISPIEL_INLINE_INTERN
 * #include INLINE_INTERN
 *
 * // Deklariere Inline-Funktionen:
 * #ifdef EXTERN
 * EXTERN int func1(int a, int b);
 * EXTERN int func2(int c);
 * #endif
 *
 * // Definiere Inline-Funktionen:
 * #ifdef INLINE
 * INLINE int func1(int a, int b) {...}
 * INLINE int func2(int c) {...}
 * #endif
 *
 * (...)
 ******************************************************************************
 *	Zu den verschiedenen Semantiken fuer inline:
 *	Das Schluesselwort "inline" gibt es erst seit C99. GNU C unterstuetzt "inline"
 *	als Erweiterung allerdings schon laenger, und zwar mit einer anderen Semantik
 *	als der, die ISO C99 vorschreibt. Das ist die sog. GNU89-Inlinesemantik.
 *	Genaueres dazu unter:
 *		http://www.greenend.org.uk/rjk/2003/03/inline.html
 *  und __natuerlich__ im aktuellen GCC-Handbuch!
 *      http://gcc.gnu.org/onlinedocs/
 *
 *	Semantik	|.h								|.c
 *	------------|-------------------------------|--------------------
 *	ISO C99		|inline <Definition>			|extern <Deklaration>
 *	GNU			|extern inline <Definition>		|<Definition>
 *
 *	GCC ab 4.1.3 verwendet im C99-Modus die ISO-C99-Semantik.
 *	Die frueheren Versionen verwenden immer die GNU-Semantik.
 *	Ab Version 4.1.3 gibt es die Makros
 *		__GNUC_STDC_INLINE__ und
 *		__GNUC_GNU_INLINE__:
 *
 *	defined __GNUC_STDC_INLINE__ <=> ISO-C99-Semantik wird verwendet.
 *	defined __GNUC_GNU_INLINE__  <=> GNU-Semantik wird verwendet.
 *
 *  UEbrigens: Wer Lust hat, kann auch unter GCC 4.1.3 und spaeter die
 *  GNU89-Inlinesemantik einschalten, und zwar mit dem Kommandozeilen-
 *  parameter -fgnu89-inline oder dem Funktionsattribut ((gnu_inline)).
 */

// Aufraeumen
#ifdef INLINE
#	undef INLINE
#endif

#ifdef EXTERN
#	undef EXTERN
#endif

#ifdef INLINE_GNU
#	undef INLINE_GNU
#endif

// INLINE_GNU <=> GNU89-Semantik
#if defined __GNUC_GNU_INLINE__ || defined __GNUC_STDC_INLINE__
#	define INLINE_GNU defined __GNUC_GNU_INLINE__
#else
#	define INLINE_GNU defined __GNUC__
#endif

#ifndef INLINE_INTERN
#	define INLINE_INTERN 0
#endif
#ifndef INLINE_DISABLED
#	if INLINE_INTERN
#		if INLINE_GNU	// GNU89 intern:
#			define INLINE inline // "inline"-Funktionsdefinition
#			define EXTERN // normale Funktionsdeklaration
#		else			// C99 intern:
#			define INLINE inline // "inline"-Funktionsdefinition
#			define EXTERN extern // "extern"-Funktionsdekl. fuer Stand-alone
#		endif
#	else
#		if INLINE_GNU	// GNU89 extern:
#			define INLINE extern inline // "extern inline"-Funktionsdefinition
#			define EXTERN // normale Funktionsdeklaration
#		else			// C99 extern:
#			define INLINE inline // "inline"-Funktionsdefinition, keine Dekl.
#		endif
#	endif
#else					// kein Inline
#	define EXTERN		// extern: normale Funktionsdeklaration
#	if INLINE_INTERN	// intern:
#		define INLINE	// normale Funktionsdefinition
#	endif
#endif

#undef INLINE_GNU
#undef INLINE_INTERN