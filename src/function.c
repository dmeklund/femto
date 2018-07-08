#include "femto/util.h"
#include "femto/function.h"
#include "femto/basis/poly.h"

extern enum FtoError fto_function_eval1d(struct FtoGeneric1DFunc *func, double x_pt, double *result_out)
{
    double result;
    if (func->type == FTO_POLYNOMIAL)
    {
        const struct FtoPoly1D *poly = func->state;
        result = fto_basis_poly1d_eval(poly, x_pt);
    }
    else
    {
        return fto_err_set(FTO_INVALID_ARG, "Unknown function type: %d", func->type);
    }
    *result_out = result;
    return FTO_OK;
}


extern enum FtoError fto_function_fromPoly1D(struct FtoPoly1D *poly, struct FtoGeneric1DFunc *func_out)
{
    func_out->state = poly;
    func_out->type = FTO_POLYNOMIAL;
    return FTO_OK;
}
