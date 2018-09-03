#include "femto/util.h"

#include "test_poly.h"


extern enum FtoError fto_test_poly_poly_addAll(struct FtoArray *tests)
{
    AOK(FTO_TEST_APPEND(tests, fto_test_poly_poly2d_init));
    AOK(FTO_TEST_APPEND(tests, fto_test_poly_poly2d_eval));
    AOK(FTO_TEST_APPEND(tests, fto_test_poly_poly2d_diff));
    return FTO_OK;
}


extern void fto_test_poly_poly2d_init(void **state)
{
    (void)state;
    struct FtoPoly2D *poly = fto_malloc(sizeof *poly);
    AOK(fto_poly2d_init(poly, 0));
    FTO_ASSERT(poly->num_elements == 0);
    AOK(fto_poly2d_init(poly, 1, 1.0, 0, 0));
    FTO_ASSERT(poly->num_elements == 1);
    FTO_ASSERT(fto_isClose_default(poly->coeffs[0], 1.0));
    FTO_ASSERT(poly->orders[0] == 0);
    FTO_ASSERT(poly->orders[1] == 0);
}


extern void fto_test_poly_poly2d_eval(void **state)
{
    (void)state;
    struct FtoPoly2D *poly = fto_malloc(sizeof *poly);
    AOK(fto_poly2d_init(poly, 0));
    FTO_ASSERT(fto_isClose_default(fto_poly2d_eval(poly, 1.5, 3.8), 0));
    AOK(fto_poly2d_init(poly, 2, 1.0, 1, 0, 1.0, 0, 1));
    FTO_ASSERT(fto_isClose_default(fto_poly2d_eval(poly, 1.5, 2.3), 3.8));
    AOK(fto_poly2d_init(poly, 1, 2.0, 1, 1));
    FTO_ASSERT(fto_isClose_default(fto_poly2d_eval(poly, 2.0, 4.0), 16.0));
}


extern void fto_test_poly_poly2d_diff(void **state)
{
    (void)state;
    struct FtoPoly2D *poly = fto_malloc(sizeof *poly);
    AOK(fto_poly2d_init(poly, 1, 1.0, 1, 1));
    struct FtoPoly2D *poly_diff = fto_malloc(sizeof *poly_diff);
    AOK(fto_poly2d_diff(poly, 0, poly_diff));
    FTO_ASSERT(fto_isClose_default(poly_diff->coeffs[0], 1.0));
    FTO_ASSERT(poly_diff->orders[0] == 0);
    FTO_ASSERT(poly_diff->orders[1] == 1);
    AOK(fto_poly2d_diff(poly, 1, poly_diff));
    FTO_ASSERT(fto_isClose_default(poly_diff->coeffs[0], 1.0));
    FTO_ASSERT(poly_diff->orders[0] == 1);
    FTO_ASSERT(poly_diff->orders[1] == 0);
    AOK(fto_poly2d_init(poly, 1, 3.0, 2, 3));
    AOK(fto_poly2d_diff(poly, 0, poly_diff));
    FTO_ASSERT(fto_isClose_default(poly_diff->coeffs[0], 6.0));
    FTO_ASSERT(poly_diff->orders[0] == 1);
    FTO_ASSERT(poly_diff->orders[1] == 3);
    AOK(fto_poly2d_diff(poly, 1, poly_diff));
    FTO_ASSERT(fto_isClose_default(poly_diff->coeffs[0], 9.0));
    FTO_ASSERT(poly_diff->orders[0] == 2);
    FTO_ASSERT(poly_diff->orders[1] == 2);
}


extern void fto_test_poly_poly2d_mult(void **state)
{
    (void)state;

}
