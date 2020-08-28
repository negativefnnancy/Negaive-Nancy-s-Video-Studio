#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <stdbool.h>

extern bool DEBUG;

/* debug logging functions */
void _debug_printf (char *format, ...);
void debug_printf (char *format, ...);

/* exit the program after printing a formatted message to stderr */
void die_with_message (char *format, ...);

/* splice out an element from an array of pointers if found */
void remove_array_item (void **array, size_t *n_items, void *item);

#endif /* UTIL_H */