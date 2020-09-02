#include <math.h>

#include "shape.h"

void trace_shape (shape_t *shape, cairo_t *cairo) {

    /* polymorphic dispatch */
    shape->trace (shape, cairo);
}

bool inside_shape (shape_t *shape, vec2_t point) {

    /* polymorphic dispatch */
    return shape->inside (shape, point);
}

void destroy_shape (shape_t *shape) {

    /* polymorphic dispatch */
    shape->destroy (shape);

    free (shape);
}

shape_t *create_shape (void *data,
                       shape_trace_method_t *trace,
                       shape_inside_method_t *inside,
                       shape_destructor_t *destroy) {

    shape_t *shape = (shape_t *) calloc (1, sizeof (shape_t));
    shape->data    = data;
    shape->trace   = trace;
    shape->inside  = inside;
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

bool inside_ellipse (shape_t *shape, vec2_t point) {

    shape_ellipse_t *ellipse = (shape_ellipse_t *) shape->data;

    /* to tell whether a point is inside an elipse,
     * got to transform the point to coordinates of a unit circle
     * and then determine whether the point is inside the unit circle
     * by whether the distance from the center to the point is less than
     * the radius */

    /* divide the point by the size of the ellipse */
    vec2_t scaled = make_vec2 (point.x / ellipse->size.x,
                               point.y / ellipse->size.y);

    /* the distance the scaled point is from the center of the unit circle */
    double distance = length_vec2 (scaled);

    /* whether the point is inside the unit circle */
    return distance < 1;
}

void destroy_ellipse (shape_t *shape) {

    free (shape->data);
}

shape_t *create_ellipse (vec2_t size) {

    shape_ellipse_t *ellipse =
        (shape_ellipse_t *) calloc (1, sizeof (shape_ellipse_t));
    ellipse->size = size;

    return create_shape (ellipse, trace_ellipse, inside_ellipse, destroy_ellipse);
}

void trace_rectangle (shape_t *shape, cairo_t *cairo) {

    shape_rectangle_t *rectangle = (shape_rectangle_t *) shape->data;

    cairo_new_path  (cairo);
    cairo_rectangle (cairo, -rectangle->size.x / 2,
                            -rectangle->size.y / 2,
                             rectangle->size.x,
                             rectangle->size.y);
}

bool inside_rectangle (shape_t *shape, vec2_t point) {

    shape_rectangle_t *rectangle = (shape_rectangle_t *) shape->data;

    /* whether a point is inside a rectangle is just 2 tests one along each dimension
     * of whether its in the bounds! */

    bool x_in = fabs (point.x) < rectangle->size.x / 2;
    bool y_in = fabs (point.y) < rectangle->size.y / 2;

    return x_in && y_in;
}

void destroy_rectangle (shape_t *shape) {

    free (shape->data);
}

shape_t *create_rectangle (vec2_t size) {

    shape_rectangle_t *rectangle =
        (shape_rectangle_t *) calloc (1, sizeof (shape_rectangle_t));
    rectangle->size = size;

    return create_shape (rectangle, trace_rectangle, inside_rectangle, destroy_rectangle);
}

void add_vertex (shape_t *shape, vec2_t vertex) {

    shape_polygon_t *polygon = (shape_polygon_t *) shape->data;
    polygon->vertices[polygon->n_vertices++] = vertex;
}

void trace_polygon (shape_t *shape, cairo_t *cairo) {

    int i;
    vec2_t vertex;
    shape_polygon_t *polygon = (shape_polygon_t *) shape->data;
    vertex = polygon->vertices[polygon->n_vertices - 1];

    /* begin a new path and iterate all the vertices of the polygon */
    cairo_new_path (cairo);
	cairo_move_to  (cairo, vertex.x, vertex.y);
    for (i = 0; i < polygon->n_vertices; i++) {
        
        vertex = polygon->vertices[i];
	    cairo_line_to (cairo, vertex.x, vertex.y);
    }
}

bool inside_polygon (shape_t *shape, vec2_t point) {

    int i;
    shape_polygon_t *polygon = (shape_polygon_t *) shape->data;

   /* using the crossing number algorithm */
   int crossings = 0;

   /* iterate the lines along all the edges of the polygon */
   for (i = 0; i < polygon->n_vertices; i++) {

        /* the two vertices of this edge */
        vec2_t a = polygon->vertices[i];
        vec2_t b = polygon->vertices[(i + 1) % polygon->n_vertices];

        /* get the line formula for this edge relative to the point in question */
        double dx = b.x - a.x; /* change in x */
        double dy = b.y - a.y; /* change in y */
        double slope = dy / dx; /* change in y over change in x */

        /* find the y intercept relative to the point in question */
        double rx = point.x - a.x; /* x ways of point from a */
        double y_intercept = a.y + slope * rx / dx;

        /* solve for the intersection point on the edge with the x axis
         * y = mx + b
         * y = 0
         * 0 = mx + b
         * b = -mx
         * -b / m = x */
        double x_intercept = -y_intercept / slope;

        /* the -> facing x axis ray intersects with the edge if...
         * x intercept > 0 and
         * a.x < x intercept < b.x */
        bool intersects = x_intercept > 0 && a.x < x_intercept && x_intercept < b.x;

        /* count the number of intersections with edges */
        if (intersects)
            crossings++;
    }

    /* if crossings is odd, only then is point in polygon */
    return crossings % 2 == 1;
}

void destroy_polygon (shape_t *shape) {

    shape_polygon_t *polygon = (shape_polygon_t *) shape->data;

    free (polygon->vertices);
    free (polygon);
}

shape_t *create_polygon () {

    shape_polygon_t *polygon =
        (shape_polygon_t *) calloc (1, sizeof (shape_polygon_t));
    polygon->n_vertices = 0;
    polygon->vertices = (vec2_t *) calloc (VERTEX_CAPACITY, sizeof (vec2_t));

    return create_shape (polygon, trace_polygon, inside_polygon, destroy_polygon);
}
