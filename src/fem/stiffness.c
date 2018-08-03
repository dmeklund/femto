#include "femto/util.h"
#include "femto/fem/stiffness.h"

#include "femto/basis/set.h"
#include "femto/calc/diff.h"
#include "femto/quad/gauss.h"
#include "femto/mesh/mesh.h"

static double calculateStiffnessElement(
        const struct Fto2DMesh *mesh,
        const struct FtoBasisSet *basis_set,
        int triangle_ind,
        int offset1,
        int offset2)
{
    int node_i = mesh->triangles[mesh->num_nodesPerTriangle*triangle_ind + offset1];
    int node_j = mesh->triangles[mesh->num_nodesPerTriangle*triangle_ind + offset2];
    struct FtoGenericFunc basis1, basis2, *func_mult;
    // calculate the H^1 scalar product: integral over triangle of vw + v'w', where v and w are the basis functions
    fto_basis_set_getBasisFunctionForNode(basis_set, node_i, &basis1);
    fto_basis_set_getBasisFunctionForNode(basis_set, node_j, &basis2);
    fto_function_mult(&basis1, &basis2, &func_mult);
    struct FtoVectorFunc grad1, grad2;
    fto_calc_grad(&basis1, &grad1);
    fto_calc_grad(&basis2, &grad2);
    struct FtoGenericFunc dot_prod;
    fto_function_dot(&grad2, &grad2, &dot_prod);
    struct FtoGenericFunc *integrand;
    fto_function_add(&dot_prod, func_mult, &integrand);
    double result;
    struct Fto2DTriangle triangle;
    fto_2dmesh_toTriangle(mesh, triangle_ind, &triangle);
    fto_gauss_integrate2d_triangle(integrand, &triangle, 3, &result);
    return result;
}


extern enum FtoError fto_stiffness_elementMatrix2D(
        const struct Fto2DMesh *mesh,
        const struct FtoBasisSet *basis_set,
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
            const double a_jk = calculateStiffnessElement(mesh, basis_set, triangle_ind, row, col);
            // todo: represent symmetric matrices
            if ((ret = fto_mat_setval(matrix_out, row, col, a_jk)) != FTO_OK) return ret;
            if ((ret = fto_mat_setval(matrix_out, col, row, a_jk)) != FTO_OK) return ret;
        }
    }
    return FTO_OK;
}


extern enum FtoError fto_stiffness_calculateStiffnessMatrix2D(
        const struct Fto2DMesh *mesh,
        const struct FtoBasisSet *basis_set,
        struct FtoMatrix *matrix_out)
{
    enum FtoError ret;
    if ((ret = fto_mat_init(mesh->num_nodes, mesh->num_nodes, matrix_out)) != FTO_OK) return ret;
    // inefficient
    for (int triangle_ind = 0; triangle_ind < mesh->num_triangles; ++triangle_ind)
    {
        struct FtoMatrix element_mat;
        if ((ret = fto_stiffness_elementMatrix2D(mesh, basis_set, triangle_ind, &element_mat)) != FTO_OK) return ret;
        for (int row = 0; row < mesh->num_nodesPerTriangle; ++row)
        {
            for (int col = 0; col < mesh->num_nodesPerTriangle; ++col)
            {
                int node_i = mesh->triangles[mesh->num_nodesPerTriangle*triangle_ind + row];
                int node_j = mesh->triangles[mesh->num_nodesPerTriangle*triangle_ind + col];
                double val;
                if ((ret = fto_mat_getval(&element_mat, row, col, &val)) != FTO_OK) return ret;
                if ((ret = fto_mat_plusEquals(matrix_out, node_i, node_j, val)) != FTO_OK) return ret;
            }
        }
    }
    return FTO_OK;
}
