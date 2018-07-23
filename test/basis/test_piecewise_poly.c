#include "femto/util.h"
#include "basis/test_piecewise_poly.h"

#include "femto/basis/piecewise_poly.h"

extern enum FtoError fto_test_basis_piecewisePoly_addAll(struct FtoArray *tests)
{
    enum FtoError ret;
    if ((ret = FTO_TEST_APPEND(tests, fto_test_basis_piecewisePoly_construct)) != FTO_OK) return ret;
    return FTO_OK;
}


extern void fto_test_basis_piecewisePoly_construct(void **state)
{
    (void)state;
    struct Fto2DMesh *mesh = fto_malloc(sizeof *mesh);
    struct Fto2DPoint pt1 = {.x = 1.3, .y = 0.3};
    struct Fto2DPoint pt2 = {.x = 3.2, .y = -.4};
    struct Fto2DPoint pt3 = {.x = -.2, .y = 1.5};
    struct Fto2DTriangle *triangle = fto_2dtriangle_new(pt1, pt2, pt3);
    AOK(fto_2dmesh_fromTriangle(triangle, mesh));
    struct Fto2DNodeToTriangleSet *nodeToTriangles = fto_malloc(sizeof *nodeToTriangles);
    fto_2dmesh_constructNodeToTriangleSet(mesh, nodeToTriangles);
    int poly_order = 1;
    struct FtoBasisSet *set = fto_malloc(sizeof *set);
    AOK(fto_basis_piecewisePoly_construct(mesh, nodeToTriangles, poly_order, set));
    struct FtoGenericFunc *func = fto_malloc(sizeof *func);
    double val;
    const struct Fto2DPoint points[] = {pt1, pt2, pt3};
    for (int ind1 = 0; ind1 < 3; ++ind1)
    {
        AOK(fto_basis_set_getBasisFunctionForNode(set, ind1, func));
        for (int ind2 = 0; ind2 < 3; ++ind2)
        {
            AOK(fto_function_eval2d(func, points[ind2].x, points[ind2].y, &val));
            if (ind1 == ind2)
            {
                FTO_ASSERT(fto_isClose_default(val, 1));
            }
            else
            {
                FTO_ASSERT(fto_isClose_default(val, 0));
            }
        }
    }
}
