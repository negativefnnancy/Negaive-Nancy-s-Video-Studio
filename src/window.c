#include "window.h"

/* instantiate window */
window_t *create_window () {

    window_t *window = (window_t *) calloc (1, sizeof (window_t));
    return window;
}

/* destroy instantiated window */
void destroy_window (window_t *window) {

    free (window);
}

/* redraw the contents of the window */
void update_window (window_t *window) {

}

/* tell the window to close */
void close_window (window_t *window) {

}

/* enter event loop */
void run_window (window_t *window) {

}
