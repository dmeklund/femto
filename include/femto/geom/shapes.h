#ifndef FTO_GEOM_SHAPES_H
#define FTO_GEOM_SHAPES_H

#include "femto/geom/point.h"

struct Fto2DTriangle
{
    struct Fto2DPoint *point1, *point2, *point3;
};

struct Fto2DRectangle
{
    struct Fto2DPoint *corner1, *corner2;
};

extern struct Fto2DTriangle* fto_2dtriangle_new(
        struct Fto2DPoint *pt1,
        struct Fto2DPoint *pt2,
        struct Fto2DPoint *pt3);

extern double fto_2dtriangle_area(const struct Fto2DTriangle *triangle);

#endif // FTO_GEOM_SHAPES_H
