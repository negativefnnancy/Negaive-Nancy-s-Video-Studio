#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <stdbool.h>

#include <cairo.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

extern bool DEBUG;

/* debug logging functions */
void _debug_printf (char *format, ...);
void debug_printf (char *format, ...);

/* exit the program after printing a formatted message to stderr */
void die_with_message (char *format, ...);

/* splice out an element from an array of pointers if found */
void remove_array_item (void **array, size_t *n_items, void *item);

/* multiply matrix by scalar */
void cairo_matrix_scalar_multiply (cairo_matrix_t *matrix, double scalar);

#endif /* UTIL_H */
