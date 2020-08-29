#include "stage.h"

void integrate_body (body_t *body, double delta_time) {

    /* TODO: do better than this forward euler integration (maybe) */

    /* assuming acceleration has already been calculated elsewhere */

    vec2_t scaled_velocity;
    vec2_t scaled_acceleration;

    /* scale the derivatives by the delta time first */
    scaled_velocity     = multiply_vec2_scalar (body->velocity,     delta_time);
    scaled_acceleration = multiply_vec2_scalar (body->acceleration, delta_time);

    /* integrate translational state */
    body->position = add_vec2 (body->position, scaled_velocity);
    body->velocity = add_vec2 (body->velocity, scaled_acceleration);

    /* integrate rotational state */
    body->angle            += body->angular_velocity     * delta_time;
    body->angular_velocity += body->angular_acceleration * delta_time;

    /* update the transformation from the new state */
    cairo_matrix_init_translate (body->transformation,
                                 body->position.x,
                                 body->position.y);
    cairo_matrix_rotate (body->transformation, body->angle);
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

    remove_array_item ((void **) stage->drawables, &(stage->n_drawables), drawable);
}

void remove_body (stage_t *stage, body_t *body) {

    remove_array_item ((void **) stage->bodies, &(stage->n_bodies), body);
}

void remove_forces (stage_t *stage, force_t *force) {

    remove_array_item ((void **) stage->forces, &(stage->n_forces), force);
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
    for (i = 0; i < stage->n_drawables; i++)
        draw_drawable (stage->drawables[i], cairo);
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

    free (stage->drawables);
    free (stage->bodies);
    free (stage->forces);
    free (stage);
}
