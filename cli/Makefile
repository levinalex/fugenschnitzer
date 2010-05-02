CC             = gcc

# *** Linux
# CFLAGS         = -std=c99 -Os -pthread -D_XOPEN_SOURCE=500
# LDFLAGS        = -lm -lpng12 -lseamcarv -L. -Wl,-R./ -s
# RM             = rm
# *** OSX
# CFLAGS         = -std=c99 -Os
# LDFLAGS        = -lpng -lseamcarv -L. -s
# RM             = rm
# *** Windows
# CFLAGS         = -std=c99 -Os -U__STRICT_ANSI__
# LDFLAGS        = -lpng -lseamcarv -lpthread -L. -s
# RM             = del
# ***


fusch: fusch.c
	-$(CC) $^ $(CFLAGS) $(LDFLAGS) -o $@

# Fugenschnitzer German/deutsch, UTF-8
	
de: fusch.c
	-$(CC) $^ $(CFLAGS) -DDE $(LDFLAGS) -o fusch

# Fugenschnitzer German/deutsch, CP850 (Windows)

de850: fusch.c
	-$(CC) $^ $(CFLAGS) -DDE850 $(LDFLAGS) -o fusch

clean:
	-$(RM) fusch fusch.exe
