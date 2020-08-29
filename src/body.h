#ifndef BODY_H
#define BODY_H

#include <math.h>

#include <cairo.h>

#include "vector.h"

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

#endif /* BODY_H */
