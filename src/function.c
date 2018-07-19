#include "femto/util.h"
#include "femto/function.h"
#include "femto/basis/poly.h"

extern enum FtoError fto_function_eval1d(struct FtoGenericFunc *func, double x_pt, double *result_out)
{
    double result;
    if (func->type == FTO_POLYNOMIAL_1D)
    {
        const struct FtoPoly1D *poly = func->state;
        result = fto_basis_poly1d_eval(poly, x_pt);
    }
    else if (func->type == FTO_FUNC_PTR_1D)
    {
        Fto1DFunctionPtr func_ptr = func->state;
        result = func_ptr(x_pt);
    }
    else
    {
        return fto_err_set(FTO_INVALID_ARG, "Invalid function type: %d", func->type);
    }
    *result_out = result;
    return FTO_OK;
}


extern enum FtoError fto_function_fromPoly1D(struct FtoPoly1D *poly, struct FtoGenericFunc *func_out)
{
    func_out->state = poly;
    func_out->type = FTO_POLYNOMIAL_1D;
    return FTO_OK;
}


extern enum FtoError fto_function_fromPoly2D(struct FtoPoly2D *poly, struct FtoGenericFunc *func_out)
{
    func_out->state = poly;
    func_out->type = FTO_POLYNOMIAL_2D;
    return FTO_OK;
}


extern enum FtoError fto_function_eval2d(struct FtoGenericFunc *func, double x_pt, double y_pt, double *result_out)
{
    double result;
    if (func->type == FTO_FUNC_PTR_2D)
    {
        Fto2DFunctionPtr func_ptr = func->state;
        result = func_ptr(x_pt, y_pt);
    }
    else if (func->type == FTO_POLYNOMIAL_2D)
    {
        result = fto_basis_poly2d_eval(func->state, x_pt, y_pt);
    }
    else
    {
        return fto_err_set(FTO_INVALID_ARG, "Invalid function type: %d", func->type);
    }
    *result_out = result;
    return FTO_OK;
}


extern enum FtoError fto_function_dot(struct FtoVectorFunc *func1, struct FtoVectorFunc *func2)
{
    if (func1->order != func2->order)
    {
        return fto_err_set(
                FTO_INVALID_ARG,
                "Can't dot product vector functions of different order: (%d, %d)",
                func1->order, func2->order);
    }
    return FTO_OK;
}


extern enum FtoError fto_function_mult(const struct FtoGenericFunc *func1, const struct FtoGenericFunc *func2, struct FtoGenericFunc *func_out)
{
    enum FtoError ret;
    if (func1->type == FTO_POLYNOMIAL_2D && func2->type == FTO_POLYNOMIAL_2D)
    {
        struct FtoPoly2D *poly_result = fto_malloc(sizeof *poly_result);
        if ((ret = fto_basis_poly2d_mult(func1->state, func2->state, poly_result)) != FTO_OK) return ret;
        if ((ret = fto_function_fromPoly2D(poly_result, func_out)) != FTO_OK) return ret;
    }
    else
    {
        return fto_err_set(FTO_INVALID_ARG, "Can't multiply specified function types");
    }
    return FTO_OK;
}
