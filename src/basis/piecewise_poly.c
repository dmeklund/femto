#include "femto/util.h"
#include "femto/basis/piecewise_poly.h"

#include "femto/util/array.h"
#include "femto/poly/piecewise.h"
#include "femto/poly/poly.h"


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
    struct FtoArray *basis_functions = fto_array_new_capacity(mesh->num_nodes);
    for (int node_ind = 0; node_ind < mesh->num_nodes; ++node_ind)
    {
        struct FtoValueArray *triangles;
        if ((ret = fto_2dmesh_getTrianglesForNode(nodeToTriangles, node_ind, &triangles)) != FTO_OK) return ret;
        struct FtoArray *chunks = fto_array_new();
        for (int triangle_ind = 0; triangle_ind < triangles->length; ++triangle_ind)
        {
            struct Fto2DTriangle *triangle = fto_malloc(sizeof *triangle);
            const int *node_offsets = &mesh->triangles[mesh->num_nodesPerTriangle*triangle_ind];
            const int node_offset = fto_intArray_offset(node_offsets, node_ind, mesh->num_nodesPerTriangle);
            if ((ret = fto_assert_nonnegative(node_offset)) != FTO_OK) return ret;
            if ((ret = fto_2dmesh_toTriangle(mesh, triangle_ind, triangle)) != FTO_OK) return ret;
            struct FtoPolyPiecewise2DTriangle *chunk = fto_malloc(sizeof *chunk);
            if ((ret = fto_poly_piecewise_chunkFromTriangle(triangle, node_offset, chunk)) != FTO_OK) return ret;
            if ((ret = fto_array_append(chunks, chunk)) != FTO_OK) return ret;
        }
        struct FtoPolyPiecewise2D *poly = fto_malloc(sizeof *poly);
        poly->chunks = chunks;
        struct FtoGenericFunc *function = fto_malloc(sizeof *function);
        if ((ret = fto_function_fromPolyPiecewise2D(poly, function)) != FTO_OK) return ret;
        if ((ret = fto_array_append(basis_functions, function)) != FTO_OK) return ret;
    }
    *set_out = (struct FtoBasisSet){
        .basis_functions = basis_functions
    };
    return FTO_OK;
}
