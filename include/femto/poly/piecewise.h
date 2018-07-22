#ifndef FTO_POLY_PIECEWISE_H
#define FTO_POLY_PIECEWISE_H

#include "femto/util.h"

#include "femto/poly/poly.h"
#include "femto/geom/shapes.h"
#include "femto/util/array.h"

// defines the polynomial for a given chunk of 2D space via a triangle
struct FtoPolyPiecewise2DTriangle
{
    const struct FtoPoly2D *poly;
    const struct Fto2DTriangle *triangle;
};

struct FtoPolyPiecewise2D
{
    struct FtoArray *chunks;
};

extern double fto_poly_piecewise_eval2D(struct FtoPolyPiecewise2D *poly, double x, double y);
extern enum FtoError fto_poly_piecewise_chunkFromTriangle(
        const struct Fto2DTriangle *triangle,
        int node_offset,
        struct FtoPolyPiecewise2DTriangle *chunk_out);


#endif /* FTO_POLY_PIECEWISE_H */
