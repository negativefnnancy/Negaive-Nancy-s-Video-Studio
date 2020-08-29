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
    color_t color = drawable_shape->color;

    /* trace the shape */
    trace_shape (drawable_shape->shape, cairo);

    /* choose the desired color */
	cairo_set_source_rgb (cairo, color.r, color.g, color.b);

    /* paint the traced shape with the chosen color */
    /* TODO: stroke too? */
	cairo_fill (cairo);
}

void destroy_drawable_shape (drawable_t *drawable) {

    free (drawable->data);
}

drawable_t *create_drawable_shape (shape_t *shape, color_t color) {

    drawable_shape_t *drawable_shape =
        (drawable_shape_t *) calloc (1, sizeof (drawable_shape_t));
    drawable_shape->shape = shape;
    drawable_shape->color = color;

    return create_drawable (drawable_shape,
                            draw_drawable_shape,
                            destroy_drawable_shape);
}

void group_add_drawable (drawable_t *group, drawable_t *drawable) {

    drawable_group_t *drawable_group = (drawable_group_t *) group->data;
    drawable_group->drawables[drawable_group->n_drawables++] = drawable;
}

void group_remove_drawable (drawable_t *group, drawable_t *drawable) {

    drawable_group_t *drawable_group = (drawable_group_t *) group->data;
    remove_array_item ((void **) drawable_group->drawables,
                       &(drawable_group->n_drawables),
                       drawable);
}

void group_remove_all_drawables (drawable_t *group) {

    drawable_group_t *drawable_group = (drawable_group_t *) group->data;

    /* simply set the size of the array to 0 */
    drawable_group->n_drawables = 0;
}

void draw_drawable_group (drawable_t *drawable, cairo_t *cairo) {

    int i;
    drawable_group_t *drawable_group = (drawable_group_t *) drawable->data;

    /* draw all the drawables */
    for (i = 0; i < drawable_group->n_drawables; i++)
        draw_drawable (drawable_group->drawables[i], cairo);
}

void destroy_drawable_group (drawable_t *drawable) {

    drawable_group_t *drawable_group = (drawable_group_t *) drawable->data;

    free (drawable_group->drawables);
    free (drawable_group);
}

drawable_t *create_drawable_group () {

    drawable_group_t *drawable_group =
        (drawable_group_t *) calloc (1, sizeof (drawable_group_t));
    drawable_group->n_drawables = 0;
    drawable_group->drawables =
        (drawable_t **) calloc (DRAWABLE_GROUP_CAPACITY, sizeof (drawable_t *));

    return create_drawable (drawable_group,
                            draw_drawable_group,
                            destroy_drawable_group);
}
