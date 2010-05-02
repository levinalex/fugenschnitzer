/*
fusch.h
Diese Quelltextdatei ist Bestandteil des FUGENSCHNITZER-Anwendungsprogramms.

Das FUGENSCHNITZER-Anwendungsprogramm ist ein Seam-Carving-Programm, das 
auf der FUGENSCHNITZER-Programmbibliothek basiert. 

FUGENSCHNITZER -- Seam Carving fuer jedermann.
http://fugenschnitzer.sourceforge.net
Copyright (C) 2008/9 David Eckardt

Dieses Programm ist freie Software. Sie koennen es unter den Bedingungen
der GNU General Public License, wie von der Free Software
Foundation veroeffentlicht, weitergeben und/oder modifizieren, entweder
gemaess Version 3 der Lizenz oder (nach Ihrer Option) jeder spaeteren
Version.
Die Veroeffentlichung dieses Programms erfolgt in der Hoffnung, dass es
Ihnen von Nutzen sein wird, aber OHNE IRGENDEINE GARANTIE, sogar ohne
die implizite Garantie der MARKTREIFE oder der VERWENDBARKEIT FUER EINEN
BESTIMMTEN ZWECK. Details finden Sie in der GNU General Public License.
Sie sollten ein Exemplar der GNU General Public License zusammen mit
diesem Programm erhalten haben. Falls nicht, siehe
http://www.gnu.org/licenses/gpl-3.0.html
http://www.gnu.de/documents/gpl-3.0.de.html
.

This source code file is a part of the FUGENSCHNITZER Application Program.

The FUGENSCHNITZER Application Program Library is a Seam Carving program 
basing on the FUGENSCHNITZER program library. 

FUGENSCHNITZER -- Seam Carving for everyone.
http://fugenschnitzer.sourceforge.net
Copyright (C) 2008/9 David Eckardt

This program is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.
This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
Public License for more details.
You should have received a copy of the GNU General Public License along
with this program. If not, see
http://www.gnu.org/licenses/gpl-3.0.html
.
*/

enum {
	I_HELP1,
	I_HELP2,
	I_COMPUTING_TABLE,
	I_OPEN,
	E_OPEN,
	E_CORRUPTED,
	E_GENERIC,
	E_READ,
	E_READPAL,
	E_DIM,
	E_SIGN,
	I_NOTHINGTODO,
	E_PAR,
	E_SIZE,
	I_LOAD,
	I_MEASURES,
	I_VERT,
	I_HORI,
	E_ALPHA,
	I_RESIZING,
	I_SAVE,
	E_SAVE,
	E_WRITE,
	I_FINISHED,
	I_COMPLETED,
};

char *MSG[] = {
	"Fugenschnitzer 0.8 beta 2\n"
	"http://fugenschnitzer.sourceforge.net\n\n"
	"Usage: %s <source> <target> <value>\n",
	"source, target: PNG image file\n"
	"value:\n"
	"  h#    - Horizontal image resizing to # pixels. Example: h500\n"
	"  h+#   - Horizontal image enlarging by # pixels. Example: h+50\n"
	"  h-#   - Horizontal image shrinking by # pixels. Example: h-50\n"
	"  v#\n  v+#\n  v-#   - As above, but vertical.\n"
	"  @+#:# - Image enlarging in one dimension to the aspect ratio #:#.\n"
	"          Example: @+16:9\n"
	"  @-#:# - Image shrinking in one dimension to the aspect ratio  #:#.\n"
	"          Example: @-16:9\n"
	"  %+#\n  %-#   - Image enlarging/shrinking in one dimension\n"
	"          to the aspect ratio #% (width/height). Example: %+133\n",
//	"Fugensuche: ges. erm.   %% Fu/s    Dauer  verstr.   verbl.",
	"Computing:  totl curr   % se/s duration  elapsed  remaing",
	"Opening image",
	" - error opening",
	" - no PNG image or corrupted",
	"Internal error",
	"Failed reading PNG",
	"Failed reading palette",
	"Invalid dimension.",
	"Aspect ratio requires sign.",
	"Resizing not neccessary.",
	"Invalid size parameter or zero.",
	"Resizing exceeds image measures.",
	"Loading image",
	"Measures",
	"vertical",
	"horizontal",
	"Alpha mark was not loaded.",
	"Resizing: ",
	"Saving image: ",
	" - Error opening (write)",
	"Failed writing PNG",
	"finished.",
	"Successfully completed.",
};
