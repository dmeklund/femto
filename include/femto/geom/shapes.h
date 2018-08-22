#ifndef FTO_GEOM_SHAPES_H
#define FTO_GEOM_SHAPES_H

#include "femto/util.h"

#include "femto/geom/point.h"

struct Fto2DTriangle
{
    struct Fto2DPoint point1, point2, point3;
};

struct Fto2DRectangle
{
    const struct Fto2DPoint *corner1, *corner2;
};

extern struct Fto2DTriangle* fto_2dtriangle_new(
        struct Fto2DPoint pt1,
        struct Fto2DPoint pt2,
        struct Fto2DPoint pt3);
extern struct Fto2DRectangle* fto_2drectangle_new(const struct Fto2DPoint *corner1, const struct Fto2DPoint *corner2);

extern double fto_2dtriangle_area(const struct Fto2DTriangle *triangle);
extern bool fto_2dtriangle_contains(const struct Fto2DTriangle *triangle, double x, double y);
extern void fto_2dtriangle_xformCoordToRegularTriangle(
        const struct Fto2DTriangle *triangle,
        double x, double y,
        double *xi_out, double *eta_out);

extern enum FtoError fto_2dtriangle_print(const struct Fto2DTriangle *triangle);

#endif // FTO_GEOM_SHAPES_H
