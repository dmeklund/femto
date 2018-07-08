#include "femto/basis/poly.h"
#include "femto/util.h"
#include "test_gauss.h"
#include "../fto_test.h"
#include "femto/quad/gauss.h"


extern void fto_test_quad_gauss_integrate1d(void **state)
{
    (void)state;
    struct FtoPoly1D *poly = fto_malloc(sizeof *poly);
    AOK(fto_basis_poly1d_init(poly, 1, 0, 1));
    struct FtoGeneric1DFunc *func = fto_malloc(sizeof *func);
    AOK(fto_function_fromPoly1D(poly, func));
    double a = 0;
    double b = 1;
    int num_nodes = 2;
    double result;
    AOK(fto_gauss_integrate1d(func, a, b, num_nodes, &result));
}

extern enum FtoError fto_test_quad_gauss_addAll(struct FtoArray *tests)
{
    return FTO_OK;
}