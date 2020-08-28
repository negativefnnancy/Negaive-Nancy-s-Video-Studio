#ifndef STAGE_H
#define STAGE_H

#include <stdlib.h>

#include "util.h"
#include "matrix.h"

/* a drawable is any object that can be drawn on screen on the stage */

typedef struct drawable_t {

    mat3_t transformation;
    
} drawable_t;

/* a body is a physical entity whose position is integrated over time */

typedef struct body_t {

    mat3_t *position;       /* represents the body's current state */
    mat3_t velocity;        /* first derivative of state */
    mat3_t acceleration;    /* second derivative of state */
    double mass;              /* mass of the whole body */
    double moment_of_inertia; /* moment of inertia of the whole body */

} body_t;

/* integrate the state of the body for the next frame */
void integrate_body (body_t *body);

/* a force causes an exchange in momentum between two bodies */

typedef struct force_t {

    int DUMMY; /* dummy lol */

} force_t;

/* a stage is a state machine representing everything happening on screen
 * at a specific moment */

/* size of preallocated arrays */
#define STAGE_CAPACITY 256

typedef struct stage_t {

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

/* instantiate a stage */
stage_t *create_stage ();

/* free an instantiated stage */
void destroy_stage (stage_t *stage);

#endif /* STAGE_H */
