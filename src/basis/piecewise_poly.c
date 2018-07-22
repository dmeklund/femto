#include <femto/util/array.h>
#include "femto/util.h"
#include "femto/basis/piecewise_poly.h"

#include "femto/basis/poly.h"

extern enum FtoError fto_basis_piecewisePoly_construct(
        const struct Fto2DMesh *mesh,
        const struct Fto2DNodeToTriangleSet *nodeToTriangles,
        int poly_order,
        struct FtoBasisSet *set_out)
{
    enum FtoError ret;
    if (poly_order != 1)
    {
        return fto_err_set(FTO_INVALID_ARG, "Unsupported polynomial order: %d", poly_order);
    }
    for (int node_ind = 0; node_ind < mesh->num_nodes; ++node_ind)
    {
        struct FtoValueArray *triangles;
        if ((ret = fto_2dmesh_getTrianglesForNode(nodeToTriangles, node_ind, &triangles)) != FTO_OK) return ret;

    }
    return FTO_OK;
}