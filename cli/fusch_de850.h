/*
/*
fusch_de.h
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
	"Verwendung: %s <Quelle> <Ziel> <Wert>\n",
	"Quelle, Ziel: PNG-Bilddatei\n"
	"Wert:\n"
	"  h#    - éndert die horizontale Bildgrî·e auf # Pixel. Beispiel: h500\n"
	"  h+#   - Vergrî·ert das Bild horizontal um # Pixel.    Beispiel: h+50\n"
	"  h-#   - Verkleinert das Bild horizontal um # Pixel.   Beispiel: h-50\n"
	"  v#\n  v+#\n  v-#   - Wie zuvor, allerdings vertikal.\n"
	"  @+#:# - Vergrî·ert das Bild in einer Dimension auf das SeitenverhÑltnis #:#.\n"
	"          Beispiel: @+16:9\n"
	"  @-#:# - Verkleinert das Bild in einer Dimension auf das SeitenverhÑltnis #:#.\n"
	"          Beispiel: @-16:9\n"
	"  %+#\n  %-#   - Vergrî·ert bzw. verkleinert das Bild in einer Dimension\n"
	"          auf das SeitenverhÑltnis #% (Breite/Hîhe). Beispiel: %+133\n",
	"Fugensuche: ges. erm.   % Fu/s    Dauer  verstr.   verbl.",
	"Bild wird geîffnet.",
	" - Fehler beim ôffnen",
	" - kein PNG-Bild oder beschÑdigt",
	"Interner Fehler",
	"PNG lesen fehlgeschlagen",
	"Palette lesen fehlgeschlagen.",
	"UngÅltige Dimensionsangabe.",
	"SeitenverhÑltnis erfordert Vorzeichen.",
	"Keine Grî·enÑnderung erforderlich.",
	"UngÅltiger Grî·enparameter oder Null angegeben.",
	"Grî·enÑnderung Åbersteigt Bildma·e.",
	"Bild wird geladen.",
	"Bildma·e",
	"vertikal",
	"horizontal",
	"Die Alpha-Markierung wurde nicht geladen.",
	"Bildgrî·e Ñndern: ",
	"Bild speichern: ",
	" - Fehler beim ôffnen (Schreiben)",
	"PNG schreiben fehlgeschlagen.",
	"fertig.",
	"Vorgang erfolgreich abgeschlossen.",
};
