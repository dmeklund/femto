#include "femto/util.h"
#include "fto_test.h"
#include "fem/test_stiffness.h"

#include "femto/fem/stiffness.h"
#include "femto/mesh/mesh.h"
#include "femto/basis/piecewise_poly.h"


extern enum FtoError fto_test_fem_stiffness_addAll(struct FtoArray *tests)
{
    AOK(FTO_TEST_APPEND(tests, fto_test_fem_stiffness_calculateStiffnessMatrix2D));
    return FTO_OK;
}


extern void fto_test_fem_stiffness_calculateStiffnessMatrix2D(void **state)
{
    struct Fto2DMesh *mesh = fto_malloc(sizeof *mesh);
    struct Fto2DRectangle *rect = fto_2drectangle_new(fto_2dpoint_new(0, 0), fto_2dpoint_new(1, 1));
    AOK(fto_2dmesh_fromRectangle(rect, mesh));
    struct FtoBasisSet *basis_set = fto_malloc(sizeof *basis_set);
    AOK(fto_basis_piecewisePoly_construct(mesh, NULL, 1, basis_set));
    struct FtoMatrix *stiffness = fto_malloc(sizeof *stiffness);
    AOK(fto_stiffness_calculateStiffnessMatrix2D(mesh, basis_set, stiffness));
    (void)state;
}

