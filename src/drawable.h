#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <cairo.h>

#include "shape.h"



/* DRAWABLE */

struct drawable_t;

/* polymorphic method types */
typedef void drawable_draw_method_t (struct drawable_t *, cairo_t *);
typedef void drawable_destructor_t  (struct drawable_t *);

/* a drawable is an abstract object that can be drawn on screen on the stage */
typedef struct drawable_t {

    /* transforation that gets applied when drawing */
    cairo_matrix_t transformation;

    /* polymorphic child struct */
    void *data;

    /* abstract method to draw the drawable in its current state */
    drawable_draw_method_t *draw;

    /* abstract destructor */
    drawable_destructor_t *destroy;
    
} drawable_t;

/* draw a drawable in its current state */
void draw_drawable (drawable_t *drawable, cairo_t *cairo);

/* free a drawable instance */
void destroy_drawable (drawable_t *drawable);

/* instantiate a drawable */
drawable_t *create_drawable (void *data,
                             drawable_draw_method_t *draw,
                             drawable_destructor_t *destroy);



/* DRAWABLE_SHAPE */

/* a drawable shape is a drawable that draws a given shape */
typedef struct drawable_shape_t {

    /* the shape to be drawn */
    shape_t *shape;

    /* the color to draw the shape */
    color_t color;

    /* TODO: texture? */

} drawable_shape_t;

/* draw a drawable shape */
void draw_drawable_shape (drawable_t *drawable, cairo_t *cairo);

/* free drawable shape instance */
void destroy_drawable_shape (drawable_t *drawable);

/* instantiate a drawable shape */
drawable_t *create_drawable_shape (shape_t *shape, color_t color);

#endif /* DRAWABLE_H */
