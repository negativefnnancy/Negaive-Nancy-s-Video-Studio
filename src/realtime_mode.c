#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

#include <SDL2/SDL.h>
#include <cairo.h>

#include "util.h"
#include "stage.h"

/* realtime mode is where the video is displayed immediately in a window */
int entry_realtime (char *script_path) {

    /* nnvs stuff */
    stage_t *stage;

    /* sdl stuff */
    SDL_Window *window;
    SDL_Surface *sdl_surface;
    SDL_Event event;

    /* cairo stuff */
    cairo_surface_t *cairo_surface;
    cairo_t *cairo;

    puts ("Ready for some realtime action!");

    /* setup the stage */
    stage = create_stage ();

    /* initialize sdl and create a window and surface */

    if (SDL_Init (SDL_INIT_VIDEO) == -1)
        die_with_message ("Failed to initialize SDL: %s\n", SDL_GetError ());

    if (!(window = SDL_CreateWindow ("Negative Nancy's Video Studio",
                                     SDL_WINDOWPOS_UNDEFINED,
                                     SDL_WINDOWPOS_UNDEFINED,
                                     /* TODO: paramaterize dimensions */
                                     300, 200,
                                     SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE)))
        die_with_message ("Failed to create window: %s\n", SDL_GetError ());

    /* main event loop */
    for (;;) {

        while (SDL_PollEvent (&event) != 0)
            switch (event.type) {

                case SDL_QUIT:
                    goto quit;

            }

        /* draw a new frame */

        sdl_surface = SDL_GetWindowSurface (window);
        SDL_LockSurface (sdl_surface);

        /* TODO: make sure the pixel format is same across sdl and cairo */
        cairo_surface = cairo_image_surface_create_for_data (sdl_surface->pixels,
                                                             CAIRO_FORMAT_RGB24,
                                                             sdl_surface->w,
                                                             sdl_surface->h,
                                                             sdl_surface->pitch);
        cairo = cairo_create (cairo_surface);

        draw_stage (stage, cairo);

        cairo_destroy (cairo);
        cairo_surface_destroy (cairo_surface);

        SDL_UnlockSurface (sdl_surface);

        /* TODO: do double buffering? */
        SDL_UpdateWindowSurface (window);

        /* prepare the stage for the next frame */
        advance_stage (stage);

        /* TODO: some sort of proper vsync mechanism ?? */
        SDL_Delay (1000 / 60);
    }
quit:

    /* cleanup */
    SDL_DestroyWindow (window);
    SDL_Quit ();
    destroy_stage (stage);

    return EXIT_SUCCESS;
}
