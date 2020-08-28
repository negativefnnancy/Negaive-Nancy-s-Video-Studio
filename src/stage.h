#ifndef STAGE_H
#define STAGE_H

#include "matrix.h"

/* a drawable is any object that can be drawn on screen on the stage */

typedef struct drawable_t {

    matrix_t transformation;
    
} drawable_t;

/* a body is a physical entity whose position is integrated over time */

typedef struct body_t {

    matrix_t *position;       /* represents the body's current state */
    matrix_t velocity;        /* first derivative of state */
    matrix_t acceleration;    /* second derivative of state */
    double mass;              /* mass of the whole body */
    double moment_of_inertia; /* moment of inertia of the whole body */

} body_t;

/* integrate the state of the body for the next frame */
void integrate_body (body_t *body) {

    *(body->position) = add_matrix (*(body->position), body->velocity);

    /* assuming acceleration has already been calculated */
    body->velocity = add_matrix (*(body->velocity), body->acceleration);
}

/* a force causes an exchange in momentum between two bodies */

typedef struct force_t {

} force_t;

/* a stage is a state machine representing everything happening on screen
 * at a specific moment */

typedef struct stage_t {

    /* null terminated arrays */
    drawable_t **drawables; /* everything on screen */
    body_t     **bodies;    /* entities subject to physical integration */
    force_t    **forces;    /* forces acting on bodies */

} stage_t;

/* add a drawable instance onto a stage */
void add_drawable (stage_t *stage, drawable_t *drawable) {
    
    reallocarray (stage->drawables, 
}

/* remove a drawable instance from a stage */

/* instantiate a stage */
stage_t *create_stage () {

    stage_t *stage   = (stage_t *)     calloc (1, sizeof (stage_t));
    stage->drawables = (drawable_t **) calloc (1, sizeof (drawable_t *));
    stage->bodies    = (body_t **)     calloc (1, sizeof (body_t *));
    stage->forces    = (force_t **)    calloc (1, sizeof (force_t *));
    return stage;
}

/* free an instantiated stage */
void destroy_stage (stage_t *stage) {

    remove_all_drawables (stage);
    remove_all_bodies    (stage);
    remove_all_forces    (stage);
    free (stage);
}

#endif /* STAGE_H */
