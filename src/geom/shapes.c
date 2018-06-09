#include "femto/util.h"
#include "femto/geom/shapes.h"

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
