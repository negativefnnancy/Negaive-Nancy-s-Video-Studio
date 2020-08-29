#include "shape.h"

void trace_shape (shape_t *shape, cairo_t *cairo) {

    /* polymorphic dispatch */
    shape->trace (shape, cairo);
}

void destroy_shape (shape_t *shape) {

    /* polymorphic dispatch */
    shape->destroy (shape);

    free (shape);
}

shape_t *create_shape (void *data,
                       shape_trace_method_t *trace,
                       shape_destructor_t *destroy) {

    shape_t *shape = (shape_t *) calloc (1, sizeof (shape_t));
    shape->data    = data;
    shape->trace   = trace;
    shape->destroy = destroy;
    return shape;
}

void trace_ellipse (shape_t *shape, cairo_t *cairo) {

    shape_ellipse_t *ellipse = (shape_ellipse_t *) shape->data;

    cairo_save     (cairo);
    cairo_scale    (cairo, ellipse->size.x, ellipse->size.y);
    cairo_new_path (cairo);
    cairo_arc      (cairo, 0, 0, 1, 0, 2 * M_PI);
    cairo_restore  (cairo);
}

void destroy_ellipse (shape_t *shape) {

    free (shape->data);
}

shape_t *create_ellipse (vec2_t size) {

    shape_ellipse_t *ellipse =
        (shape_ellipse_t *) calloc (1, sizeof (shape_ellipse_t));
    ellipse->size = size;

    return create_shape (ellipse, trace_ellipse, destroy_ellipse);
}

void trace_rectangle (shape_t *shape, cairo_t *cairo) {

    shape_rectangle_t *rectangle = (shape_rectangle_t *) shape->data;

    cairo_new_path  (cairo);
    cairo_rectangle (cairo, -rectangle->size.x / 2,
                            -rectangle->size.y / 2,
                             rectangle->size.x,
                             rectangle->size.y);
}

void destroy_rectangle (shape_t *shape) {

    free (shape->data);
}

shape_t *create_rectangle (vec2_t size) {

    shape_rectangle_t *rectangle =
        (shape_rectangle_t *) calloc (1, sizeof (shape_rectangle_t));
    rectangle->size = size;

    return create_shape (rectangle, trace_rectangle, destroy_rectangle);
}
