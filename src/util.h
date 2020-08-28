#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <stdbool.h>

extern bool DEBUG;

/* debug logging functions */
void _debug_printf (char *format, ...);
void debug_printf (char *format, ...);

void die_with_message (char *format, ...);

#endif /* UTIL_H */
