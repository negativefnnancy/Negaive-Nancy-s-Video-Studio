#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

#include <SDL2/SDL.h>

#include "util.h"

/* realtime mode is where the video is displayed immediately in a window */
int entry_realtime (char *script_path) {

    SDL_Window *window;
    SDL_Surface *surface;
    uint8_t *pixels;
    int i;

    puts ("Ready for some realtime action!");

    /* initialize sdl and create a window */

    if (SDL_Init (SDL_INIT_VIDEO) == -1)
        die_with_message ("Failed to initialize SDL: %s\n", SDL_GetError ());

    if (!(window = SDL_CreateWindow ("Nancy Says",
                                     SDL_WINDOWPOS_UNDEFINED,
                                     SDL_WINDOWPOS_UNDEFINED,
                                     /* TODO: paramaterize dimensions */
                                     300, 200,
                                     SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE)))
        die_with_message ("Failed to create window: %s\n", SDL_GetError ());

    surface = SDL_GetWindowSurface (window);
    pixels = surface->pixels;
    for (i = 0; i < surface->w * surface->h * surface->format->BytesPerPixel; i++)
        pixels[i] = i % 256;

    SDL_UpdateWindowSurface (window);

    scanf ("huh");

    /* cleanup */

    SDL_DestroyWindow (window);
    SDL_Quit ();

    return EXIT_SUCCESS;
}
