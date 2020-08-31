#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <cairo.h>

#include "shape.h"



/* DRAWABLE */

struct drawable_t;

/* polymorphic method types */
typedef void drawable_draw_method_t (struct drawable_t *, cairo_t *, double time);
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
void draw_drawable (drawable_t *drawable, cairo_t *cairo, double time);

/* free a drawable instance */
void destroy_drawable (drawable_t *drawable);

/* instantiate a drawable */
drawable_t *create_drawable (void *data,
                             drawable_draw_method_t *draw,
                             drawable_destructor_t *destroy);



/* DRAWABLE SHAPE */

/* a drawable shape is a drawable that draws a given shape */
typedef struct drawable_shape_t {

    /* the shape to be drawn */
    shape_t *shape;

    /* the color to draw the shape */
    color_t color;

    /* TODO: texture? */

} drawable_shape_t;

/* draw a drawable shape */
void draw_drawable_shape (drawable_t *drawable, cairo_t *cairo, double time);

/* free a drawable shape instance */
void destroy_drawable_shape (drawable_t *drawable);

/* instantiate a drawable shape */
drawable_t *create_drawable_shape (shape_t *shape, color_t color);



/* DRAWABLE GROUP */

/* size of preallocated arrays */
#define DRAWABLE_GROUP_CAPACITY 256

/* a drawable group is a container for multiple drawables */
typedef struct drawable_group_t {

    /* number of drawables in the group */
    size_t n_drawables;

    /* the drawables in the group */
    drawable_t **drawables;

} drawable_group_t;

/* add an instance onto a drawable group */
void group_add_drawable (drawable_t *group, drawable_t *drawable);

/* remove an instance from a drawable group */
void group_remove_drawable (drawable_t *group, drawable_t *drawable);

/* remove all the instances from a drawable group */
void group_remove_all_drawables (drawable_t *group);

/* draw a drawable group */
void draw_drawable_group (drawable_t *drawable, cairo_t *cairo, double time);

/* free a drawable group instance */
void destroy_drawable_group (drawable_t *drawable);

/* instantiate a drawable group */
drawable_t *create_drawable_group ();



/* DRAWABLE SVG */

/* size of preallocated arrays */
#define DRAWABLE_SVG_CAPACITY 256

/* an svg drawable loads an svg and automatically creates shapes and drawables
 * for each element in the svg */
typedef struct drawable_svg_t {

    /* collection of created shapes from loading the svg file */
    int n_shapes;
    shape_t **shapes;

    /* the top level group drawable containing all the drawables */
    drawable_t *root;

} drawable_svg_t;

/* draw a drawable svg */
void draw_drawable_svg (drawable_t *drawable, cairo_t *cairo, double time);

/* free a drawable svg instance */
void destroy_drawable_svg (drawable_t *drawable);

/* instantiate a drawable svg */
drawable_t *create_drawable_svg (char *svg_path);



/* DRAWABLE TEXT */

/* a drawable text item that displays a given string */
typedef struct drawable_text_t {

    /* the string to draw */
    char *string;

    /* the color to draw the string */
    color_t color;

    /* the surface containing the bitmap font to draw the string with */
    cairo_surface_t *font;

} drawable_text_t;

/* draw a drawable text */
void draw_drawable_text (drawable_t *drawable, cairo_t *cairo, double time);

/* free a drawable text instance */
void destroy_drawable_text (drawable_t *drawable);

/* instantiate a drawable text */
drawable_t *create_drawable_text (char *string, color_t color, cairo_surface_t *font);

#endif /* DRAWABLE_H */
