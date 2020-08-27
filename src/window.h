#ifndef WINDOW_H
#define WINDOW_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* window event types */
typedef enum window_event_type_t {

    WINDOW_OPEN,
    WINDOW_CLOSE,
    WINDOW_RESIZE,
    WINDOW_VSYNC

} window_event_type_t;

/* window events */
typedef struct window_event_t {

    struct window_t *window;
    window_event_type_t type;
    union {

        struct {

            int width;
            int height;

        } resize;
    } data;

} window_event_t;

/* configuration for an open window */
typedef struct window_t {

    /* function to handle window events */
    void (*event_handler) (window_event_t event);

    /* whether to send vsync events on vsync */
    bool vsync;

} window_t;

/* instantiate window */
window_t *create_window ();

/* destroy instantiated window */
void destroy_window (window_t *window);

/* redraw the contents of the window */
void update_window (window_t *window);

/* tell the window to close */
void close_window (window_t *window);

/* enter event loop */
void run_window (window_t *window);

#endif /* WINDOW_H */
