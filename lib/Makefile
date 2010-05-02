# Makefile fuer die FUGENSCHNITZER-Programmbibliothek
# Dieses Makefile erzeugt die Programmbibliotheksdatei
#   "seamcarv.dll"   (Windows) oder
#   "seamcarv.dylib" (OSX)     oder
#   "libseamcarv.so" (Linux).
# WICHTIGE HINWEISE:
# 1. Quelltext
# Die Quelltextdateien muessen sich entweder im selben Verzeichnis wie dieses
# Makefile befinden oder im Unterverzeichnis "lib".
# 2. Plattformen
# VOR dem Erzeugen der FUGENSCHNITZER-Programmbibliothek muss ZUERST
# einer der Plattformspezifikationsbloecke aktiviert werden. Es gibt einen Block fuer
# Windows, einen fuer OSX und einen fuer Linux. Der Anfang ist durch "# *** <Plattform>"
# markiert. Der Anfang eines Blocks ist das Ende des vorausgehenden; der letzte endet
# mit "# ***". Das Aktivieren eines Blocks geschieht durch Entfernen der
# Auskommentierung.
# Das Erzeugen von seamcarv.dll/seamcarv.dylib/libseamcarv.so erfolgt dann mit
# "make" oder "make lib".
#
# Makefile for the FUGENSCHNITZER Program Library
# This makefile builds the program library file 
#   "seamcarv.dll"   (Windows) or
#   "seamcarv.dylib" (OSX)     or
#   "libseamcarv.so" (Linux).
# IMPORTANT NOTES:
# 1. Source Code
# The source code files must be present either in the same directory as this
# Makefile or in the "lib" subdirectory.
# 2. Platforms
# 2. BEFORE building the FUGENSCHNITZER Program Library one of the
# platform specification blocks must be activated FIRST. There is one block for
# Windows, another one for OSX und one for Linux. The beginning is denoted by
# "# *** <platform>". Each block's beginning denotes the end of the preceding;
# the last one ends at "# ***". Block activation is done by uncommenting.
# After that, build seamcarv.dll/seamcarv.dylib/libseamcarv.so by entering
# "make" or "make lib".

# CC      -- command line for compiler
# RM      -- remove command (Windows: del, Unix-like: rm)
# WINDRES -- command line for resources compiler (Windows)
#
#  CFLAGS -- command line parameters for compiling:
#            -c -std=c99 -DBUILDING_LIB are essential;
#            -fno-common is essential (OSX);
#            -Ox may be appended individually
# LDFLAGS -- command line parameters for linking; shared library parameter is
#            essential: -shared (Windows/Linux) or -dynamiclib (OSX)
#
# LIB  -- file name of the program library to build; this is the linker output
# OBJ  -- compiler output files; the library is built by linking these
# O3   -- source code files to be compiled using maximum optimization -O3
# Os   -- source code files to be compiled without using maximum optimization
# Ores -- resource file for version information (Windows)
#
# lib     -- Make target for building the library without clean/cleanup
# obj     -- Make target for compiling the source code without clean
# clean   -- delete compiler and linker output
# cleanup -- delete compiler output

CC             = gcc

# *** Windows

# LIBSUFFIX      = .dll
# LIBPREFIX      = 
# ores           = scdllres.o


# RM             = del
# WINDRES        = windres

# CFLAGS         = -c -std=c99 -DBUILDING_LIB -pedantic -Wextra -Os -march=native
# LDFLAGS        = -shared -s
# TESTFLAGS      = -std=c99 -DBUILDING_LIB -shared

# *** OSX

# LIBSUFFIX      = .dylib
# LIBPREFIX      = 
# ores           = 

# RM             = rm
# WINDRES        = 

# CFLAGS         = -c -std=c99 -DBUILDING_LIB -pedantic -Wextra -Os -arch i386 -arch ppc
# LDFLAGS        = -dynamiclib -Wl,-x -arch i386 -arch ppc
# TESTFLAGS      = -std=c99 -DBUILDING_LIB -dynamiclib

# *** Linux

LIBSUFFIX      = .so
LIBPREFIX      = lib
ores           = 

RM             = rm
WINDRES        = 

CFLAGS         = -c -std=c99 -DBUILDING_LIB -pedantic -Wextra -Os -fPIC
LDFLAGS        = -shared

# ***

VPATH          = src

LIBBASENAME    = seamcarv

O3             = sc_core.c sc_carve.c sc_color.c sc_shift.c
Os             = sc_mgmnt.c seamcarv.c
Ores           = scdllres.rc
OBJ            = o3 os ores
ALLOBJ         = *.o

LIB            = $(LIBPREFIX)$(LIBBASENAME)$(LIBSUFFIX)

.PHONY: all obj o3 os ores clean cleanup test

all: clean lib cleanup

lib: $(LIB)

$(LIB): $(OBJ)
	@echo Linking...
	$(CC) $(ALLOBJ) -o $@ $(LDFLAGS)
	@echo $(LIB) has been successfully built.

o3: $(O3)
	$(CC) $^ $(CFLAGS) -O3
os: $(Os)
	$(CC) $^ $(CFLAGS)

ores: $(ores)
$(ores): $(Ores)
	@echo $(WINDRES) $< $@

obj: $(OBJ)

cleanup:
	-$(RM) $(ALLOBJ)

clean:
	-$(RM) $(ALLOBJ) $(LIB)

test:
	$(CC) *.c $(TESTFLAGS) -o $(LIB)
