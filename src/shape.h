#ifndef SHAPE_H
#define SHAPE_H

#include <stdlib.h>

#include <cairo.h>

#include "util.h"
#include "vector.h"



/* SHAPE */

typedef struct shape_t shape_t;

/* polymorphic method types */
typedef void shape_trace_method_t (shape_t *, cairo_t *);
typedef void shape_destructor_t   (shape_t *);

/* a shape is an abstract entity that represents a 2d geometry on screen */
struct shape_t {

    /* polymorphic child struct */
    void *data;

    /* abstract method to trace the geometry of the shape */
    shape_trace_method_t *trace;

    /* TODO: maybe some sdf's or somethin */

    /* abstract destructor */
    shape_destructor_t *destroy;

};

/* trace the shape using cairo */
void trace_shape (shape_t *shape, cairo_t *cairo);

/* free a shape instance */
void destroy_shape (shape_t *shape);

/* instantiate a shape */
shape_t *create_shape (void *data,
                       shape_trace_method_t *trace,
                       shape_destructor_t *destroy);



/* ELLIPSE */

/* an ellipse is a subclass of shape that represents an ovular region on screen */
typedef struct shape_ellipse_t {

    /* width and height of ellipse */
    vec2_t size;

} shape_ellipse_t;

/* trace an ellipse */
void trace_ellipse (shape_t *shape, cairo_t *cairo);

/* free an ellipse instance */
void destroy_ellipse (shape_t *shape);

/* instantiate an ellipse */
shape_t *create_ellipse (vec2_t size);

#endif /* SHAPE_H */
