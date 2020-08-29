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
