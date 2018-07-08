#ifndef FTO_BASIS_POLY_H
#define FTO_BASIS_POLY_H

#include "femto/util.h"

struct FtoPoly1D
{
    double *coeffs;
    int order;
};

extern enum FtoError fto_basis_poly1d_init(struct FtoPoly1D *poly, int order, ...);
extern double fto_basis_poly1d_eval(const struct FtoPoly1D *poly, double pt_x);
extern enum FtoError fto_basis_poly1d_diff(const struct FtoPoly1D *poly, struct FtoPoly1D *diff_poly_out);

#endif /* FTO_BASIS_POLY_H */