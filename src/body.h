#ifndef BODY_H
#define BODY_H

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

#endif /* BODY_H */
