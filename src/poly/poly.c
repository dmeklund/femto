#include "femto/util.h"
#include "femto/poly/poly.h"

#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <femto/util/array.h>

extern enum FtoError fto_poly1d_init(struct FtoPoly1D *poly, int order, ...)
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


extern enum FtoError fto_poly2d_init(struct FtoPoly2D *poly, int num_elements, ...)
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


extern double fto_poly1d_eval(const struct FtoPoly1D *poly, double pt_x)
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


extern enum FtoError fto_poly1d_diff(const struct FtoPoly1D *poly, struct FtoPoly1D *diff_poly_out)
{
    diff_poly_out->order = poly->order - 1;
    diff_poly_out->coeffs = fto_malloc_atomic((diff_poly_out->order + 1) * sizeof *diff_poly_out->coeffs);
    for (int coeff_ind = 0; coeff_ind <= diff_poly_out->order; ++coeff_ind)
    {
        diff_poly_out->coeffs[coeff_ind] = (coeff_ind+1) * poly->coeffs[coeff_ind+1];
    }
    return FTO_OK;
}


extern double fto_poly2d_eval(const struct FtoPoly2D *poly, double pt_x, double pt_y)
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


extern enum FtoError fto_poly2d_diff(const struct FtoPoly2D *poly, int axis, struct FtoPoly2D *diff_poly_out)
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


extern enum FtoError fto_poly2d_mult(
        const struct FtoPoly2D *poly1, const struct FtoPoly2D *poly2, struct FtoPoly2D *poly_out)
{
    // does not simplify
    int num_elements = poly1->num_elements * poly2->num_elements;
    int element_offset = 0;
    double *coeffs = fto_malloc_atomic(num_elements * sizeof *coeffs);
    int *orders = fto_malloc_atomic(2*num_elements * sizeof *orders);
    for (int element_ind1 = 0; element_ind1 < poly1->num_elements; ++element_ind1)
    {
        for (int element_ind2 = 0; element_ind2 < poly2->num_elements; ++element_ind2)
        {
            coeffs[element_offset] = poly1->coeffs[element_ind1] * poly2->coeffs[element_ind2];
            orders[2*element_offset] = poly1->orders[2*element_ind1] + poly2->orders[2*element_ind2];
            orders[2*element_offset+1] = poly1->orders[2*element_ind1+1] + poly2->orders[2*element_ind2+1];
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


extern enum FtoError fto_poly2d_add(
        const struct FtoPoly2D *poly1,
        const struct FtoPoly2D *poly2,
        struct FtoPoly2D *poly_out)
{
    int num_elements = poly1->num_elements + poly2->num_elements;
    double *coeffs = fto_malloc_atomic(num_elements * sizeof *coeffs);
    int *orders = fto_malloc_atomic(2*num_elements * sizeof *orders);
    memcpy(coeffs, poly1->coeffs, poly1->num_elements * sizeof *coeffs);
    memcpy(orders, poly1->orders, 2*poly1->num_elements * sizeof *orders);
    memcpy(coeffs + poly1->num_elements, poly2->coeffs, poly2->num_elements * sizeof *coeffs);
    memcpy(orders + 2*poly1->num_elements, poly2->orders, 2*poly2->num_elements * sizeof *orders);
    *poly_out = (struct FtoPoly2D){
        .num_elements = num_elements,
        .orders = orders,
        .coeffs = coeffs
    };
    return FTO_OK;
}


extern enum FtoError fto_poly2d_print(const struct FtoPoly2D *poly)
{
    for (int element_ind = 0; element_ind < poly->num_elements; ++element_ind)
    {
        printf("%g x^%d y^%d", poly->coeffs[element_ind], poly->orders[2*element_ind], poly->orders[2*element_ind+1]);
        if (element_ind < poly->num_elements-1)
            printf(" + ");
    }
    printf("\n");
    return FTO_OK;
}


extern enum FtoError fto_poly2d_simplify(struct FtoPoly2D *poly)
{
    for (int ind1 = 0; ind1 < poly->num_elements; ++ind1)
    {
        for (int ind2 = ind1+1; ind2 < poly->num_elements; ++ind2)
        {
            if (poly->orders[2*ind1] == poly->orders[2*ind2] && poly->orders[2*ind1+1] == poly->orders[2*ind2+1])
            {
                poly->coeffs[ind1] += poly->coeffs[ind2];
                poly->coeffs[ind2] = 0;
            }
        }
    }
    int copyFrom_ind = 0;
    int new_num_elements = poly->num_elements;
    for (int element_ind = 0; element_ind < poly->num_elements && copyFrom_ind < poly->num_elements; ++element_ind)
    {
        while (copyFrom_ind < poly->num_elements && poly->coeffs[copyFrom_ind] == 0)
        {
            ++copyFrom_ind;
            --new_num_elements;
        }
        if (copyFrom_ind != element_ind && copyFrom_ind < poly->num_elements)
        {
            poly->coeffs[element_ind] = poly->coeffs[copyFrom_ind];
            poly->orders[2*element_ind] = poly->orders[2*copyFrom_ind];
            poly->orders[2*element_ind+1] = poly->orders[2*copyFrom_ind+1];
        }
        ++copyFrom_ind;
    }
    poly->num_elements = new_num_elements;
    return FTO_OK;
}


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
        struct FtoPoly2D *poly_out)
{
    enum FtoError ret;
    poly_out->num_elements = 0;
    for (int element_ind = 0; element_ind < poly->num_elements; ++element_ind)
    {
        struct FtoPoly2D *element_poly = fto_malloc(sizeof *element_poly);
        int order_x = poly->orders[2*element_ind];
        int order_y = poly->orders[2*element_ind+1];
        if ((ret = fto_poly2d_ipow(xold, order_x, element_poly)) != FTO_OK) return ret;
        struct FtoPoly2D *element_poly_y = fto_malloc(sizeof *element_poly_y);
        if ((ret = fto_poly2d_ipow(yold, order_y, element_poly_y)) != FTO_OK) return ret;
        if ((ret = fto_poly2d_mult(element_poly, element_poly_y, element_poly)) != FTO_OK) return ret;
        if ((ret = fto_poly2d_scale(element_poly, poly->coeffs[element_ind])) != FTO_OK) return ret;
        if ((ret = fto_poly2d_add(poly_out, element_poly, poly_out)) != FTO_OK) return ret;
    }
    return FTO_OK;
}


extern enum FtoError fto_poly2d_scale(struct FtoPoly2D *poly, const double scale)
{
    for (int element_ind = 0; element_ind < poly->num_elements; ++element_ind)
    {
        poly->coeffs[element_ind] *= scale;
    }
    return FTO_OK;
}


extern enum FtoError fto_poly2d_ipow(const struct FtoPoly2D *poly, int pow, struct FtoPoly2D *poly_out)
{
    enum FtoError ret;
    if (pow < 0)
    {
        return fto_err_set(FTO_INVALID_ARG, "fto_poly2d_ipow only supports non-negative integers");
    }
    if (pow == 0)
    {
        if (fto_poly2d_iszero(poly))
        {
            return fto_err_set(FTO_ILLEGAL_OP, "Zero taken to zeroth power");
        }
        *poly_out = (struct FtoPoly2D){
            .num_elements = 1,
            .coeffs = fto_doubleArray_new(1, 1.0),
            .orders = fto_intArray_new(2, 0, 0)
        };
        return FTO_OK;
    }
    *poly_out = *poly;
    for (int count = 1; count < pow; ++count)
    {
        if ((ret = fto_poly2d_mult(poly_out, poly, poly_out)) != FTO_OK) return ret;
    }
    return FTO_OK;
}


extern bool fto_poly2d_iszero(const struct FtoPoly2D *poly)
{
    if (poly->num_elements == 0) return true;
    for (int element_ind = 0; element_ind < poly->num_elements; ++element_ind)
    {
        if (fabs(poly->coeffs[element_ind]) > 0)
            return false;
    }
    return true;
}


extern bool fto_poly2d_isEqual(struct FtoPoly2D *poly1, struct FtoPoly2D *poly2)
{
    fto_poly2d_simplify(poly1);
    fto_poly2d_simplify(poly2);
    if (poly1->num_elements != poly2->num_elements) return false;
    for (int element_ind1 = 0; element_ind1 < poly1->num_elements; ++element_ind1)
    {
        bool element_match = false;
        for (int element_ind2 = 0; element_ind2 < poly2->num_elements; ++element_ind2)
        {
            if (fto_intArray_allEqual(2, &poly1->orders[2*element_ind1], &poly2->orders[2*element_ind2]) &&
                    fto_isClose_default(poly1->coeffs[element_ind1], poly2->coeffs[element_ind2]))
            {
                element_match = true;
                break;
            }
        }
        if (!element_match)
            return false;
    }
    return true;
}
