#include "drawable.h"

void draw_drawable (drawable_t *drawable, cairo_t *cairo) {

    /* transform by the transformation of each drawables */
    cairo_save (cairo);
    cairo_transform (cairo, &(drawable->transformation));

    /* polymorphic dispatch */
    drawable->draw (drawable, cairo);

    /* restore the transformation matrix */
    cairo_restore (cairo);
}

void destroy_drawable (drawable_t *drawable) {

    /* polymorphic dispatch */
    drawable->destroy (drawable);

    free (drawable);
}

drawable_t *create_drawable (void *data,
                             drawable_draw_method_t *draw,
                             drawable_destructor_t *destroy) {

    drawable_t *drawable = (drawable_t *) calloc (1, sizeof (drawable_t));
    drawable->data       = data;
    drawable->draw       = draw;
    drawable->destroy    = destroy;
    cairo_matrix_init_identity (&(drawable->transformation));
    return drawable;
}

void draw_drawable_shape (drawable_t *drawable, cairo_t *cairo) {

    drawable_shape_t *drawable_shape = (drawable_shape_t *) drawable->data;

    /* trace the shape */
    trace_shape (drawable_shape->shape, cairo);

    /* choose the desired color */
    /* TODO: parameterize color */
	cairo_set_source_rgb (cairo, 1, 1, 1);

    /* paint the traced shape with the chosen color */
    /* TODO: stroke too? */
	cairo_fill (cairo);
}

void destroy_drawable_shape (drawable_t *drawable) {

    free (drawable->data);
}

drawable_t *create_drawable_shape (shape_t *shape) {

    drawable_shape_t *drawable_shape =
        (drawable_shape_t *) calloc (1, sizeof (drawable_shape_t));
    drawable_shape->shape = shape;

    return create_drawable (drawable_shape,
                            draw_drawable_shape,
                            destroy_drawable_shape);
}
