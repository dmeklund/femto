#include "femto/util.h"
#include "fto_test.h"
#include "fem/test_stiffness.h"

#include "femto/fem/stiffness.h"
#include "femto/mesh/mesh.h"
#include "femto/basis/piecewise_poly.h"


extern enum FtoError fto_test_fem_stiffness_addAll(struct FtoArray *tests)
{
    AOK(FTO_TEST_APPEND(tests, fto_test_fem_stiffness_calculateStiffnessMatrix2D));
    AOK(FTO_TEST_APPEND(tests, fto_test_fem_stiffness_elementMatrix2D));
    return FTO_OK;
}


extern void fto_test_fem_stiffness_calculateStiffnessMatrix2D(void **state)
{
    (void)state;
    struct Fto2DMesh *mesh = fto_malloc(sizeof *mesh);
//    struct Fto2DRectangle *rect = fto_2drectangle_new(fto_2dpoint_new(0, 0), fto_2dpoint_new(1, 1));
//    AOK(fto_2dmesh_fromRectangle(rect, mesh));
    const double height = 1;
    struct Fto2DTriangle *triangle = fto_2dtriangle_new(
            fto_2dpoint_new(0, 0), fto_2dpoint_new(height, 0), fto_2dpoint_new(0, height));
    AOK(fto_2dmesh_fromTriangle(triangle, mesh));
    struct FtoBasisSet *basis_set = fto_malloc(sizeof *basis_set);
    AOK(fto_basis_piecewisePoly_construct(mesh, NULL, 1, basis_set));
    struct FtoMatrix *stiffness = fto_malloc(sizeof *stiffness);
    AOK(fto_stiffness_calculateStiffnessMatrix2D(mesh, basis_set, stiffness));
    double val;
    AOK(fto_mat_getval(stiffness, 0, 0, &val));
    FTO_ASSERT(fto_isClose_default(val, 1.0));
    AOK(fto_mat_getval(stiffness, 0, 1, &val));
    FTO_ASSERT(fto_isClose_default(val, -.5));
    AOK(fto_mat_getval(stiffness, 0, 2, &val));
    FTO_ASSERT(fto_isClose_default(val, -.5));
    AOK(fto_mat_getval(stiffness, 1, 1, &val));
    FTO_ASSERT(fto_isClose_default(val, .5));
    AOK(fto_mat_getval(stiffness, 1, 2, &val));
    FTO_ASSERT(fto_isClose_default(val, 0.0));
    AOK(fto_mat_getval(stiffness, 2, 2, &val));
    FTO_ASSERT(fto_isClose_default(val, 0.5));
}


extern void fto_test_fem_stiffness_elementMatrix2D(void **state)
{
    (void)state;
    struct Fto2DMesh *mesh = fto_malloc(sizeof *mesh);
    const double height = 8;
    struct Fto2DTriangle *triangle = fto_2dtriangle_new(
            fto_2dpoint_new(0, 0), fto_2dpoint_new(height, 0), fto_2dpoint_new(0, height));
    AOK(fto_2dmesh_fromTriangle(triangle, mesh));
    struct FtoBasisSet *basis_set = fto_malloc(sizeof *basis_set);
    AOK(fto_basis_piecewisePoly_construct(mesh, NULL, 1, basis_set));
    struct FtoMatrix *element_stiffness = fto_malloc(sizeof *element_stiffness);
    AOK(fto_stiffness_elementMatrix2D(mesh, basis_set, 0, element_stiffness));
//    AOK(fto_basis_set_print(basis_set));
    double val;
    AOK(fto_mat_getval(element_stiffness, 0, 0, &val));
    FTO_ASSERT(fto_isClose_default(val, 1.0));
    AOK(fto_mat_getval(element_stiffness, 0, 1, &val));
    FTO_ASSERT(fto_isClose_default(val, -.5));
    AOK(fto_mat_getval(element_stiffness, 0, 2, &val));
    FTO_ASSERT(fto_isClose_default(val, -.5));
    AOK(fto_mat_getval(element_stiffness, 1, 1, &val));
    FTO_ASSERT(fto_isClose_default(val, .5));
    AOK(fto_mat_getval(element_stiffness, 1, 2, &val));
    FTO_ASSERT(fto_isClose_default(val, 0.0));
    AOK(fto_mat_getval(element_stiffness, 2, 2, &val));
    FTO_ASSERT(fto_isClose_default(val, 0.5));
}
