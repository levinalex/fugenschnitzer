FUGENSCHNITZER 0.8 beta 2, command line version
FUGENSCHNITZER – Seam Carving for everyone.
Copyright (C) 2008/9 David Eckardt
http://fugenschnitzer.sourceforge.net/ 

Fugenschnitzer is a program for resizing images using the Seam Carving 
method. This command line version supports the PNG (Portable Network 
Graphics) image format. 

The Seam Carving method has been presented by A. Shamir and S. Avidan:
http://www.seamcarving.com


*** Usage

> fusch <source> <target> <value>

Source, target: PNG image file

Value:
  h#    - Horizontal image resizing to # pixels.
  h+#   - Horizontal image enlarging by # pixels.
  h-#   - Horizontal image shrinking by # pixels.
  v#
  v+# 
  v-#   - As above, but vertical.
  @+#:# - Image enlarging in one dimension to the aspect ratio #:#.
  @-#:# - Image shrinking in one dimension to the aspect ratio  #:#
          (width:height).
  %+#
  %-#   - Image enlarging/shrinking in one dimension to the aspect ratio #%
          (width/height).

Examples: 

> fusch image.png image2.png h500
Changes the horizontal image size in image.png to 500 pixels and saves 
the changed image in image2.png. 

> fusch image.png image2.png v+30
Vertically enlarges the image in image.png by 30 pixels and saves the 
changed image in image2.png. 


> fusch image.png image2.png @+16:9
Enlarges the image in image.png in one direction so that the image 
aspect ration becomes 16:9 (width:height). The changed image is saved 
surprizingly in image2.png. 

> fusch image.png image2.png %-133
Shrinks the image in image.png in one direction so that the image aspect 
ratio becomes 133% (width/height). (133% is the aspect ratio 4:3.) The 
changed image is saved in image2.png. 


*** Invoked program libraries

- libseamcarv (Windows: seamcarv.dll)
This library contains the Seam Carving method implementation an is 
written by the same author as the application program. 

- libpng12
PNG image file format library. 
http://libpng.org/ http://libpng.org/pub/png/libpng.html 

- zlib1 (Windows only)
Required by libpng.
http://www.zlib.net/

- pthreadGC2 (Windows only)
Multi threading library. 
http://sourceware.org/pthreads-win32/ 


*** License, Copying And Warranty

The FUGENSCHNITZER Program Library is published under the terms and 
conditions of the GNU Lesser General Public License 3. The 
FUGENSCHNITZER Application program which is basing on the FUGENSCHNITZER 
Program Library is published under the terms and conditions of the GNU 
General Public License 3. The license texts are in the file named 
license.txt. 

This program is free software: you can redistribute it and/or modify it 
under the terms of the GNU General Public License as published by the 
Free Software Foundation, either version 3 of the License, or (at your 
option) any later version. 

This program is distributed in the hope that it will be useful, but 
WITHOUT ANY WARRANTY; without even the implied warranty of 
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General 
Public License for more details. 

You should have received a copy of the GNU General Public License along 
with this program. If not, see <http://www.gnu.org/licenses/>. 
