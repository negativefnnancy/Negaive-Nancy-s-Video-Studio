#include "matrix.h"

vec3_t make_vec3 (double x, double y, double z) {

    vec3_t vec;
    vec.x = x;
    vec.y = y;
    vec.z = z;
    return vec;
}

mat3_t make_mat3 (vec3_t x, vec3_t y, vec3_t z) {

    mat3_t mat;
    mat.x = x;
    mat.y = y;
    mat.z = z;
    return mat;
}

vec3_t add_vec3 (vec3_t a, vec3_t b) {

    return make_vec3 (a.x + b.x,
                      a.y + b.y,
                      a.z + b.z);
}

mat3_t add_mat3 (mat3_t a, mat3_t b) {

    return make_mat3 (add_vec3 (a.x, b.x),
                      add_vec3 (a.y, b.y),
                      add_vec3 (a.z, b.z));
}
