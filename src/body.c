#include "body.h"

void integrate_body (body_t *body, double delta_time) {

    /* TODO: do better than this forward euler integration (maybe) */

    /* assuming acceleration has already been calculated elsewhere */

    vec2_t scaled_velocity;
    vec2_t scaled_acceleration;

    /* scale the derivatives by the delta time first */
    scaled_velocity     = multiply_vec2_scalar (body->velocity,     delta_time);
    scaled_acceleration = multiply_vec2_scalar (body->acceleration, delta_time);

    /* integrate translational state */
    body->position = add_vec2 (body->position, scaled_velocity);
    body->velocity = add_vec2 (body->velocity, scaled_acceleration);

    /* integrate rotational state */
    body->angle            += body->angular_velocity     * delta_time;
    body->angular_velocity += body->angular_acceleration * delta_time;

    /* update the transformation from the new state */
    cairo_matrix_init_translate (body->transformation,
                                 body->position.x,
                                 body->position.y);
    cairo_matrix_rotate (body->transformation, body->angle);
}

void apply_body_translational_force (body_t *body, vec2_t force) {

    vec2_t acceleration = multiply_vec2_scalar (force, 1. / body->mass);
    body->acceleration = add_vec2 (body->acceleration, acceleration);
}

void apply_body_torque (body_t *body, double torque) {

    body->angular_acceleration += torque / body->moment_of_inertia;
}

void apply_body_force (body_t *body, vec2_t force, vec2_t position) {

    vec2_t delta_position;
    double torque;

    /* apply the translational component */
    apply_body_translational_force (body, force);

    /* calculate the implied torque */
    delta_position = subtract_vec2 (position, body->position);
    torque = cross_vec2 (delta_position, force);

    /* apply the implied torque */
    apply_body_torque (body, torque);
}

vec2_t get_body_transformed_position (body_t *body, vec2_t position) {

    vec2_t rotated;

    /* get the position in polar coordinates */
    double length = length_vec2 (position);
    double angle  = angle_vec2  (position);

    /* transform the angle relative to the body */
    angle += body->angle;

    /* convert back to cartesian coordinates */
    rotated = make_vec2 (cos (angle) * length,
                         sin (angle) * length);

    /* then translate it */
    return add_vec2 (rotated, body->position);
}

vec2_t get_body_transformed_velocity (body_t *body, vec2_t position) {

    /* distance of point from center of gravity */
    double length = length_vec2 (position);

    /* translational velocity of a point on the body due to angular velocity */
    vec2_t linear_velocity =
        make_vec2 (-sin (body->angle) * length * body->angular_velocity,
                    cos (body->angle) * length * body->angular_velocity);

    /* add translational velocity */
    return add_vec2 (linear_velocity, body->velocity);
}
