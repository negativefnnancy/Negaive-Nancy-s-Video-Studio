#ifndef BODY_H
#define BODY_H

#include <stdlib.h>
#include <math.h>

#include <cairo.h>

#include "util.h"
#include "vector.h"

/* line width of drawn force arrows in normalized deviced units */
#define ARROW_LINE_WIDTH      0.0075
#define ARROW_SECOND_LENGTH   0.125 /* spatial length of a second */
#define ARROW_HEAD_ANGLE      (M_PI / 6)
#define ARROW_HEAD_PROPORTION 0.25 /* arrow head length to arrow length ratio */

/* struct to record a force and its origin */
typedef struct original_force_t {

    vec2_t origin;
    vec2_t force;

} original_force_t;

/* return an initialized original force */
original_force_t make_original_force (vec2_t origin, vec2_t force);

/* max amount of forces to be logged by a body */
#define FORCE_CAPACITY 256

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

    /* a log of all the forces accumulated for a frame */
    size_t n_forces;
    original_force_t forces[FORCE_CAPACITY];

} body_t;

/* integrate the state of the body for the next frame */
void integrate_body (body_t *body, double delta_time);

/* apply only translational force to a body without implied torque */
void apply_body_translational_force (body_t *body, vec2_t force);

/* apply torque to a body */
void apply_body_torque (body_t *body, double torque);

/* apply a force to a body at a given absolute point in space, implying torque */
void apply_body_force (body_t *body, vec2_t force, vec2_t position);

/* get the transformed position on a body relative to the center of gravity */
vec2_t get_body_transformed_position (body_t *body, vec2_t position);

/* get the transformed velocity on a body relative to the center of gravity */
vec2_t get_body_transformed_velocity (body_t *body, vec2_t position);

/* clear the force accumulation including the acceleration accumulator */
void clear_forces (body_t *body);

/* draw a representation of an original force */
void draw_force (original_force_t force, cairo_t *cairo);

#endif /* BODY_H */
