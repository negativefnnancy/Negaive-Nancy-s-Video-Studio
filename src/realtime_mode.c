#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "debug.h"
#include "window.h"

/* window event callback */
void callback (window_event_t event) {

    switch (event.type) {

        case WINDOW_OPEN:
            debug_printf ("[EVENT] open!");
            update_window (event.window);
            break;

        case WINDOW_CLOSE:
            debug_printf ("[EVENT] close!");
            break;

        case WINDOW_RESIZE:
            debug_printf ("[EVENT] resize! (%d, %d)", event.data.resize.width,
                                                      event.data.resize.height);
            break;

        case WINDOW_VSYNC:
            debug_printf ("[EVENT] vsync!");
            update_window (event.window);
            break;
    }
}

/* realtime mode is where the video is displayed immediately in a window */
int entry_realtime (char *script_path) {

    window_t *window;

    puts ("Ready for some realtime action!");

    /* create a window */
    debug_printf ("creating a window!\n");
    window = create_window ();
    window->event_handler = callback;
    window->vsync = true;

    /* enter window's event loop */
    debug_printf ("entering window's run loop!\n");
    run_window (window);

    /* cleanup */
    debug_printf ("destroying window!\n");
    destroy_window (window);

    return EXIT_SUCCESS;
}
