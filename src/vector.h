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

/* subtract two vectors */
vec2_t subtract_vec2 (vec2_t a, vec2_t b);

/* multiply a vector by a scalar */
vec2_t multiply_vec2_scalar (vec2_t vec, double scalar);

/* get the magnitude of the cross product of two vectors */
double cross_vec2 (vec2_t a, vec2_t b);

/* get the dot product of two vectors */
double dot_vec2 (vec2_t a, vec2_t b);

/* get the length of a vector */
double length_vec2 (vec2_t vec);

/* get the distance between two points */
double distance_vec2 (vec2_t a, vec2_t b);

/* get the unit direction from point a to point b */
vec2_t direction_vec2 (vec2_t a, vec2_t b);

/* get the angle of a vector aroud the origin */
double angle_vec2 (vec2_t vec);

/* rotate a vector 90 degrees clockwise */
vec2_t rotate_vec2 (vec2_t vec);

#endif /* VECTOR_H */
