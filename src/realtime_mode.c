#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <cairo.h>

#include "util.h"
#include "font.h"
#include "stage.h"

#define WIDTH 640
#define HEIGHT 480

/* realtime mode is where the video is displayed immediately in a window */
int entry_realtime (char *script_path) {

    /* timer */
    clock_t start_time;
    double time;

    /* nnvs stuff */
    force_t *gravity;
    force_t *drag;
    force_t *spring_1;
    force_t *spring_2;
    force_t *mouse_force;
    interaction_force_t *mouse_interaction_force;
    spring_force_t *mouse_spring_force;
    stage_t *stage;
    shape_t *shape_1;
    shape_t *shape_2;
    shape_t *shape_3;
    drawable_t *drawable_1;
    drawable_t *drawable_2;
    drawable_t *drawable_3;
    drawable_t *drawable_4;
    drawable_t *drawable_5;
    drawable_t *drawable_6;
    body_t body_1;
    body_t body_2;
    font_t *font;

    /* sdl stuff */
    SDL_Window *window;
    SDL_Surface *sdl_surface;
    SDL_Event event;

    /* cairo stuff */
    cairo_surface_t *cairo_surface;
    cairo_t *cairo;

    /* mouse state */
    vec2_t mouse_position = make_vec2 (0, 0);
    bool mouse_down = false;

    puts ("Ready for some realtime action!");

    /* load the font */
    font = create_font ("res/font.png", 128, 128, 80, 3, 20);

    /* setup the stage */
    gravity  = create_gravity_force (make_vec2 (0, 2));
    drag     = create_drag_force    (0.25, 0.25);
    spring_1 = create_spring_force    (NULL,                  &body_1,
                                       make_vec2 (-.25, -1), make_vec2 (-.25, 0),
                                       8, 0.75, 0);
    spring_2 = create_spring_force    (&body_1,               &body_2,
                                       make_vec2 (.25, -0),    make_vec2 (0, -.25),
                                       8, 0.75, 0);
    mouse_force = create_spring_force (NULL,                   NULL,
                                       make_vec2 (0, 0),    make_vec2 (0, 0),
                                       8, 0.5, 0);

    /* this is scary 0-0 */
    mouse_interaction_force = (interaction_force_t *) mouse_force->data;
    mouse_spring_force = (spring_force_t *) mouse_interaction_force->data;

    shape_1 = create_rectangle (make_vec2 (1, 1));    /* square */
    shape_2 = create_rectangle (make_vec2 (.25, .5)); /* tall rect */
    shape_3 = create_polygon   ();                    /* star */
    add_vertex (shape_3, make_vec2 (-1,    0));
    add_vertex (shape_3, make_vec2 (-0.3, -0.3));
    add_vertex (shape_3, make_vec2 ( 0,   -1));
    add_vertex (shape_3, make_vec2 ( 0.3, -0.3));
    add_vertex (shape_3, make_vec2 ( 1,    0));
    add_vertex (shape_3, make_vec2 ( 0.3,  0.3));
    add_vertex (shape_3, make_vec2 ( 0,    1));
    add_vertex (shape_3, make_vec2 (-0.3,  0.3));

    drawable_1 = create_drawable_shape (shape_3, make_color (0, 1, 1, 1));
    drawable_2 = create_drawable_shape (shape_3, make_color (0, 0, 1, 1));
    drawable_3 = create_drawable_group ();
    /*drawable_4 = create_drawable_shape (shape_2, make_color (1, 1, 0, 1));*/
    drawable_4 = create_drawable_svg ("res/test.svg");
    drawable_5 = create_drawable_text ("It's working !! :3", make_color (0.5, 0.875, 1, 1), font);
    drawable_6 = create_drawable_text ("It's working !! :3", make_color (0.1, 0.2, 0.3, 0.5), font);
    cairo_matrix_translate (&(drawable_5->transformation), -1, -0.5);
    cairo_matrix_translate (&(drawable_6->transformation), -1.025, -0.475);
    cairo_matrix_scale (&(drawable_5->transformation), 0.2, 0.2);
    cairo_matrix_scale (&(drawable_6->transformation), 0.2, 0.2);
    group_add_drawable (drawable_3, drawable_1);
    group_add_drawable (drawable_3, drawable_2);
    cairo_matrix_scale (&(drawable_1->transformation), .25, .25);
    cairo_matrix_scale (&(drawable_2->transformation), .15, .15);

    stage = create_stage (make_color (.2, .2, .2, 1));
    add_drawable (stage, drawable_3);
    add_drawable (stage, drawable_4);
    add_drawable (stage, drawable_6);
    add_drawable (stage, drawable_5);
    add_body     (stage, &body_1);
    add_body     (stage, &body_2);
    add_force    (stage, gravity);
    add_force    (stage, drag);
    add_force    (stage, spring_1);
    add_force    (stage, spring_2);

    body_1.transformation = &(drawable_3->transformation);
    body_1.position     = make_vec2 (0, 0.05);
    body_1.velocity     = make_vec2 (0, 0);
    body_1.acceleration = make_vec2 (0, 0);
    body_1.angle                = 0;
    body_1.angular_velocity     = 0;
    body_1.angular_acceleration = 0;
    body_1.mass              = 1;
    body_1.moment_of_inertia = 1;

    body_2.transformation = &(drawable_4->transformation);
    body_2.position     = make_vec2 (.25, .25);
    body_2.velocity     = make_vec2 (0, 0);
    body_2.acceleration = make_vec2 (0, 0);
    body_2.angle                = 0;
    body_2.angular_velocity     = 0;
    body_2.angular_acceleration = 0;
    body_2.mass              = 1;
    body_2.moment_of_inertia = 0.125;

    /* initialize sdl and create a window and surface */
    if (SDL_Init (SDL_INIT_VIDEO) == -1)
        die_with_message ("Failed to initialize SDL: %s\n", SDL_GetError ());

    if (!(window = SDL_CreateWindow ("Negative Nancy's Video Studio",
                                     SDL_WINDOWPOS_UNDEFINED,
                                     SDL_WINDOWPOS_UNDEFINED,
                                     WIDTH, HEIGHT,
                                     SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE)))
        die_with_message ("Failed to create window: %s\n", SDL_GetError ());

    /* make note of the start time */
    start_time = clock ();

    /* main event loop */
    for (;;) {

        while (SDL_PollEvent (&event) != 0)
            switch (event.type) {

                case SDL_QUIT:
                    goto quit;

                case SDL_MOUSEMOTION:
                    /* record the mouse position
                     * and transform it to normalized device units */
                    mouse_position = make_vec2 (event.motion.x, event.motion.y);
                    mouse_position = multiply_vec2_scalar (mouse_position,
                                                           2.0 / HEIGHT);
                    mouse_position = subtract_vec2 (mouse_position,
                                                    make_vec2 ((double) WIDTH / HEIGHT,
                                                               1));

                    debug_printf ("mouse position: %f, %f\n", mouse_position.x,
                                                              mouse_position.y);

                    /* the location of the mouse in the mouse force */
                    mouse_spring_force->position_a = mouse_position;

                    break;

                case SDL_MOUSEBUTTONDOWN:
                    /* if a physical body was clicked on, create a spring force
                     * that attaches the body at that point to the mouse */
                    mouse_down = true;
                    add_force (stage, mouse_force);

                    /* the first body of the spring will be the mouse position
                     * and the second body will be the target body */
                    mouse_interaction_force->b = stage->bodies[0]; /* TODO for real! */
                    
                    /* the relative location on the target body */
                    mouse_spring_force->position_b = make_vec2 (0, 0); /* TODO: !!! */

                    break;

                case SDL_MOUSEBUTTONUP:
                    /* remove any spring force due to the mouse */
                    mouse_down = false;
                    remove_force (stage, mouse_force);
                    break;
            }

        /* get time of current frame */
        /* TODO: get this in actual time 0-0 */
        time = (double) (clock () - start_time) / CLOCKS_PER_SEC;

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

        draw_stage (stage, cairo, time);

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
    destroy_drawable (drawable_4);
    destroy_drawable (drawable_5);
    destroy_drawable (drawable_6);
    destroy_shape    (shape_1);
    destroy_shape    (shape_2);
    destroy_force    (gravity);
    destroy_force    (drag);
    destroy_force    (spring_1);
    destroy_force    (spring_2);
    destroy_force    (mouse_force);
    destroy_font     (font);

    return EXIT_SUCCESS;
}
