#include "femto/util.h"
#include "femto/basis/piecewise_linear.h"

#include <math.h>
#include "femto/util/array.h"

extern double fto_basis_pwl2d_eval(const struct FtoPwl2DFunction *func, const struct Fto2DPoint *point)
{
    // not going to be optimal since we need to find which triangle the point lies in
    const struct Fto2DMesh *mesh = func->mesh;
    int relevant_triangle_ind = -1;
    for (int triangle_ind = 0; triangle_ind < mesh->num_triangles; ++triangle_ind)
    {
        const int *triangle = &mesh->triangles[3*triangle_ind];
        const double *pt0 = &mesh->nodes[2*triangle[0]];
        const double *pt1 = &mesh->nodes[2*triangle[1]];
        const double *pt2 = &mesh->nodes[2*triangle[2]];
        const double area = .5 * (-pt1[1]*pt2[0] + pt0[1]*(-pt1[0] + pt2[0]) + pt0[0]*(pt1[1] - pt2[1]) + pt1[0]*pt2[1]);
        const double s = 1/(2*area) * (pt0[1]*pt2[0] - pt0[0]*pt2[1] + (pt2[1] - pt0[1])*point->x + (pt0[0] - pt2[0])*point->y);
        const double t = 1/(2*area) * (pt0[0]*pt1[1] - pt0[1]*pt1[0] + (pt0[1] - pt1[1])*point->x + (pt1[0] - pt0[0])*point->y);
        if (s >= 0 && t >= 0 && 1-s-t >= 0)
        {
            relevant_triangle_ind = triangle_ind;
            break;
        }
    }
    if (relevant_triangle_ind == -1)
    {
        return 0;
    }
    return fto_basis_pwl2d_eval_tri(func, relevant_triangle_ind, point);
}


extern double fto_basis_pwl2d_eval_tri(const struct FtoPwl2DFunction *func, int triangle_ind, const struct Fto2DPoint *point)
{
    // there's probably a more efficient way to do this: find the intersection of the line connecting the node where
    // the basis function is 1 with the line connecting the other two points of the triangle, then find how far along
    // the given point is along that line and interpolate between 0 (at the line between the two zeros) and 1 (at the
    // relevant node).
    const int node_ind = func->node_index;
    const int *triangle = &func->mesh->triangles[3*triangle_ind];
    // first check if it lies within the triangle. since we're limiting evaluation to this triangle all values outside
    // the triangle are set to 0.
    if (!fto_intArray_contains(triangle, node_ind, 3))
    {
        return 0;
    }
    const double *nodes = func->mesh->nodes;
    double x1 = nodes[2*triangle[0]];
    double y1 = nodes[2*triangle[0]+1];
    double x2 = nodes[2*triangle[1]];
    double y2 = nodes[2*triangle[1]+1];
    double x3 = nodes[2*triangle[2]];
    double y3 = nodes[2*triangle[2]+1];
    const double x4 = point->x;
    const double y4 = point->y;
    if (triangle_ind == triangle[0])
    {
        FTO_DBL_SWAP(x1, x3);
        FTO_DBL_SWAP(y1, y3);
    }
    else if (triangle_ind == triangle[1])
    {
        FTO_DBL_SWAP(x2, x3);
        FTO_DBL_SWAP(y2, y3);
    }
    const double denom = (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4);
    const double intersect_x = ((x1*y2-y1*x2)*(x3-x4) - (x1-x2)*(x3*y4-y3*x4)) / denom;
    const double intersect_y = ((x1*y2-y1*x2)*(y3-y4) - (y1-y2)*(x3*y4-y3*x4)) / denom;
    const double result = sqrt(FTO_SQUARE(x4 - intersect_x) + FTO_SQUARE(y4-intersect_y)) / sqrt(FTO_SQUARE(x3-x4) + FTO_SQUARE(y3-y4));
    return result;
}
