#include "femto/util.h"
#include "femto/fem/stiffness.h"

static double calculateStiffnessElement(const struct Fto2DMesh *mesh, int triangle_ind, int row, int col)
{
    int node_i = mesh->triangles[mesh->num_nodesPerTriangle*triangle_ind + row];
    int node_j = mesh->triangles[mesh->num_nodesPerTriangle*triangle_ind + col];
    return 0;
}


extern enum FtoError fto_stiffness_elementMatrix2D(
        const struct Fto2DMesh *mesh,
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
            const double a_jk = calculateStiffnessElement(mesh, triangle_ind, row, col);
            // todo: represent symmetric matrices
            if ((ret = fto_mat_setval(matrix_out, row, col, a_jk)) != FTO_OK) return ret;
            if ((ret = fto_mat_setval(matrix_out, col, row, a_jk)) != FTO_OK) return ret;
        }
    }
    return FTO_OK;
}
