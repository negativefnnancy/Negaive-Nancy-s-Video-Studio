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
    force_t *gravity;
    force_t *spring;
    stage_t *stage;
    shape_t *shape_1;
    shape_t *shape_2;
    shape_t *shape_3;
    drawable_t *drawable_1;
    drawable_t *drawable_2;
    drawable_t *drawable_3;
    body_t body_1;

    /* sdl stuff */
    SDL_Window *window;
    SDL_Surface *sdl_surface;
    SDL_Event event;

    /* cairo stuff */
    cairo_surface_t *cairo_surface;
    cairo_t *cairo;

    puts ("Ready for some realtime action!");

    /* setup the stage */
    gravity = create_gravity_force (make_vec2 (0, 4));
    spring  = create_spring_force  (NULL,                &body_1,
                                    make_vec2 (-.5, 0),  make_vec2 (-.5, 0),
                                    8, 1, 0);
    shape_1 = create_rectangle (make_vec2 (1, 1));   /* square */
    shape_2 = create_rectangle (make_vec2 (0.5, 1)); /* tall rect */
    shape_3 = create_polygon   ();                   /* star */
    add_vertex (shape_3, make_vec2 (-1, 0));
    add_vertex (shape_3, make_vec2 (-.3, -.3));
    add_vertex (shape_3, make_vec2 (0, -1));
    add_vertex (shape_3, make_vec2 (.3, -.3));
    add_vertex (shape_3, make_vec2 (1, 0));
    add_vertex (shape_3, make_vec2 (.3, .3));
    add_vertex (shape_3, make_vec2 (0, 1));
    add_vertex (shape_3, make_vec2 (-.3, .3));

    drawable_1 = create_drawable_shape (shape_3, make_color (0, 1, 1, 1));
    drawable_2 = create_drawable_shape (shape_3, make_color (0, 0, 1, 1));
    drawable_3 = create_drawable_group ();
    group_add_drawable (drawable_3, drawable_1);
    group_add_drawable (drawable_3, drawable_2);
    cairo_matrix_scale (&(drawable_1->transformation), 0.5, 0.5);
    cairo_matrix_scale (&(drawable_2->transformation), 0.3, 0.3);

    stage = create_stage (make_color (0.2, 0.2, 0.2, 1));
    add_drawable (stage, drawable_3);
    add_body     (stage, &body_1);
    add_force    (stage, gravity);
    add_force    (stage, spring);

    body_1.transformation = &(drawable_3->transformation);
    body_1.position     = make_vec2 (0, 0);
    body_1.velocity     = make_vec2 (0, .1);
    body_1.acceleration = make_vec2 (0, 0);
    body_1.angle                = 0;
    body_1.angular_velocity     = 0;
    body_1.angular_acceleration = 0;
    body_1.mass              = 1;
    body_1.moment_of_inertia = 1;

    /* initialize sdl and create a window and surface */
    if (SDL_Init (SDL_INIT_VIDEO) == -1)
        die_with_message ("Failed to initialize SDL: %s\n", SDL_GetError ());

    if (!(window = SDL_CreateWindow ("Negative Nancy's Video Studio",
                                     SDL_WINDOWPOS_UNDEFINED,
                                     SDL_WINDOWPOS_UNDEFINED,
                                     /* TODO: paramaterize dimensions */
                                     640, 480,
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
        /* TODO: proper delta time */
        advance_stage (stage, 1. / 60);

        /* TODO: some sort of proper vsync mechanism ?? */
        SDL_Delay (1000. / 60);
    }
quit:

    /* cleanup */
    SDL_DestroyWindow (window);
    SDL_Quit ();
    destroy_stage    (stage);
    destroy_drawable (drawable_1);
    destroy_drawable (drawable_2);
    destroy_drawable (drawable_3);
    destroy_shape    (shape_1);
    destroy_shape    (shape_2);
    destroy_force    (gravity);
    destroy_force    (spring);

    return EXIT_SUCCESS;
}
