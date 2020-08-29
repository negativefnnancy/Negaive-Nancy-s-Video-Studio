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
