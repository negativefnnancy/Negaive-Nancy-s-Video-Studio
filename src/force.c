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
    body->acceleration = add_vec2 (body->acceleration, gravity_force->acceleration);
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
