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
    va_end(args);
    return FTO_OK;
}


extern enum FtoError fto_basis_poly2d_init(struct FtoPoly2D *poly, int num_elements, ...)
{
    poly->coeffs = fto_malloc_atomic(num_elements * sizeof *poly->coeffs);
    poly->orders = fto_malloc_atomic(2*num_elements * sizeof *poly->orders);
    poly->num_elements = num_elements;
    va_list args;
    va_start(args, num_elements);
    for (int element_ind = 0; element_ind < num_elements; ++element_ind)
    {
        poly->coeffs[element_ind] = va_arg(args, double);
        poly->orders[2*element_ind] = va_arg(args, int);
        poly->orders[2*element_ind+1] = va_arg(args, int);
    }
    va_end(args);
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


extern double fto_basis_poly2d_eval(const struct FtoPoly2D *poly, const double pt_x, const double pt_y)
{
    double result = 0;
    for (int element_ind = 0; element_ind < poly->num_elements; ++element_ind)
    {
        result += poly->coeffs[element_ind]
                * pow(pt_x, poly->orders[2*element_ind])
                * pow(pt_y, poly->orders[2*element_ind+1]);
    }
    return result;
}


extern enum FtoError fto_basis_poly2d_diff(const struct FtoPoly2D *poly, int axis, struct FtoPoly2D *diff_poly_out)
{
    double *coeffs = fto_malloc_atomic(poly->num_elements * sizeof *coeffs);
    int *orders = fto_malloc_atomic(2*poly->num_elements * sizeof *orders);
    int element_offset = 0;
    int num_elements = 0;
    for (int element_ind = 0; element_ind < poly->num_elements; ++element_ind)
    {
        if (poly->orders[2*element_ind+axis] > 0 && poly->coeffs[element_ind] != 0)
        {
            coeffs[element_offset] = poly->orders[2*element_ind+axis] * poly->coeffs[element_ind];
            orders[2*element_offset+axis] = poly->orders[2*element_ind+axis] - 1;
            orders[2*element_offset+(1-axis)] = poly->orders[2*element_ind+(1-axis)];
            ++num_elements;
            ++element_offset;
        }
    }
    *diff_poly_out = (struct FtoPoly2D){
        .num_elements = num_elements,
        .coeffs = coeffs,
        .orders = orders
    };
    return FTO_OK;
}


extern enum FtoError fto_basis_poly2d_mult(
        const struct FtoPoly2D *poly1, const struct FtoPoly2D *poly2, struct FtoPoly2D *poly_out)
{
    // currently makes no attempt at combining like terms
    int num_elements = poly1->num_elements * poly2->num_elements;
    int element_offset = 0;
    double *coeffs = fto_malloc_atomic(num_elements * sizeof *coeffs);
    int *orders = fto_malloc_atomic(2*num_elements * sizeof *coeffs);
    for (int element_ind1 = 0; element_ind1 < poly1->num_elements; ++element_ind1)
    {
        for (int element_ind2 = 0; element_ind2 < poly2->num_elements; ++element_ind2)
        {
            coeffs[element_offset] = poly1->coeffs[element_ind1] * poly2->coeffs[element_ind2];
            orders[2*element_offset] = poly1->orders[2*element_ind1] + poly2->orders[2*element_ind2];
            orders[2*element_offset+1] = poly1->orders[2*element_ind1+1] + poly2->orders[2*element_ind1+1];
            ++element_offset;
        }
    }
    *poly_out = (struct FtoPoly2D){
        .num_elements = num_elements,
        .orders = orders,
        .coeffs = coeffs
    };
    return FTO_OK;
}
