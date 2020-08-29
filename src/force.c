#include "stage.h"
#include "force.h"

void apply_force (force_t *force, struct stage_t *stage) {

    /* polymorphic dispatch */
    force->apply (force, stage);
}

void destroy_force (force_t *force) {

    /* polymorphic dispatch */
    force->destroy (force);

    free (force);
}

force_t *create_force (void *data,
                       force_apply_method_t *apply,
                       force_destructor_t *destroy) {

    force_t *force = (force_t *) calloc (1, sizeof (force_t));
    force->data = data;
    force->apply = apply;
    force->destroy = destroy;
    return force;
}

void destroy_interaction_force (force_t *force) {

    interaction_force_t *interaction_force = (interaction_force_t *) force->data;

    /* polymorphic dispatch */
    interaction_force->destroy (interaction_force);

    free (interaction_force);
}

force_t *create_interaction_force (body_t *a,
                                   body_t* b,
                                   void *data,
                                   force_apply_method_t *apply,
                                   interaction_force_destructor_t *destroy) {

    interaction_force_t *interaction_force =
        (interaction_force_t *) calloc (1, sizeof (interaction_force_t));
    interaction_force->a = a;
    interaction_force->b = b;
    interaction_force->data = data;
    interaction_force->destroy = destroy;

    return create_force (interaction_force, apply, destroy_interaction_force);
}

void spring_force_apply (force_t *force, struct stage_t *stage) {

    double current_length;
    double length_delta;
    double restoring_magnitude;
    double damping_magnitude;
    vec2_t direction;
    vec2_t relative_velocity;
    double relative_speed;

    /* component and net forces acting via the spring on its endpoints */
    vec2_t restoring_force_a, restoring_force_b;
    vec2_t damping_force_a, damping_force_b;
    vec2_t net_force_a, net_force_b;

    /* positions ad velocities of the end points of the spring */
    vec2_t position_a, position_b;
    vec2_t velocity_a, velocity_b;

    interaction_force_t *interaction_force = (interaction_force_t *) force->data;
    spring_force_t *spring_force = (spring_force_t *) interaction_force->data;

    /* if the body targets are NULL, then its an absolute world position
     * otherwise, its the transformed position relative to the center of the body */
    if (interaction_force->a) {

        position_a = get_body_transformed_position (interaction_force->a,
                                                    spring_force->position_a);
        velocity_a = get_body_transformed_velocity (interaction_force->a,
                                                    spring_force->position_a);
    } else {

        position_a = spring_force->position_a;
        velocity_a = make_vec2 (0, 0);
    }

    if (interaction_force->b) {

        position_b = get_body_transformed_position (interaction_force->b,
                                                    spring_force->position_b);
        velocity_b = get_body_transformed_velocity (interaction_force->b,
                                                    spring_force->position_b);
    } else {

        position_b = spring_force->position_b;
        velocity_b = make_vec2 (0, 0);
    }

    /* get the current distance between two end points
     * and the difference between that and the equilibrium distance */
    current_length = distance_vec2 (position_a, position_b);
    if (current_length == 0) /* can't divide by zero now */
        return;
    length_delta   = spring_force->length - current_length;

    /* calculate spring restoring force */
    restoring_magnitude = length_delta * spring_force->stiffness;
    direction = direction_vec2 (position_a, position_b);
    restoring_force_a = multiply_vec2_scalar (direction, -restoring_magnitude);
    restoring_force_b = multiply_vec2_scalar (direction,  restoring_magnitude);

    /* get the current relative velocity between the two endpoints
     * and the component of that along the direction of the spring */
    relative_velocity = subtract_vec2 (velocity_a, velocity_b);
    relative_speed    = dot_vec2 (relative_velocity, direction);

    /* calculate the damping forces */
    damping_magnitude = relative_speed * spring_force->damping;
    damping_force_a = multiply_vec2_scalar (direction, -damping_magnitude);
    damping_force_b = multiply_vec2_scalar (direction,  damping_magnitude);

    /* calculate net force of restoring and damping forces */
    net_force_a = add_vec2 (restoring_force_a, damping_force_a);
    net_force_b = add_vec2 (restoring_force_b, damping_force_b);

    /* apply the force */
    if (interaction_force->a)
        apply_body_force (interaction_force->a,
                          net_force_a,
                          position_a);

    if (interaction_force->b)
        apply_body_force (interaction_force->b,
                          net_force_b,
                          position_b);

}

void destroy_spring_force (interaction_force_t *force) {

    free (force->data);
}

force_t *create_spring_force (body_t *a,
                              body_t *b,
                              vec2_t position_a, 
                              vec2_t position_b,
                              double stiffness,
                              double damping,
                              double length) {

    spring_force_t *spring_force =
        (spring_force_t *) calloc (1, sizeof (spring_force_t));
    spring_force->position_a = position_a;
    spring_force->position_b = position_b;
    spring_force->stiffness  = stiffness;
    spring_force->damping    = damping;
    spring_force->length     = length;

    return create_interaction_force (a, b, spring_force,
                                     spring_force_apply,
                                     destroy_spring_force);
}

void apply_phantom_force (force_t *force, struct stage_t *stage) {

    int i;
    phantom_force_t *phantom_force = (phantom_force_t *) force->data;

    /* iterate all of the targets and apply the phantom force to each */
    for (i = 0; i < stage->n_bodies; i++)
        phantom_force->apply (phantom_force, stage, stage->bodies[i]);
}

void destroy_phantom_force (force_t *force) {

    phantom_force_t *phantom_force = (phantom_force_t *) force->data;

    /* polymorphic dispatch */
    phantom_force->destroy (phantom_force);

    free (phantom_force);
}

force_t *create_phantom_force (void *data,
                               phantom_force_apply_method_t *apply,
                               phantom_force_destructor_t *destroy) {

    phantom_force_t *phantom_force =
        (phantom_force_t *) calloc (1, sizeof (phantom_force_t));
    phantom_force->data = data;
    phantom_force->apply = apply;
    phantom_force->destroy = destroy;

    return create_force (phantom_force, apply_phantom_force, destroy_phantom_force);
}

void gravity_force_apply (phantom_force_t *force, struct stage_t *stage, body_t *body) {

    gravity_force_t *gravity_force = (gravity_force_t *) force->data;

    /* apply the acceleration due to gravity to the target body */
    apply_body_force (body,
                      multiply_vec2_scalar (gravity_force->acceleration, body->mass),
                      body->position);
}

void destroy_gravity_force (phantom_force_t *force) {

    free (force->data);
}

force_t *create_gravity_force (vec2_t acceleration) {

    gravity_force_t *gravity_force =
        (gravity_force_t *) calloc (1, sizeof (gravity_force_t));
    gravity_force->acceleration = acceleration;

    return create_phantom_force (gravity_force,
                                 gravity_force_apply,
                                 destroy_gravity_force);
}

void drag_force_apply (phantom_force_t *force, struct stage_t *stage, body_t *body){

    drag_force_t *drag_force = (drag_force_t *) force->data;

    /* TODO: make this more..... better........ more accurate simulation */

    /* apply the translational component */
    apply_body_force (body,
                      multiply_vec2_scalar (body->velocity,
                                            -drag_force->translational_drag),
                      body->position);

    /* apply the angular component */
    body->angular_acceleration -= body->angular_velocity
                                * drag_force->angular_drag
                                / body->moment_of_inertia;
}

void destroy_drag_force (phantom_force_t *force) {

    free (force->data);
}

force_t *create_drag_force (double translational_drag, double angular_drag) {

    drag_force_t *drag_force =
        (drag_force_t *) calloc (1, sizeof (drag_force_t));
    drag_force->translational_drag = translational_drag;
    drag_force->angular_drag       = angular_drag;

    return create_phantom_force (drag_force,
                                 drag_force_apply,
                                 destroy_drag_force);
}
