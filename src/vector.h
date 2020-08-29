#ifndef VECTOR_H
#define VECTOR_H

/* represents a 2d vector */
typedef struct vec2_t {

    double x;
    double y;

} vec2_t;

/* return an initialized vector */
vec2_t make_vec2 (double x, double y);

/* add two vectors */
vec2_t add_vec2 (vec2_t a, vec2_t b);

/* multiply a vector by a scalar */
vec2_t multiply_vec2_scalar (vec2_t vec, double scalar);

#endif /* VECTOR_H */
