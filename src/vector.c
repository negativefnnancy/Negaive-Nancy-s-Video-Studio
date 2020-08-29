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

vec2_t multiply_vec2_scalar (vec2_t vec, double scalar) {

    vec.x *= scalar;
    vec.y *= scalar;
    return vec;
}
