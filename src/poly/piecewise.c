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
//            // basis functions currently expect coords xformed to regular triangle (no longer)
//            double xi, eta;
//            fto_2dtriangle_xformCoordToRegularTriangle(chunk->triangle, x, y, &xi, &eta);
            return fto_poly2d_eval(chunk->poly, x, y);
        }
    }
    return 0;
}


extern enum FtoError fto_poly_piecewise2d_chunkFromTriangle(
        const struct Fto2DTriangle *triangle,
        int node_offset,
        struct FtoPolyPiecewise2DTriangle *chunk_out)
{
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
    struct FtoPoly2D *new_poly = fto_malloc(sizeof *new_poly);
    struct FtoPoly2D *xold = fto_malloc(sizeof *xold);
    struct FtoPoly2D *yold = fto_malloc(sizeof *yold);
    const double area = fto_2dtriangle_area(triangle);
    // xi = (y3 - y1)/(2*A_k) x - (x3 - x1)/(2*A_k) y + ((x3 - x1)*y1 - ((y3 - y1)*x1)/(2*A_k)
    if ((ret = fto_poly2d_init(
            xold,
            3,
            .5 * (triangle->point3.y - triangle->point1.y) / area, 1, 0,
            .5 * (triangle->point1.x - triangle->point3.x) / area, 0, 1,
            .5 * ((triangle->point3.x - triangle->point1.x) * triangle->point1.y - (triangle->point3.y - triangle->point1.y) * triangle->point1.x) / area, 0, 0
            )) != FTO_OK)
        return ret;
    // eta = (y1 - y2)/(2*A_k) x + (x2 - x1)/(2*A_k) y + ((y2 - y1)*x1 - (x2 - x1)*y1)/(2*A_k)
    if ((ret = fto_poly2d_init(
            yold,
            3,
            .5 * (triangle->point1.y - triangle->point2.y) / area, 1, 0,
            .5 * (triangle->point2.x - triangle->point1.x) / area, 0, 1,
            .5 * ((triangle->point2.y - triangle->point1.y) * triangle->point1.x - (triangle->point2.x - triangle->point1.x) * triangle->point1.y) / area, 0, 0
            )) != FTO_OK)
        return ret;
    if ((ret = fto_poly2d_substitute(
            poly,
            xold,
            yold,
            new_poly)) != FTO_OK)
        return ret;
    fto_poly2d_simplify(new_poly);
    printf("Old poly: ");
    fto_poly2d_print(poly);
    printf("New poly: ");
    fto_poly2d_print(new_poly);
    *chunk_out = (struct FtoPolyPiecewise2DTriangle){
        .triangle = triangle,
        .poly = new_poly
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
    printf("\n");
    return FTO_OK;
}
