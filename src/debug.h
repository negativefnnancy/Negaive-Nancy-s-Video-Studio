#ifndef DEBUG_H
#define DEBUG_H

#include <stdbool.h>

extern bool DEBUG;

/* debug logging functions */
void _debug_printf (char *format, ...);
void debug_printf (char *format, ...);

#endif /* DEBUG_H */
