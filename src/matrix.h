#ifndef MATRIX_H
#define MATRIX_H

/* a 3 by 3 matrix representing 2d affine transformations */

typedef struct matrix_t {

    struct {

        double x;
        double y;
        double z;

    } x;

    struct {

        double x;
        double y;
        double z;

    } y;

    struct {

        double x;
        double y;
        double z;

    } z;

} matrix_t;

#endif /* MATRIX_H */
