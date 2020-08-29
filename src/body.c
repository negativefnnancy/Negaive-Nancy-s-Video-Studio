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
