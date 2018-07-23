#include "femto/util.h"
#include "femto/function.h"
#include "femto/poly/poly.h"

extern enum FtoError fto_function_eval1d(const struct FtoGenericFunc *func, double x_pt, double *result_out)
{
    double result;
    if (func->type == FTO_POLYNOMIAL_1D)
    {
        const struct FtoPoly1D *poly = func->state;
        result = fto_poly1d_eval(poly, x_pt);
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


extern enum FtoError fto_function_fromPoly1D(const struct FtoPoly1D *poly, struct FtoGenericFunc *func_out)
{
    func_out->state = poly;
    func_out->type = FTO_POLYNOMIAL_1D;
    return FTO_OK;
}


extern enum FtoError fto_function_fromPoly2D(const struct FtoPoly2D *poly, struct FtoGenericFunc *func_out)
{
    func_out->state = poly;
    func_out->type = FTO_POLYNOMIAL_2D;
    return FTO_OK;
}


extern enum FtoError fto_function_eval2d(const struct FtoGenericFunc *func, double x_pt, double y_pt, double *result_out)
{
    enum FtoError ret;
    double result;
    if (func->type == FTO_FUNC_PTR_2D)
    {
        Fto2DFunctionPtr func_ptr = func->state;
        result = func_ptr(x_pt, y_pt);
    }
    else if (func->type == FTO_POLYNOMIAL_2D)
    {
        result = fto_poly2d_eval(func->state, x_pt, y_pt);
    }
    else if (func->type == FTO_POLY_PIECEWISE_2D)
    {
        result = fto_poly_piecewise_eval2D(func->state, x_pt, y_pt);
    }
    else if (func->type == FTO_FUNC_COMBINE_MULTIPLY_2D)
    {
        const struct FtoCombinedFuncData *data = func->state;
        double val1, val2;
        if ((ret = fto_function_eval2d(data->func1, x_pt, y_pt, &val1)) != FTO_OK) return ret;
        if ((ret = fto_function_eval2d(data->func2, x_pt, y_pt, &val2)) != FTO_OK) return ret;
        result = val1 * val2;
    }
    else
    {
        return fto_err_set(FTO_INVALID_ARG, "Invalid function type: %d", func->type);
    }
    *result_out = result;
    return FTO_OK;
}


//extern enum FtoError fto_function_dot(struct FtoVectorFunc *func1, struct FtoVectorFunc *func2)
//{
//    if (func1->order != func2->order)
//    {
//        return fto_err_set(
//                FTO_INVALID_ARG,
//                "Can't dot product vector functions of different order: (%d, %d)",
//                func1->order, func2->order);
//    }
//    return FTO_OK;
//}


extern enum FtoError fto_function_mult(const struct FtoGenericFunc *func1, const struct FtoGenericFunc *func2, struct FtoGenericFunc *func_out)
{
    enum FtoError ret;
    if (func1->type == FTO_POLYNOMIAL_2D && func2->type == FTO_POLYNOMIAL_2D)
    {
        struct FtoPoly2D *poly_result = fto_malloc(sizeof *poly_result);
        if ((ret = fto_poly2d_mult(func1->state, func2->state, poly_result)) != FTO_OK) return ret;
        if ((ret = fto_function_fromPoly2D(poly_result, func_out)) != FTO_OK) return ret;
    }
    else
    {
        int ndims = fto_function_ndims(func1);
        if (ndims != fto_function_ndims(func2))
        {
            return fto_err_set(
                    FTO_INVALID_ARG,
                    "Cannot multiply functions with different dimensions: %d, %d",
                    ndims, fto_function_ndims(func2));
        }
        if (ndims == 2)
        {
            struct FtoCombinedFuncData *combined = fto_malloc(sizeof *combined);
            *combined = (struct FtoCombinedFuncData){
                .func1 = func1,
                .func2 = func2
            };
            *func_out = (struct FtoGenericFunc){
                .type = FTO_FUNC_COMBINE_MULTIPLY_2D,
                .state = combined
            };
        }
    }
    return FTO_OK;
}


extern enum FtoError fto_function_fromPolyPiecewise2D(struct FtoPolyPiecewise2D *poly, struct FtoGenericFunc *func_out)
{
    *func_out = (struct FtoGenericFunc){
        .state = poly,
        .type = FTO_POLY_PIECEWISE_2D
    };
    return FTO_OK;
}


extern int fto_function_ndims(const struct FtoGenericFunc *func)
{
    switch (func->type)
    {
        case FTO_POLYNOMIAL_1D:
        case FTO_FUNC_PTR_1D:
            return 1;
        case FTO_POLYNOMIAL_2D:
        case FTO_POLY_PIECEWISE_2D:
        case FTO_FUNC_PTR_2D:
        case FTO_FUNC_COMBINE_MULTIPLY_2D:
            return 2;
    }
    return -1;
}
