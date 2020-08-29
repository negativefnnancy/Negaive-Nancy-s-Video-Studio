#ifndef STAGE_H
#define STAGE_H

#include <stdlib.h>

#include <cairo.h>

#include "util.h"
#include "vector.h"
#include "drawable.h"



/* BODY */

/* a body is a physical entity whose position is integrated over time */
typedef struct body_t {

    /* attached transformation if any */
    cairo_matrix_t *transformation;

    /* translational state and first and second derivative */
    vec2_t position;
    vec2_t velocity;
    vec2_t acceleration;

    /* rotational state and first and second derivative */
    double angle;
    double angular_velocity;
    double angular_acceleration;

    /* inertial properties */
    double mass;                 /* quantity of translational inertia */
    double moment_of_inertia;    /* quantity of rotational inertia */

} body_t;

/* integrate the state of the body for the next frame */
void integrate_body (body_t *body, double delta_time);



/* FORCE */

/* a force causes an exchange in momentum between two bodies */
typedef struct force_t {

    int DUMMY; /* dummy lol */

} force_t;



/* STAGE */

/* size of preallocated arrays */
#define STAGE_CAPACITY 256

/* a stage is a state machine representing everything happening on screen
 * at a specific moment */
typedef struct stage_t {

    /* color of the background */
    color_t background_color;

    /* number of each item on stage */
    size_t n_drawables;
    size_t n_bodies;
    size_t n_forces;

    drawable_t **drawables; /* everything on screen */
    body_t     **bodies;    /* entities subject to physical integration */
    force_t    **forces;    /* forces acting on bodies */

} stage_t;

/* add an instance onto a stage */
void add_drawable (stage_t *stage, drawable_t *drawable);
void add_body     (stage_t *stage, body_t *body);
void add_force    (stage_t *stage, force_t *force);

/* remove an instance from a stage */
void remove_drawable (stage_t *stage, drawable_t *drawable);
void remove_body     (stage_t *stage, body_t *body);
void remove_forces   (stage_t *stage, force_t *force);

/* remove all the instances from a stage */
void remove_all_drawables (stage_t *stage);
void remove_all_bodies    (stage_t *stage);
void remove_all_forces    (stage_t *stage);

/* draw the current state of a stage */
void draw_stage (stage_t *stage, cairo_t *cairo);

/* advance the state of a stage to the next frame */
void advance_stage (stage_t *stage, double delta_time);

/* TODO: process a single audio frame */

/* instantiate a stage */
stage_t *create_stage (color_t background_color);

/* free an instantiated stage */
void destroy_stage (stage_t *stage);

#endif /* STAGE_H */
