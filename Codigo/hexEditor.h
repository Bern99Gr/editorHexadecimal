#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <curses.h>
#include <sys/stat.h>
#include <sys/mman.h>

#ifndef hexEditor_h
#define hexEditor_h
#ifndef hexEditor_IMPORT
    #define EXTERN
#else
    #define EXTERN extern
#endif

EXTERN int hexEditor(char *filename);


#undef hexEditor_IMPORT
#undef EXTERN
#endif