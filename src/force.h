#ifndef FORCE_H
#define FORCE_H

struct force_t;

/* polymorphic method types */
typedef void force_apply_method_t (struct force_t *);
typedef void force_destructor_t   (struct force_t *);

/* an abstract force that is applied to manipulate the stage */
typedef struct force_t {

    /* polymorphic child struct */
    void *data;

    /* abstract method to apply the force to its targets */
    force_apply_method_t *apply;

    /* abstract destructor */
    force_destructor_t *destroy;

} force_t;

/* apply the force to its targets */
void apply_force (force_t *force);

/* free a force instance */
void destroy_force (force_t *force);

/* instantiate a force */
force_t *create_force (void *data,
                       force_apply_method_t *apply,
                       force_destructor_t *destroy);

#endif /* FORCE_H */
