#include "femto/util.h"
#include "femto/geom/shapes.h"

#include <math.h>

extern struct Fto2DTriangle* fto_2dtriangle_new(
        struct Fto2DPoint *pt1,
        struct Fto2DPoint *pt2,
        struct Fto2DPoint *pt3)
{
    struct Fto2DTriangle *triangle = fto_malloc(sizeof *triangle);
    *triangle = (struct Fto2DTriangle){
        .point1 = pt1,
        .point2 = pt2,
        .point3 = pt3
    };
    return triangle;
}


extern double fto_2dtriangle_area(const struct Fto2DTriangle *triangle)
{
    const double x1 = triangle->point1->x;
    const double x2 = triangle->point2->x;
    const double x3 = triangle->point3->x;
    const double y1 = triangle->point1->y;
    const double y2 = triangle->point2->y;
    const double y3 = triangle->point3->y;
    return .5 * fabs(x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2));
}
