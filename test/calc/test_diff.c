#include "femto/util.h"

#include "calc/test_diff.h"

#include "femto/calc/diff.h"

extern enum FtoError fto_test_calc_diff_addAll(struct FtoArray *tests)
{
    enum FtoError ret;
    if ((ret = FTO_TEST_APPEND(tests, fto_test_calc_grad_poly2d)) != FTO_OK)
        return ret;
    return FTO_OK;
}


extern void fto_test_calc_grad_poly2d(void **state)
{
    (void)state;
    struct FtoPoly2D *poly = fto_malloc(sizeof *poly);
    AOK(fto_poly2d_init(poly, 1, 1.0, 0, 0)); // 1 x^0 y^0
    struct FtoVectorFunc *grad = fto_malloc(sizeof *grad);
    AOK(fto_calc_grad_poly2d(poly, grad));
    AOK(fto_assert_int_equal(grad->order, 2))
    struct FtoGenericFunc *func1 = &grad->funcs[0];
    struct FtoGenericFunc *func2 = &grad->funcs[1];
    FTO_ASSERT(func1->type == FTO_POLYNOMIAL_2D);
    FTO_ASSERT(func2->type == FTO_POLYNOMIAL_2D);
    struct FtoPoly2D *poly1 = func1->state;
    struct FtoPoly2D *poly2 = func2->state;
    struct FtoPoly2D *zero = fto_poly2d_zero();
    FTO_ASSERT(fto_poly2d_isEqual(poly1, zero));
    FTO_ASSERT(fto_poly2d_isEqual(poly2, zero));

    AOK(fto_poly2d_init(poly, 1, 1.0, 1, 1)); // 1 x^1 y^1
    AOK(fto_calc_grad_poly2d(poly, grad));
    AOK(fto_assert_int_equal(grad->order, 2));
    func1 = &grad->funcs[0];
    func2 = &grad->funcs[1];
    FTO_ASSERT(func1->type == FTO_POLYNOMIAL_2D);
    FTO_ASSERT(func2->type == FTO_POLYNOMIAL_2D);
    poly1 = func1->state;
    poly2 = func2->state;
    struct FtoPoly2D *poly_x = fto_malloc(sizeof *poly_x);
    struct FtoPoly2D *poly_y = fto_malloc(sizeof *poly_y);
    AOK(fto_poly2d_init(poly_x, 1, 1.0, 1, 0)); // 1 x^1 y^0
    AOK(fto_poly2d_init(poly_y, 1, 1.0, 0, 1)); // 1 x^0 y^1
    FTO_ASSERT(fto_poly2d_isEqual(poly1, poly_y));
    FTO_ASSERT(fto_poly2d_isEqual(poly2, poly_x));
}
