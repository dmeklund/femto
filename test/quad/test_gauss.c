#include "femto/util.h"

#include "femto/poly/poly.h"
#include "femto/quad/gauss.h"
#include "femto/geom/point.h"

#include "quad/test_gauss.h"
#include "fto_test.h"

#include <math.h>


extern void fto_test_quad_gauss_integrate1d(void **state)
{
    (void)state;
    struct FtoPoly1D *poly = fto_malloc(sizeof *poly);
    AOK(fto_poly1d_init(poly, 1, 0.0, 1.0));
    struct FtoGenericFunc *func = fto_malloc(sizeof *func);
    AOK(fto_function_fromPoly1D(poly, func));
    double a = 0;
    double b = 1;
    int num_nodes = 2;
    double result;
    AOK(fto_gauss_integrate1d(func, a, b, num_nodes, &result));
    AOK(fto_assertClose(result, 0.5, FTO_DEFAULT_RTOL, FTO_DEFAULT_ATOL));
}


extern void fto_test_quad_gauss_integrate2d_triangle(void **state)
{
    (void)state;
    struct FtoPoly2D *poly = fto_malloc(sizeof *poly);
    fto_poly2d_init(poly, 1, 1.0, 0, 0); // constant value of 1
    struct FtoGenericFunc *func = fto_malloc(sizeof *func);
    fto_function_fromPoly2D(poly, func);

    struct Fto2DTriangle *triangle = fto_2dtriangle_new(
            fto_2dpoint_new(0, 0), fto_2dpoint_new(0, 1), fto_2dpoint_new(1, 0));
    int num_nodes = 1;
    double result;
    fto_gauss_integrate2d_triangle(func, triangle, num_nodes, &result);
    FTO_ASSERT(fto_isClose_default(result, 0.5));
}


extern void fto_test_quad_gauss_integrate2d_line(void **state)
{
    (void)state;
    struct FtoPoly2D *poly = fto_malloc(sizeof *poly);
    fto_poly2d_init(poly, 2, 1.0, 2, 0, 1.0, 0, 2); // x^2 + y^2
    struct FtoGenericFunc *func = fto_malloc(sizeof *func);
    fto_function_fromPoly2D(poly, func);
    int num_nodes = 3;
    double result;
    struct Fto2DLine line = fto_2dline_fromDoubles(1, 0, 1, 2); // x=1 => f(x,y) = 1.0 + y^2
    fto_gauss_integrate2d_line(func, &line, num_nodes, &result);
    AOK(fto_assertClose(result, 2 + 8.0/3, FTO_DEFAULT_RTOL, FTO_DEFAULT_ATOL));
    line = fto_2dline_fromDoubles(0, 0, 1, 1);  // y=x => f(x,y) = 2*x^2
    fto_gauss_integrate2d_line(func, &line, num_nodes, &result);
    AOK(fto_assertClose(result, sqrt(2) * 2.0/3, FTO_DEFAULT_RTOL, FTO_DEFAULT_ATOL));
}


extern enum FtoError fto_test_quad_gauss_addAll(struct FtoArray *tests)
{
    enum FtoError ret;
    if ((ret = FTO_TEST_APPEND(tests, fto_test_quad_gauss_integrate1d)) != FTO_OK) return ret;
    if ((ret = FTO_TEST_APPEND(tests, fto_test_quad_gauss_integrate2d_triangle)) != FTO_OK) return ret;
    if ((ret = FTO_TEST_APPEND(tests, fto_test_quad_gauss_integrate2d_line)) != FTO_OK) return ret;
    return FTO_OK;
}
