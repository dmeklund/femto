#include "femto/util.h"
#include "femto/poly/piecewise.h"

#include "femto/geom/shapes.h"

#include <stdio.h>


extern double fto_poly_piecewise2d_eval(const struct FtoPolyPiecewise2D *poly, double x, double y)
{
    for (int chunk_ind = 0; chunk_ind < poly->chunks->length; ++chunk_ind)
    {
        const struct FtoPolyPiecewise2DTriangle *chunk = poly->chunks->values[chunk_ind];
        if (fto_2dtriangle_contains(chunk->triangle, x, y))
        {
            // basis functions currently expect coords xformed to regular triangle
            double xi, eta;
            fto_2dtriangle_xformCoordToRegularTriangle(chunk->triangle, x, y, &xi, &eta);
            return fto_poly2d_eval(chunk->poly, xi, eta);
        }
    }
    return 0;
}


extern enum FtoError fto_poly_piecewise2d_chunkFromTriangle(
        const struct Fto2DTriangle *triangle,
        int node_offset,
        struct FtoPolyPiecewise2DTriangle *chunk_out)
{
    // note: for now this creates a polynomial that evaluates on the regular triangle (0,0),(0,1),(1,0) and expects
    // the caller to xform the coordinates appropriately
    enum FtoError ret;
    struct FtoPoly2D *poly = fto_malloc(sizeof *poly);
    if (node_offset == 0)
    {
        // 1 - xi - eta
        if ((ret = fto_poly2d_init(poly, 3, 1.0, 0, 0, -1.0, 1, 0, -1.0, 0, 1)) != FTO_OK) return ret;
    }
    else if (node_offset == 1)
    {
        // eta
        if ((ret = fto_poly2d_init(poly, 1, 1.0, 1, 0)) != FTO_OK) return ret;
    }
    else if (node_offset == 2)
    {
        // xi
        if ((ret = fto_poly2d_init(poly, 1, 1.0, 0, 1)) != FTO_OK) return ret;
    }
    else
    {
        return fto_err_set(FTO_INVALID_ARG, "Invalid node offset: %d", node_offset);
    }
    *chunk_out = (struct FtoPolyPiecewise2DTriangle){
        .triangle = triangle,
        .poly = poly
    };
    return FTO_OK;
}


extern enum FtoError fto_poly_piecewise2d_diff(
        const struct FtoPolyPiecewise2D *poly,
        int axis,
        struct FtoPolyPiecewise2D *diff_out)
{
    enum FtoError ret;
    diff_out->chunks = fto_array_new_capacity(poly->chunks->length);
    for (int chunk_ind = 0; chunk_ind < poly->chunks->length; ++chunk_ind)
    {
        const struct FtoPolyPiecewise2DTriangle *chunk = poly->chunks->values[chunk_ind];
        struct FtoPoly2D *chunk_diff = fto_malloc(sizeof *chunk_diff);
        if ((ret = fto_poly2d_diff(chunk->poly, axis, chunk_diff)) != FTO_OK) return ret;
        struct FtoPolyPiecewise2DTriangle *new_chunk = fto_malloc(sizeof *new_chunk);
        new_chunk->triangle = chunk->triangle;
        new_chunk->poly = chunk_diff;
        if ((ret = fto_array_append(diff_out->chunks, new_chunk)) != FTO_OK) return ret;
    }
    return FTO_OK;
}


extern enum FtoError fto_poly_piecewise2d_print(const struct FtoPolyPiecewise2D *poly)
{
    enum FtoError ret;
    for (int chunk_ind = 0; chunk_ind < poly->chunks->length; ++chunk_ind)
    {
        const struct FtoPolyPiecewise2DTriangle *chunk = poly->chunks->values[chunk_ind];
        if ((ret = fto_poly2d_print(chunk->poly)) != FTO_OK) return ret;
        printf(" in ");
        if ((ret = fto_2dtriangle_print(chunk->triangle)) != FTO_OK) return ret;
        if (chunk_ind < poly->chunks->length-1)
            printf(", ");
    }
    return FTO_OK;
}
