The FUGENSCHNITZER Program Library
FUGENSCHNITZER -- Seam Carving for everyone
http://fugenschnitzer.sourceforge.net
Copyright (C) 2008/9 David Eckardt
nablakreuz@users.sourceforge.net

1. Introducing the FUGENSCHNITZER Program Library

Welcome to FUGENSCHNITZER! I'm glad to see you being interested in my
small Seam Carving software project. If you never heard about Seam
Carving, visit http://www.seamcarving.com/ from the Seam Carving
inventors Ariel Shamir and Shai Avidan and be fascinated when watching
the presentation movie.

Fugenschnitzer (by the way -- "Fugen" means "seams" in German and "Schnitzer"
means "carver" so "Fugenschnitzer" is "Seam Carver") is intended to be a
usefull small Seam Carving program library with the following advances:

- It is written in ISO C99, not relying on any compiler, platform or
hardware specific extensions like compiler extensions, API functions,
or Assembler blocks. The source code, howewer, does use a few GNU compiler
extensions and Windows extensions, but these are not essentially neccessary
and only used for optimizing. Furthermore, they are disabled when compiling
in an incompatible environment.

- It uses only the C Standard Library stdlib and therefore does not
depend on any prerequisites not provided by a hosted C99 compiler. That
means, however, that this library will not be able to read images from
commom image file formats like PNG or JPEG. That sounds first quite sad
but is in fact no serious disadvantage since it also means that the
library is not bound on a specific image file format but the raw image
data is passed between the library and the invoking program. On the one
hand this means a much higher flexibility/reliability ratio to the
Fugenschnitzer library because we don't have to maintain several
interface functions, on the other hand is is a relatively easy thing to
write a program that uses specific image format libraries and invokes
the Fugenschnitzer library.


- It is small, flexible and free. The built library file (Windows DLL)
is below 30 kB. The source code is below 3000 lines in 14 files at this
time. The features implemented and used yet include preparations for
user interaction, easy multi-threading/parallel processing and a simple
image downscale function to provide a display fitting preview when
processing a large image. As written below, the Fugenschnitzer library
is licensed under the LGPL to provide a free software that may -- as a
library -- also eventually be included in non-free programs.


2. GCC For Microsoft Windows And Mac OS X

a) Windows
There are a few concurrenting Windows ports of GCC. The one I like best
is MinGW, the Minimalist GNU for Windows. The most recent "experimental"
MinGW builds on

    http://tdragon.net/recentgcc/

work very well, at least with Fugenschnitzer. GCC and Make are needed,
the binutils strongly recommended. After installing MinGW you may need
to manually add the MinGW "bin" directory to your system's PATH
environment variable. To access the environment variables, press
Win+Break to open the system properties, then select Advanced and hit
the Environment Variables button.

b) OS X
Mac OS X is based on NeXTStep and BSD, both Unix-related operating
systems. All the Unix-like OSes like the Unixes, GNU/Linux, Solaris, and
the BSD family traditionally carry along their own C compiler since C
and Unix have a close historical relationship. Nevertheless, it is not
installed on OS X by default. If you don't have the OS X setup disks,
you have to become a registered Apple Developer Connection member, tell
them your blood group and shoe size until you can download the OS X
Developer Tools which include GCC. The installation package is of
approx. 1 GB size (TDM releases: 55 MB). Maybe you manage building GCC
on Darwin from sources on your own. I didn't.

3. Building the library
I always used the GNU Compiler GCC to build the library since GCC is the
native compiler on Linux and Darwin (OSX). It certainly can also be done
using other compilers like Microsoft Visual C.
I didn't try a GCC version older than 4.0. First I developed
Fugenschnitzer on Windows using GCC 4.2 from the MinGW TDM release
mentioned above. Everything was fine. After trying to build the library
on Linux and OS X, I had to understand the sad fact that GCC 4.0 is
still the most recent version on these Unix-like OSes and spent days
implementing backward compatibility.

Here is a brief description of the things to notice when building a
program library from C source code on different platforms using gcc.


a) Shared Or Dynamic Libraries
In the C world there are two types of libraries: static and shared or
dynamic libraries. A static library contains the compiler output
(so-called "object code") to be linked later together with other
compiler output units to a solid executable program binary.
A shared or dynamic library is similar to an executable program but not
executable on its own. It is loaded on demand by the operation system
when an application program requests uning a function provided by the
library. Since several programs can request using this library at the
same time, libraries like these are called "shared" libraries. Often
they are called "dynamic" because their code is not statically included
in the program that uses it but they are loaded dynamically at run-time.

b) Missing "main" In Library Source Code
Basically, there is exactly one fundamental difference between a regular
program and a program library written in C: A library does not contain
the "main" function, at least not neccessarily.
When building the library, the linker must be informed about this fact
to prevent aborting the linking process with a "missing 'main'" error
message.

c) Two-stage Build Process
The Fugenschnitzer library is built in two steps: compiling and linking.
The only reason for this is that I like it better to use different
compiler optimization parameters for some source code units than for
others. Aside that, compiling and linking in separate steps is the usual
procedure.

d) Shared/Dynamic Libraries On Windows, OS X And Linux
As said above, the difference between a regular application program and
a program library is small and, apart from platform dependent issues,
consists in the fact that there's no "main". Anyway, the terminology is
a bit confusing. To make things a little more clear, the names for
shared/dynamic libraries on the main platforms are listed here:

Platform           Name                    File Name Suffix

Microsoft Windows  Dynamic Linked Library  .dll
Apple Mac OS X     Dynamic Library         .dylib
GNU/Linux          Shared Object           .so

e) Function Visibility Control
A program library binary file contains a kind of table of contents where
its symbols are listed. This is called the "symbol table". The library's
symbols are its functions and global variables declared anywhere in the
source code. As every program, the Fugenschnitzer library contains many
functions. Most of them are used internally and only a few are intended
to be used by the invoking program. When building the library, all
symbols are listed in the symbol table. As one can imagine, this leads
to a symbol table much longer than needed. Thus it would be nice if we
could declare the functions as "visible" or "hidden" to get only the
export functions be listed in the symbol table. Unfortunately, the C
language itself does not provide such a feature. However, Microsoft
introduced this feature as a C language extension a long time ago. To
tell the Microsoft C compiler that a certain function should be exported
from a DLL, its name is preceeded by

__declspec(dllexport)

. When building a program that invokes a DLL, the function's declaration name
should be preceeded by __declspec(dllimport).
GCC has adopted this feature on Windows. When building on other platforms,
we can use a GNU extension introduced with GCC 4. To define a function
visible, we preceed its name by

__attribute__((visibility("default")))

. Consequently, we would have to declare _any_ other global functions
and variables hidden by preceding

__attribute__((visibility("hidden")))

. Fortunately, we can tell GCC to do that by adding -fvisibility=hidden
to the command line when compiling. GCC then hides all global symbols
whose visibility is not explicitely set to "default".


f) Library Building Parameters For GCC
As described in d) we want to compile the source code first and link the
compiler output in a separate step later. To prevent GCC from linking it
right now, we add the "-c" command line switch. Additionally, we must
tell GCC that this is C99 code by adding "-std=c99" and define the
preprocessor macro BUILDING_LIB (this is used in seamcarv.h and toggles
the function DLL im/export attribute used on Windows) by adding
"-DBUILDING_LIB". Together with the source code files *.c to be compiled
we have the command line for simple compiling:


> gcc *.c -c -std=c99 -DBUILDING_LIB

GCC now compiles each source code file *.c into an object code file *.o.
In the second step we link these together to the library. As described
in b) we have to inform GCC about building a library. This is done by
adding the command line switch "-shared" on Windows and Linux and
"-dynamiclib" on OS X. We also should set an output file name. The
output file name is given to GCC by "-o <filename>". According to the
platform conventions, we name the library

   seamcarv.dll     on Windows,
libseamcarv.dylib   on Mac OS X,
libseamcarv.so      on GNU/Linux.

The linking command line is...
... for Windows:   > gcc *.o -shared -o seamcarv.dll
... for Mac OS X:  > gcc *.o -dynamiclib -o libseamcarv.dylib
... for GNU/Linux: > gcc *.o -shared -o libseamcarv.so

We're done! Anyway, for testing and troubleshooting purposes, these are the
command lines for compiling and linking the library in a single step:
Windows:   > gcc *.c -std=c99 -DBUILDING_LIB -shared -o seamcarv.dll
Mac OS X:  > gcc *.c -std=c99 -DBUILDING_LIB -dynamiclib -o seamcarv.dylib
GNU/Linux: > gcc *.c -std=c99 -DBUILDING_LIB -shared -o libseamcarv.so

For more information, see
- the GCC manual              : http://gcc.gnu.org/onlinedocs/
- for visibility control      : http://gcc.gnu.org/wiki/Visibility
- special issues on OS X
  (i386/ppc thin/fat binaries):
  http://developer.apple.com/documentation/MacOSX/Conceptual/universal_binary/

That's all for now. Currently, the source code documentation is quite
poor and the library interface is not documented at all. Much, much
documentation will follow very soon (I hope).

eDavid



The FUGENSCHNITZER Program Library is subject to the
GNU Lesser General Public Licence (Version 3):
http://www.gnu.org/licenses/lgpl-3.0.html
http://www.gnu.org/licenses/gpl-3.0.html

The FUGENSCHNITZER Program Library is a Seam Carving program library.
FUGENSCHNITZER -- Seam Carving for everyone.

Copyright (C) 2008/9 David Eckardt

This program is free software: you can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.