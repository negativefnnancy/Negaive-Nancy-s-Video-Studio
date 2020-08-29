#ifndef FORCE_H
#define FORCE_H

#include "body.h"



/* FORCE */

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



/* INTERACTION FORCE */

struct interaction_force_t;

/* polymorphic method types */
typedef void interaction_force_destructor_t (struct interaction_force_t *);

/* an abstract interaction force that mediates energy exchange between two bodies */
typedef struct interaction_force_t {

    /* the bodies interacting */
    body_t *a;
    body_t *b;

    /* polymorphic child struct */
    void *data;

    /* abstract destructor */
    interaction_force_destructor_t *destroy;

} interaction_force_t;

/* free an interaction force instance */
void destroy_interaction_force (force_t *force);

/* instantiate an interaction force */
force_t *create_interaction_force (body_t *a,
                                   body_t* b,
                                   void *data,
                                   force_apply_method_t *apply,
                                   interaction_force_destructor_t *destroy);



/* PHANTOM FORCE */

struct phantom_force_t;

/* polymorphic method types */
typedef void phantom_force_apply_method_t (struct phantom_force_t *, body_t *body);
typedef void phantom_force_destructor_t   (struct phantom_force_t *);

/* an abstract phantom force that injects energy into its target bodies */
typedef struct phantom_force_t {

    /* polymorphic child struct */
    void *data;

    /* abstract method to apply the phantom force to its targets */
    phantom_force_apply_method_t *apply;

    /* abstract destructor */
    phantom_force_destructor_t *destroy;

} phantom_force_t;

/* apply the phantom force to its targets */
void apply_phantom_force (force_t *force);

/* free a phantom force instance */
void destroy_phantom_force (force_t *force);

/* instantiate a phantom force */
force_t *create_phantom_force (void *data,
                               phantom_force_apply_method_t *apply,
                               phantom_force_destructor_t *destroy);

#endif /* FORCE_H */
