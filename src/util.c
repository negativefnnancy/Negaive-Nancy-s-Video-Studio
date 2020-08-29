#include <stdio.h>
#include <stdarg.h>

#include "util.h"

color_t make_color (double r, double g, double b, double a) {

    color_t color;
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;
    return color;
}

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

void die_with_message (char *format, ...) {

    va_list argp;
    va_start (argp, format);
    vfprintf (stderr, format, argp);\
    va_end (argp);
    exit (EXIT_FAILURE);
}

void remove_array_item (void **array, size_t *n_items, void *item) {

    /* index of item in array */
    size_t i;

    /* find the index of the item to be removed from the array */
    for (i = 0; i < *n_items; i++)
        if (array[i] == item) {
            
            /* found it */
            (*n_items)--;
            break;
        }

    /* note this loop only gets entered if item was found in array */
    for (; i < *n_items; i++)

        /* shift back all the items after the removed item */
        array[i] = array[i + 1];
}

void cairo_matrix_scalar_multiply (cairo_matrix_t *matrix, double scalar) {

    matrix->xx *= scalar;
    matrix->yx *= scalar;
    matrix->xy *= scalar;
    matrix->yy *= scalar;
    matrix->x0 *= scalar;
    matrix->y0 *= scalar;
}
