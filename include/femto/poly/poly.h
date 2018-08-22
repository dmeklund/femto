#ifndef FTO_BASIS_POLY_H
#define FTO_BASIS_POLY_H

#include "femto/util.h"

struct FtoPoly1D
{
    double *coeffs;
    int order;
};

struct FtoPoly2D
{
    // e.g., for x^2*y + 3.5*x*y^2 + 1.5*y^3
    // coeffs = [1.0 3.5 1.5]
    // orders = [2 1 1 2 0 3]
    // num_elements = 3
    double *coeffs;
    int *orders; // C-ordered (num_elements,2) array
    int num_elements;
};

extern enum FtoError fto_poly1d_init(struct FtoPoly1D *poly, int order, ...);
extern double fto_poly1d_eval(const struct FtoPoly1D *poly, double pt_x);
extern enum FtoError fto_poly1d_diff(const struct FtoPoly1D *poly, struct FtoPoly1D *diff_poly_out);

// e.g., for x^2*y + 3.5*x*y^2 + 1.5*y^3:
// fto_poly2d_init(poly, 3, /* x^2*y */ 1.0, 2, 1, /* 3.5*x*y^2 */ 3.5, 1, 2, /* 1.5*y^3 */ 1.5, 0, 3)
extern enum FtoError fto_poly2d_init(struct FtoPoly2D *poly, int num_elements, ...);
extern double fto_poly2d_eval(const struct FtoPoly2D *poly, double pt_x, double pt_y);
extern enum FtoError fto_poly2d_diff(
        const struct FtoPoly2D *poly, int axis, struct FtoPoly2D *diff_poly_out);
extern enum FtoError fto_poly2d_mult(
        const struct FtoPoly2D *poly1,
        const struct FtoPoly2D *poly2,
        struct FtoPoly2D *poly_out);
extern enum FtoError fto_poly2d_add(
        const struct FtoPoly2D *poly1,
        const struct FtoPoly2D *poly2,
        struct FtoPoly2D *poly_out);

extern enum FtoError fto_poly2d_print(const struct FtoPoly2D *poly);

#endif /* FTO_BASIS_POLY_H */
