#include "stage.h"

void draw_drawable (drawable_t *drawable, cairo_t *cairo) {

    /* TODO: polymorphic drawable types */

    /* TODO: this is just test stuff lol */
	cairo_rectangle (cairo, -0.05, -0.05, 0.1, 0.1);
	cairo_set_source_rgb (cairo, 0, 1, 0);
	cairo_fill (cairo);
}

void integrate_body (body_t *body, double delta_time) {

    /* assuming acceleration has already been calculated elsewhere */

    cairo_matrix_t scaled_velocity     = body->velocity;
    cairo_matrix_t scaled_acceleration = body->acceleration;

/*
    cairo_matrix_scalar_multiply (&(body->velocity),     delta_time);
    cairo_matrix_scalar_multiply (&(body->acceleration), delta_time);
    */

    /* TODO: do better than this forward euler integration */
    cairo_matrix_multiply (body->position,    body->position,    &scaled_velocity);
/*    cairo_matrix_multiply (&(body->velocity), &(body->velocity), &scaled_acceleration);
*/
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

    int width, height, i;
    double half_height;
    double ratio;
    cairo_surface_t *surface;

    surface     = cairo_get_target (cairo);
    width       = cairo_image_surface_get_width  (surface);
    height      = cairo_image_surface_get_height (surface);
    ratio       = (double) width / height;
    half_height = height / 2.;

    /* normalized height units */
    cairo_translate (cairo, width / 2, half_height);
    cairo_scale     (cairo, half_height, half_height);

    /* TODO: remove this temporary drawing stuff */
	cairo_set_source_rgb (cairo, 1, 0, 0);
	cairo_move_to (cairo, -1, -1);
	cairo_line_to (cairo, 1, 1);
	cairo_move_to (cairo, -1, 1);
	cairo_line_to (cairo, 1, -1);
	cairo_set_line_width (cairo, 0.2);
	cairo_stroke (cairo);

    /* draw all the drawables */
    for (i = 0; i < stage->n_drawables; i++) {

        drawable_t *drawable = stage->drawables[i];

        /* transform by the transformation of each drawables */
        cairo_save (cairo);
        cairo_transform (cairo, &(drawable->transformation));
        draw_drawable (drawable, cairo);
        cairo_restore (cairo);
    }
}

void advance_stage (stage_t *stage, double delta_time) {

    int i;

    /* compute acceleration for all bodies from forces */
    /* TODO */

    /* integrate all the bodies */
    for (i = 0; i < stage->n_bodies; i++)
        integrate_body (stage->bodies[i], delta_time);
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
