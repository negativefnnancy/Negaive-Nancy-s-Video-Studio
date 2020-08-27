#include <stdio.h>
#include <stdarg.h>

#include "debug.h"

bool DEBUG = false;

#define DEBUG_STREAM stdout

#define DEBUG_PRINT()\
    va_list argp;\
    va_start (argp, format);\
    fprintf (DEBUG_STREAM, "[DEBUG] ");\
    vfprintf (DEBUG_STREAM, format, argp);\
    va_end (argp);

void _debug_printf (char *format, ...) {

    DEBUG_PRINT ();
}

void debug_printf (char *format, ...) {

    if (DEBUG) {

        DEBUG_PRINT ();
    }
}

