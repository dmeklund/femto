#include "femto/util.h"
#include "femto/basis/poly.h"

#include <math.h>
#include <stdarg.h>


extern enum FtoError fto_basis_poly1d_init(struct FtoPoly1D *poly, int order, ...)
{
    enum FtoError ret;
    if ((ret = fto_assert_greaterThanEqual(order, 0)) != FTO_OK) return ret;
    va_list args;
    va_start(args, order);
    poly->order = order;
    poly->coeffs = fto_malloc_atomic((1+order) * sizeof *poly->coeffs);
    for (int coeff_ind = 0; coeff_ind <= order; ++coeff_ind)
    {
        poly->coeffs[coeff_ind] = va_arg(args, double);
    }
    return FTO_OK;
}


extern double fto_basis_poly1d_eval(const struct FtoPoly1D *poly, double pt_x)
{
    double result = 0;
    double mult = 1;
    for (int coeff_ind = 0; coeff_ind <= poly->order; ++coeff_ind)
    {
        result += poly->coeffs[coeff_ind] * mult;
        mult *= pt_x;
    }
    return result;
}


extern enum FtoError fto_basis_poly1d_diff(const struct FtoPoly1D *poly, struct FtoPoly1D *diff_poly_out)
{
    diff_poly_out->order = poly->order - 1;
    diff_poly_out->coeffs = fto_malloc_atomic((diff_poly_out->order + 1) * sizeof *diff_poly_out->coeffs);
    for (int coeff_ind = 0; coeff_ind <= diff_poly_out->order; ++coeff_ind)
    {
        diff_poly_out->coeffs[coeff_ind] = (coeff_ind+1) * poly->coeffs[coeff_ind+1];
    }
    return FTO_OK;
}
