#include "force.h"

void apply_force (force_t *force) {

    /* polymorphic dispatch */
    force->apply (force);
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

void apply_phantom_force (force_t *force) {

    phantom_force_t *phantom_force = (phantom_force_t *) force->data;

    /* TODO: for every target body, do:
     *   phantom_force->apply (phantom_force, body);
     */
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
