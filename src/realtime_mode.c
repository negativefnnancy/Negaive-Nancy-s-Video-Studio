#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "debug.h"

/* realtime mode is where the video is displayed immediately in a window */
int entry_realtime (char *script_path) {

    Display *display;
    int screen;
    Window window;
    int black, white;

    puts ("Ready for some realtime action!");

    /* connect to X server */

    assert ((display = XOpenDisplay (NULL)));
    screen = DefaultScreen (display);
    black = BlackPixel (display, screen);
    white = WhitePixel (display, screen);

    window = XCreateSimpleWindow (display,
                                  DefaultRootWindow (display),
                                  0, 0, 300, 200, 0, black, white);

    XSetStandardProperties (display, window, "Nancy Says", NULL, None,
                            NULL, 0, NULL);

    XSelectInput (display, window, ExposureMask |
                                   ButtonPressMask |
                                   KeyPressMask);
    
    XClearWindow (display, window);
    XMapRaised (display, window);
    XFlush (display);

    scanf ("yo");
    /* cleanup */

    XDestroyWindow (display, window);
    XCloseDisplay (display);

    return EXIT_SUCCESS;
}
