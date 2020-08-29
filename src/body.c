#include "body.h"

original_force_t make_original_force (vec2_t origin, vec2_t force) {

    original_force_t original_force;
    original_force.origin = origin;
    original_force.force = force;
    return original_force;
}

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

    /* add the force to the accumulated forces array */
    body->forces[body->n_forces++] = make_original_force (position, force);
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

void clear_forces (body_t *body) {

    body->acceleration = make_vec2 (0, 0);
    body->angular_acceleration = 0;
    body->n_forces = 0;
}

void draw_force (original_force_t force, cairo_t *cairo) {

    vec2_t tail           = force.origin;
    vec2_t scaled         = multiply_vec2_scalar (force.force, ARROW_SECOND_LENGTH);
    vec2_t head           = add_vec2 (tail, scaled);
    vec2_t direction      = direction_vec2 (head, tail);
    double arrow_length   = length_vec2 (scaled);
    double head_length    = arrow_length * ARROW_HEAD_PROPORTION;
    vec2_t back_head      = multiply_vec2_scalar (direction, head_length);
    vec2_t head_start     = add_vec2 (head, back_head);
    double back_length    = tan (ARROW_HEAD_ANGLE);
    vec2_t back_direction = rotate_vec2 (back_head);
    vec2_t back_offset    = multiply_vec2_scalar (back_direction, back_length);
    vec2_t corner_a       = add_vec2 (head_start, back_offset);
    vec2_t corner_b       = subtract_vec2 (head_start, back_offset);

    /* TODO: parameterize color */
    cairo_set_source_rgb (cairo, 1, 0, 0);
    cairo_set_line_width (cairo, ARROW_LINE_WIDTH);

    cairo_new_path (cairo);
    cairo_move_to  (cairo, tail.x,       tail.y);
    cairo_line_to  (cairo, head_start.x, head_start.y);
    cairo_stroke   (cairo);

    cairo_new_path (cairo);
    cairo_move_to  (cairo, head.x, head.y);
    cairo_line_to  (cairo, corner_a.x, corner_a.y);
    cairo_line_to  (cairo, corner_b.x, corner_b.y);
    cairo_line_to  (cairo, head.x,     head.y);
    cairo_fill     (cairo);
    cairo_stroke   (cairo);
}
