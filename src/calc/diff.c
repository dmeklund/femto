#include "femto/util.h"
#include "femto/calc/diff.h"

#include "femto/function.h"


extern enum FtoError fto_basis_poly2d_grad(const struct FtoPoly2D *poly, struct FtoVectorFunc *grad_out)
{
    enum FtoError ret;
    grad_out->funcs = fto_malloc(2 * sizeof *grad_out->funcs);
    grad_out->order = 2;
    struct FtoPoly2D *dx = fto_malloc(sizeof *dx);
    struct FtoPoly2D *dy = fto_malloc(sizeof *dy);
    if ((ret = fto_poly2d_diff(poly, 0, dx)) != FTO_OK) return ret;
    if ((ret = fto_poly2d_diff(poly, 1, dy)) != FTO_OK) return ret;
    if ((ret = fto_function_fromPoly2D(dx, &grad_out->funcs[0])) != FTO_OK) return ret;
    if ((ret = fto_function_fromPoly2D(dy, &grad_out->funcs[1])) != FTO_OK) return ret;
    return FTO_OK;
}
