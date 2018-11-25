#include "femto/util.h"

#include "femto/fem/stiffness.h"

extern enum FtoError fto_fem_solve(
        const struct Fto2DMesh *mesh,
        const struct FtoBasisSet *basis_set,
        const struct FtoGenericFunc *f_func,
        const struct FtoGenericFunc *g_func
)
{
    struct FtoMatrix *stiffness = fto_malloc(sizeof *stiffness);
    fto_stiffness_calculateStiffnessMatrix2D(mesh, basis_set, stiffness);
    struct FtoMatrix *bColVector = fto_malloc(sizeof *bColVector);
    fto_stiffness_bVector(mesh, basis_set, f_func, g_func, bColVector);
    return FTO_OK;
}