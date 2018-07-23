#include "femto/util.h"
#include "femto/poly/piecewise.h"

#include "femto/geom/shapes.h"


extern double fto_poly_piecewise_eval2D(const struct FtoPolyPiecewise2D *poly, double x, double y)
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


extern enum FtoError fto_poly_piecewise_chunkFromTriangle(
        const struct Fto2DTriangle *triangle,
        int node_offset,
        struct FtoPolyPiecewise2DTriangle *chunk_out)
{
    // note: for now this creates a polynomial that evaluates on the regular triangle (0,0),(0,1),(1,0) and expects
    // the caller to xform the coordinates appropriately
    struct FtoPoly2D *poly = fto_malloc(sizeof *poly);
    if (node_offset == 0)
    {
        // 1 - xi - eta
        fto_poly2d_init(poly, 3, 1.0, 0, 0, -1.0, 1, 0, -1.0, 0, 1);
    }
    else if (node_offset == 1)
    {
        // eta
        fto_poly2d_init(poly, 1, 1.0, 1, 0);
    }
    else if (node_offset == 2)
    {
        // xi
        fto_poly2d_init(poly, 1, 1.0, 0, 1);
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
