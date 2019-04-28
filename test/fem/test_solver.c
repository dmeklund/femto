#include "femto/util.h"
#include "fto_test.h"
#include "fem/test_solver.h"

#include "femto/fem/solver.h"
#include "femto/geom/shapes.h"
#include "femto/basis/piecewise_poly.h"

#include <assert.h>
#include <stdio.h>


extern enum FtoError fto_test_fem_solver_addAll(struct FtoArray *tests)
{
    AOK(FTO_TEST_APPEND(tests, fto_test_fem_solver_solve));
    return FTO_OK;
}

struct GFuncUserData
{
    double a;
    double b;
};

static double gFuncImpl(double x, double y, void *user_data)
{
    struct GFuncUserData *gFunc_userData = user_data;
//    const double a = gFunc_userData->a;
//    const double b = gFunc_userData->b;
    double result;
//    if (fto_isClose_default(x, 0) || fto_isClose_default(y, 0))
//        result = 0;
//    else if (fto_isClose_default(x, a))
//        result = 2*a*a;
//    else if (fto_isClose_default(x, b))
//        result = 2*b*b;
//    else
//    {
//        fto_assert_fail("Invalid point provided: (%g, %g)", x, y);
//        assert(false);
//    }
    result = 2*x*x;
    printf("gFunc at (%g, %g): %g\n", x, y, result);
    return result;
}

extern void fto_test_fem_solver_solve(void **state)
{
    (void)state;
    struct Fto2DMesh *mesh = fto_malloc(sizeof *mesh);
    const double a = 2;
    const double b = 3;
    struct Fto2DPoint corner1 = fto_2dpoint_new(0, 0);
    struct Fto2DPoint corner2 = fto_2dpoint_new(a, b);
    struct Fto2DRectangle *rect = fto_2drectangle_new(&corner1, &corner2);
    AOK(fto_2dmesh_fromRectangle(rect, mesh));
    struct FtoBasisSet *basis_set = fto_malloc(sizeof *basis_set);
    AOK(fto_basis_piecewisePoly_construct(mesh, NULL, 1, basis_set));
    struct FtoGenericFunc *f_func = fto_malloc(sizeof *f_func);
    struct FtoGenericFunc *g_func = fto_malloc(sizeof *g_func);
    struct FtoPoly2D *f_poly = fto_malloc(sizeof *f_poly);
    AOK(fto_poly2d_init(f_poly, 1, 2*a + 2*b, 0, 0));
    AOK(fto_function_fromPoly2D(f_poly, f_func));
    struct GFuncUserData gFuncUserData = {.a = a, .b = b};
    AOK(fto_function_from2DPointer(gFuncImpl, &gFuncUserData, g_func));
    AOK(fto_fem_solve(mesh, basis_set, f_func, g_func));
}

