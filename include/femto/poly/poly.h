#ifndef FTO_BASIS_POLY_H
#define FTO_BASIS_POLY_H

/**
 * @file
 * @brief Polynomial support
 */

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
extern struct FtoPoly2D* fto_poly2d_zero();

/// @brief Initialize a two dimensional polynomial.
/// There must be 3*num_elements varargs arguments passed into this function. For each element,
/// provide a coefficient (double) and powers for the x and y variables (ints). For example, to
/// create the polynomial x^2*y + 3.5*x*y^2 + 1.5*y^3
/// @code
/// fto_poly2d_init(poly, 3, /* x^2*y */ 1.0, 2, 1, /* 3.5*x*y^2 */ 3.5, 1, 2, /* 1.5*y^3 */ 1.5, 0, 3)
/// @endcode
/// @param poly the polynomial to initialize.
/// @param num_elements number of elements of the polynomial
/// @param ... specification of the polynomial
/// @return status code
extern enum FtoError fto_poly2d_init(struct FtoPoly2D *poly, int num_elements, ...);
extern double fto_poly2d_eval(const struct FtoPoly2D *poly, double pt_x, double pt_y);
extern enum FtoError fto_poly2d_diff(
        const struct FtoPoly2D *poly, int axis, struct FtoPoly2D *diff_poly_out);
/**
 * Multiply two polynomials together and assign the result to poly_out. poly1 can equal poly2, and poly_mult can equal
 * poly1 or poly2.
 * @param poly1
 * @param poly2
 * @param poly_out
 * @return
 */
extern enum FtoError fto_poly2d_mult(
        const struct FtoPoly2D *poly1,
        const struct FtoPoly2D *poly2,
        struct FtoPoly2D *poly_out);
extern enum FtoError fto_poly2d_add(
        const struct FtoPoly2D *poly1,
        const struct FtoPoly2D *poly2,
        struct FtoPoly2D *poly_out);
extern enum FtoError fto_poly2d_scale(struct FtoPoly2D *poly, double scale);
extern enum FtoError fto_poly2d_ipow(const struct FtoPoly2D *poly, int pow, struct FtoPoly2D *poly_out);
extern bool fto_poly2d_iszero(const struct FtoPoly2D *poly);
/**
 * Substitutes existing variables with new variables, xnew and ynew.
 * @param poly The existing polynomial, in coordinates xold and yold.
 * @param xold The "xold" variable in terms of the new variables, xnew and ynew
 * @param yold The "yold" variable in terms of the new variables, xnew and ynew
 * @param poly_out The new polynomial, in terms of xnew and ynew.
 * @return
 */
extern enum FtoError fto_poly2d_substitute(
        const struct FtoPoly2D *poly,
        const struct FtoPoly2D *xold,
        const struct FtoPoly2D *yold,
        struct FtoPoly2D *poly_out
);
extern enum FtoError fto_poly2d_simplify(struct FtoPoly2D *poly);
extern bool fto_poly2d_isEqual(struct FtoPoly2D *poly1, struct FtoPoly2D *poly2);

extern enum FtoError fto_poly2d_print(const struct FtoPoly2D *poly);

#endif /* FTO_BASIS_POLY_H */
