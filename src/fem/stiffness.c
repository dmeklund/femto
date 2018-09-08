#include "femto/util.h"
#include "femto/fem/stiffness.h"

#include "femto/basis/set.h"
#include "femto/calc/diff.h"
#include "femto/quad/gauss.h"
#include "femto/mesh/mesh.h"

#include <stdio.h>


static enum FtoError calculateStiffnessElement(
        const struct Fto2DMesh *mesh,
        const struct FtoBasisSet *basis_set,
        int triangle_ind,
        int offset1,
        int offset2,
        double *val_out)
{
    enum FtoError ret;
    int node_i = mesh->triangles[mesh->num_nodesPerTriangle*triangle_ind + offset1];
    int node_j = mesh->triangles[mesh->num_nodesPerTriangle*triangle_ind + offset2];
    struct FtoGenericFunc basis1, basis2, *func_mult;
    // calculate the H^1 scalar product: integral over triangle of vw + v'w', where v and w are the basis functions
    if ((ret = fto_basis_set_getBasisFunctionForNode(basis_set, node_i, &basis1)) != FTO_OK) return ret;
    if ((ret = fto_basis_set_getBasisFunctionForNode(basis_set, node_j, &basis2)) != FTO_OK) return ret;
    if ((ret = fto_function_mult(&basis1, &basis2, &func_mult)) != FTO_OK) return ret;
    struct FtoVectorFunc grad1, grad2;
    if ((ret = fto_calc_grad(&basis1, &grad1)) != FTO_OK) return ret;
    if ((ret = fto_calc_grad(&basis2, &grad2)) != FTO_OK) return ret;
    struct FtoGenericFunc dot_prod;
    if ((ret = fto_function_dot(&grad1, &grad2, &dot_prod)) != FTO_OK) return ret;
    struct FtoGenericFunc *integrand = &dot_prod;
//    if ((ret = fto_function_add(&dot_prod, func_mult, &integrand)) != FTO_OK) return ret;
    double result;
    struct Fto2DTriangle triangle;
    if ((ret = fto_2dmesh_toTriangle(mesh, triangle_ind, &triangle)) != FTO_OK) return ret;
    if ((ret = fto_gauss_integrate2d_triangle(integrand, &triangle, 3, &result)) != FTO_OK) return ret;
    *val_out = result;
    return FTO_OK;
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
            double a_jk;
            if ((ret = calculateStiffnessElement(mesh, basis_set, triangle_ind, row, col, &a_jk)) != FTO_OK) return ret;
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


extern enum FtoError fto_stiffness_bVector(
        const struct Fto2DMesh *mesh,
        const struct FtoBasisSet *basis_set,
        const struct FtoGenericFunc *f_func,
        const struct FtoGenericFunc *g_func,
        struct FtoMatrix *bColVector_out)
{
    enum FtoError ret;
    if ((ret = fto_mat_init(mesh->num_nodes, 1, bColVector_out)) != FTO_OK) return ret;
    for (int triangle_ind = 0; triangle_ind < mesh->num_triangles; ++triangle_ind)
    {
        struct Fto2DTriangle triangle;
        fto_2dmesh_toTriangle(mesh, triangle_ind, &triangle);
        for (int node_offset = 0; node_offset < mesh->num_nodesPerTriangle; ++node_offset)
        {
            int node_ind = mesh->triangles[mesh->num_nodesPerTriangle*triangle_ind + node_offset];
            struct FtoGenericFunc basis;
            if ((ret = fto_basis_set_getBasisFunctionForNode(basis_set, node_ind, &basis)) != FTO_OK) return ret;
            struct FtoGenericFunc *integrand1;
            if ((ret = fto_function_mult(&basis, f_func, &integrand1)) != FTO_OK) return ret;
            double result;
            if ((ret = fto_gauss_integrate2d_triangle(integrand1, &triangle, 3, &result)) != FTO_OK) return ret;
            if ((ret = fto_mat_plusEquals(bColVector_out, node_ind, 0, result)) != FTO_OK) return ret;
        }
    }
    for (int edge_offset = 0; edge_offset < mesh->num_edgeNodes; ++edge_offset)
    {
        int node_ind1 = mesh->edgePath[edge_offset];
        int node_ind2;
        if (edge_offset == mesh->num_edgeNodes - 1)
        {
            node_ind2 = mesh->edgePath[0];
        }
        else
        {
            node_ind2 = mesh->edgePath[edge_offset+1];
        }
        double *node1 = &mesh->nodes[2*node_ind1];
        double *node2 = &mesh->nodes[2*node_ind2];
        struct Fto2DLine line = fto_2dline_fromDoubles(node1[0], node1[1], node2[0], node2[1]);
        struct FtoGenericFunc basis;
        struct FtoGenericFunc *integrand2;
        double result;

        if ((ret = fto_basis_set_getBasisFunctionForNode(basis_set, node_ind1, &basis)) != FTO_OK) return ret;
        if ((ret = fto_function_mult(&basis, g_func, &integrand2)) != FTO_OK) return ret;
        if ((ret = fto_gauss_integrate2d_line(integrand2, &line, 3, &result)) != FTO_OK) return ret;
        if ((ret = fto_mat_plusEquals(bColVector_out, node_ind1, 0, result)) != FTO_OK) return ret;

        if ((ret = fto_basis_set_getBasisFunctionForNode(basis_set, node_ind2, &basis)) != FTO_OK) return ret;
        if ((ret = fto_function_mult(&basis, g_func, &integrand2)) != FTO_OK) return ret;
        if ((ret = fto_gauss_integrate2d_line(integrand2, &line, 3, &result)) != FTO_OK) return ret;
        if ((ret = fto_mat_plusEquals(bColVector_out, node_ind2, 0, result)) != FTO_OK) return ret;

    }
    return FTO_OK;
}
