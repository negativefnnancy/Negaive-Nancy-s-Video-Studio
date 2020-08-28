#include "stage.h"

void integrate_body (body_t *body) {

    /* TODO: do better than this forward euler integration */

    *(body->position) = add_mat3 (*(body->position), body->velocity);

    /* assuming acceleration has already been calculated */
    body->velocity = add_mat3 (body->velocity, body->acceleration);
}

void add_drawable (stage_t *stage, drawable_t *drawable) {
    
    stage->drawables[stage->n_drawables++] = drawable;
}

void add_body (stage_t *stage, body_t *body) {
    
    stage->bodies[stage->n_bodies++] = body;
}

void add_force (stage_t *stage, force_t *force) {
    
    stage->forces[stage->n_forces++] = force;
}

void remove_drawable (stage_t *stage, drawable_t *drawable) {

    remove_array_item ((void **) stage->drawables, &stage->n_drawables, drawable);
}

void remove_body (stage_t *stage, body_t *body) {

    remove_array_item ((void **) stage->bodies, &stage->n_bodies, body);
}

void remove_forces (stage_t *stage, force_t *force) {

    remove_array_item ((void **) stage->forces, &stage->n_forces, force);
}

void remove_all_drawables (stage_t *stage) {

    /* simply set the size of the array to 0 */
    stage->n_drawables = 0;
}

void remove_all_bodies (stage_t *stage) {

    /* simply set the size of the array to 0 */
    stage->n_bodies = 0;
}

void remove_all_forces (stage_t *stage) {

    /* simply set the size of the array to 0 */
    stage->n_forces = 0;
}

void draw_stage (stage_t *stage, cairo_t *cairo) {

    int width, height;
    cairo_surface_t *surface = cairo_get_target (cairo);
    width  = cairo_image_surface_get_width  (surface);
    height = cairo_image_surface_get_height (surface);

    /* TODO: this is just example code lol */

    /* Examples are in 1.0 x 1.0 coordinate space */
	cairo_scale (cairo, width, height);

	/* Drawing code goes here */
	cairo_set_source_rgb (cairo, 0, 0, 0);
	cairo_move_to (cairo, 0, 0);
	cairo_line_to (cairo, 1, 1);
	cairo_move_to (cairo, 1, 0);
	cairo_line_to (cairo, 0, 1);
	cairo_set_line_width (cairo, 0.2);
	cairo_stroke (cairo);

	cairo_rectangle (cairo, 0, 0, 0.5, 0.5);
	cairo_set_source_rgba (cairo, 1, 0, 0, 0.80);
	cairo_fill (cairo);

	cairo_rectangle (cairo, 0, 0.5, 0.5, 0.5);
	cairo_set_source_rgba (cairo, 0, 1, 0, 0.60);
	cairo_fill (cairo);

	cairo_rectangle (cairo, 0.5, 0, 0.5, 0.5);
	cairo_set_source_rgba (cairo, 0, 0, 1, 0.40);
	cairo_fill (cairo);
}

void advance_stage (stage_t *stage) {

}

stage_t *create_stage () {

    stage_t *stage   = (stage_t *)     calloc (1, sizeof (stage_t));
    stage->drawables = (drawable_t **) calloc (STAGE_CAPACITY, sizeof (drawable_t *));
    stage->bodies    = (body_t **)     calloc (STAGE_CAPACITY, sizeof (body_t *));
    stage->forces    = (force_t **)    calloc (STAGE_CAPACITY, sizeof (force_t *));
    return stage;
}

void destroy_stage (stage_t *stage) {

    remove_all_drawables (stage);
    remove_all_bodies    (stage);
    remove_all_forces    (stage);
    free (stage->drawables);
    free (stage->bodies);
    free (stage->forces);
    free (stage);
}
