#ifndef STAGE_H
#define STAGE_H

#include <stdlib.h>

#include <cairo.h>

#include "util.h"
#include "vector.h"
#include "drawable.h"
#include "body.h"
#include "force.h"

/* size of preallocated arrays */
#define STAGE_CAPACITY 256

struct force_t;

/* a stage is a state machine representing everything happening on screen
 * at a specific moment */
typedef struct stage_t {

    /* color of the background */
    color_t background_color;

    /* number of each item on stage */
    size_t n_drawables;
    size_t n_bodies;
    size_t n_forces;

    struct drawable_t **drawables; /* everything on screen */
    struct body_t     **bodies;    /* entities subject to physical integration */
    struct force_t    **forces;    /* forces acting on bodies */

} stage_t;

/* add an instance onto a stage */
void add_drawable (stage_t *stage, drawable_t *drawable);
void add_body     (stage_t *stage, body_t *body);
void add_force    (stage_t *stage, struct force_t *force);

/* remove an instance from a stage */
void remove_drawable (stage_t *stage, drawable_t *drawable);
void remove_body     (stage_t *stage, body_t *body);
void remove_forces   (stage_t *stage, struct force_t *force);

/* remove all the instances from a stage */
void remove_all_drawables (stage_t *stage);
void remove_all_bodies    (stage_t *stage);
void remove_all_forces    (stage_t *stage);

/* draw the current state of a stage */
void draw_stage (stage_t *stage, cairo_t *cairo, double time);

/* advance the state of a stage to the next frame */
void advance_stage (stage_t *stage, double delta_time);

/* TODO: process a single audio frame */

/* instantiate a stage */
stage_t *create_stage (color_t background_color);

/* free an instantiated stage */
void destroy_stage (stage_t *stage);

#endif /* STAGE_H */
