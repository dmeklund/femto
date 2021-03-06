#ifndef FTO_QUAD_GAUSS_H
#define FTO_QUAD_GAUSS_H

#include "femto/util.h"

#include "femto/function.h"
#include "femto/geom/shapes.h"

extern enum FtoError fto_gauss_integrate1d(
        const struct FtoGenericFunc *func,
        double a,
        double b,
        int num_nodes,
        double *result_out);
extern enum FtoError fto_gauss_integrate2d_triangle(
        const struct FtoGenericFunc *func,
        const struct Fto2DTriangle *triangle,
        int num_nodes,
        double *result_out);
extern enum FtoError fto_gauss_integrate2d_line(
        const struct FtoGenericFunc *func,
        const struct Fto2DLine *line,
        int num_nodes,
        double *result_out);

#endif /* FTO_QUAD_GAUSS_H */
