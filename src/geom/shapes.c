#include "femto/util.h"
#include "femto/geom/shapes.h"

#include <math.h>
#include <stdio.h>


static double calc_2dtriangle_signedArea(
        const struct Fto2DPoint *pt1,
        const struct Fto2DPoint *pt2,
        const struct Fto2DPoint *pt3)
{
    const double x1 = pt1->x;
    const double x2 = pt2->x;
    const double x3 = pt3->x;
    const double y1 = pt1->y;
    const double y2 = pt2->y;
    const double y3 = pt3->y;
    return .5 * (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2));
}


extern struct Fto2DTriangle* fto_2dtriangle_new(
        struct Fto2DPoint pt1,
        struct Fto2DPoint pt2,
        struct Fto2DPoint pt3)
{
    struct Fto2DTriangle *triangle = fto_malloc(sizeof *triangle);
    if (calc_2dtriangle_signedArea(&pt1, &pt2, &pt3) < 0)
    {
        // make triangle points always counter-clockwise
        struct Fto2DPoint tmp = pt1;
        pt1 = pt2;
        pt2 = tmp;
    }
    *triangle = (struct Fto2DTriangle){
        .point1 = pt1,
        .point2 = pt2,
        .point3 = pt3
    };
    return triangle;
}


extern double fto_2dtriangle_signedArea(const struct Fto2DTriangle *triangle)
{
    return calc_2dtriangle_signedArea(&triangle->point1, &triangle->point2, &triangle->point3);
}

extern double fto_2dtriangle_area(const struct Fto2DTriangle *triangle)
{
    return fabs(fto_2dtriangle_signedArea(triangle));
}


extern bool fto_2dtriangle_contains(const struct Fto2DTriangle *triangle, double x, double y)
{
    // using barycentric coords
    const double area = calc_2dtriangle_signedArea(&triangle->point1, &triangle->point2, &triangle->point3);
    const double p1x = triangle->point1.x;
    const double p1y = triangle->point1.y;
    const double p2x = triangle->point2.x;
    const double p2y = triangle->point2.y;
    const double p3x = triangle->point3.x;
    const double p3y = triangle->point3.y;
    const double lambda1 = .5/area * (p1y*p3x - p1x*p3y + (p3y-p1y)*x + (p1x - p3x)*y);
    const double lambda2 = .5/area * (p1x*p2y - p1y*p2x + (p1y-p2y)*x + (p2x - p1x)*y);
    const double lambda3 = 1 - lambda1 - lambda2;
    return (fto_nearlyNonNegative(lambda1)
            && fto_nearlyNonNegative(lambda2)
            && fto_nearlyNonNegative(lambda3));
}


extern void fto_2dtriangle_xformCoordToRegularTriangle(
        const struct Fto2DTriangle *triangle,
        double x, double y,
        double *xi_out, double *eta_out)
{
    double area = fto_2dtriangle_area(triangle);
    *xi_out = .5/area * (
            (triangle->point3.y - triangle->point1.y) * (x - triangle->point1.x)
            - (triangle->point3.x - triangle->point1.x) * (y - triangle->point1.y));
    *eta_out = .5/area * (
             -(triangle->point2.y - triangle->point1.y) * (x - triangle->point1.x)
             + (triangle->point2.x - triangle->point1.x) * (y - triangle->point1.y));
}


extern struct Fto2DRectangle* fto_2drectangle_new(const struct Fto2DPoint *corner1, const struct Fto2DPoint *corner2)
{
    struct Fto2DRectangle *rect = fto_malloc(sizeof *rect);
    *rect = (struct Fto2DRectangle){
        .corner1 = corner1,
        .corner2 = corner2
    };
    return rect;
}


extern enum FtoError fto_2dtriangle_print(const struct Fto2DTriangle *triangle)
{
    enum FtoError ret;
    printf("Triangle(");
    if ((ret = fto_2dpoint_print(triangle->point1)) != FTO_OK) return ret;
    printf(", ");
    if ((ret = fto_2dpoint_print(triangle->point2)) != FTO_OK) return ret;
    printf(", ");
    if ((ret = fto_2dpoint_print(triangle->point3)) != FTO_OK) return ret;
    return FTO_OK;
}
