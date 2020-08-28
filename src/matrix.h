#ifndef MATRIX_H
#define MATRIX_H

/* a 3 dimensional vector */
typedef struct vec3_t {

    double x;
    double y;
    double z;

} vec3_t;

/* make an initialized vector */
vec3_t make_vec3 (double x, double y, double z);

/* add two vectors together */
vec3_t add_vec3 (vec3_t a, vec3_t b);

/* a 3 by 3 matrix representing 2d affine transformations */
typedef struct mat3_t {

    vec3_t x;
    vec3_t y;
    vec3_t z;

} mat3_t;

/* make an initialized matrix */
mat3_t make_mat3 (vec3_t x, vec3_t y, vec3_t z);

/* add two matrices together */
mat3_t add_mat3 (mat3_t a, mat3_t b);

#endif /* MATRIX_H */
