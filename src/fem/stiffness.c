#include "femto/util.h"
#include "femto/fem/stiffness.h"

#include "femto/basis/set.h"
#include "femto/quad/gauss.h"
#include "femto/mesh/mesh.h"

static double calculateStiffnessElement(
        const struct Fto2DMesh *mesh,
        const struct FtoBasisSet *basis,
        int triangle_ind,
        int offset1,
        int offset2)
{
    int node_i = mesh->triangles[mesh->num_nodesPerTriangle*triangle_ind + offset1];
    int node_j = mesh->triangles[mesh->num_nodesPerTriangle*triangle_ind + offset2];
    struct FtoGenericFunc func1, func2, func_mult;
    fto_basis_set_getBasisFunctionForNode(basis, node_i, &func1);
    fto_basis_set_getBasisFunctionForNode(basis, node_j, &func2);
    fto_function_mult(&func1, &func2, &func_mult);
    double result;
    struct Fto2DTriangle triangle;
    fto_2dmesh_toTriangle(mesh, triangle_ind, &triangle);
    fto_gauss_integrate2d_triangle(&func_mult, &triangle, 3, &result);
    return result;
}


extern enum FtoError fto_stiffness_elementMatrix2D(
        const struct Fto2DMesh *mesh,
        const struct FtoBasisSet *basis,
        int triangle_ind,
        struct FtoMatrix *matrix_out)
{
    enum FtoError ret;
    if ((ret = fto_mat_init(mesh->num_nodesPerTriangle, mesh->num_nodesPerTriangle, matrix_out)) != FTO_OK)
        return ret;
    for (int row = 0; row < mesh->num_nodesPerTriangle; ++row)
    {
        for (int col = 0; col < mesh->num_nodesPerTriangle; ++col)
        {
            const double a_jk = calculateStiffnessElement(mesh, basis, triangle_ind, row, col);
            // todo: represent symmetric matrices
            if ((ret = fto_mat_setval(matrix_out, row, col, a_jk)) != FTO_OK) return ret;
            if ((ret = fto_mat_setval(matrix_out, col, row, a_jk)) != FTO_OK) return ret;
        }
    }
    return FTO_OK;
}
