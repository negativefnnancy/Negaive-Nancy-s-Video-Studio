#include <math.h>

#include "vector.h"

vec2_t make_vec2 (double x, double y) {

    vec2_t vec;
    vec.x = x;
    vec.y = y;
    return vec;
}

vec2_t add_vec2 (vec2_t a, vec2_t b) {

    vec2_t vec;
    vec.x = a.x + b.x;
    vec.y = a.y + b.y;
    return vec;
}

vec2_t subtract_vec2 (vec2_t a, vec2_t b) {

    vec2_t vec;
    vec.x = a.x - b.x;
    vec.y = a.y - b.y;
    return vec;
}

vec2_t multiply_vec2_scalar (vec2_t vec, double scalar) {

    vec.x *= scalar;
    vec.y *= scalar;
    return vec;
}

double cross_vec2 (vec2_t a, vec2_t b) {

    return a.x * b.y - a.y * b.x;
}

double dot_vec2 (vec2_t a, vec2_t b) {

    return a.x * b.x + a.y * b.y;
}

double length_vec2 (vec2_t vec) {

    return sqrt (vec.x * vec.x + vec.y * vec.y);
}

double distance_vec2 (vec2_t a, vec2_t b) {

    return length_vec2 (subtract_vec2 (a, b));
}

vec2_t direction_vec2 (vec2_t a, vec2_t b) {

    vec2_t delta = subtract_vec2 (b, a);
    double length = length_vec2 (delta);
    return multiply_vec2_scalar (delta, 1. / length);
}

double angle_vec2 (vec2_t vec) {

    return atan2 (vec.y, vec.x);
}

vec2_t rotate_vec2 (vec2_t vec) {

    vec2_t rotated;
    rotated.x = -vec.y;
    rotated.y = vec.x;
    return rotated;
}

vec2_t rotate_vec2_radians (vec2_t vec, double radians) {

    /* first get the vector in polar coordinates */
    double length = length_vec2 (vec);
    double angle  = atan2 (vec.y, vec.x);

    /* add to the angle and convert back to cartesian coordinates */
    angle += radians;
    return multiply_vec2_scalar (make_vec2 (cos (angle), sin (angle)), length);
}

vec2_t multiply_vec2_matrix (vec2_t vec, cairo_matrix_t mat) {

    return make_vec2 (vec.x * mat.xx + vec.y * mat.xy + mat.x0,
                      vec.y * mat.xy + vec.y * mat.yy + mat.y0);
}
