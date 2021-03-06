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
        const struct Fto1DFunction *func_struct = func->state;
        result = func_struct->function(x_pt);
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
        const struct Fto2DFunction *func_struct = func->state;
        result = func_struct->function(x_pt, y_pt, func_struct->user_data);
    }
    else if (func->type == FTO_POLYNOMIAL_2D)
    {
        result = fto_poly2d_eval(func->state, x_pt, y_pt);
    }
    else if (func->type == FTO_POLY_PIECEWISE_2D)
    {
        result = fto_poly_piecewise2d_eval(func->state, x_pt, y_pt);
    }
    else if (func->type == FTO_FUNC_COMBINE_MULTIPLY_2D)
    {
        const struct FtoCombinedFuncData *data = func->state;
        double val1, val2;
        if ((ret = fto_function_eval2d(data->func1, x_pt, y_pt, &val1)) != FTO_OK) return ret;
        if ((ret = fto_function_eval2d(data->func2, x_pt, y_pt, &val2)) != FTO_OK) return ret;
        result = val1 * val2;
    }
    else if (func->type == FTO_FUNC_COMBINE_ADD_2D)
    {
        const struct FtoCombinedFuncData *data = func->state;
        double val1, val2;
        if ((ret = fto_function_eval2d(data->func1, x_pt, y_pt, &val1)) != FTO_OK) return ret;
        if ((ret = fto_function_eval2d(data->func2, x_pt, y_pt, &val2)) != FTO_OK) return ret;
        result = val1 + val2;
    }
    else
    {
        return fto_err_set(FTO_INVALID_ARG, "Invalid function type: %d", func->type);
    }
    *result_out = result;
    return FTO_OK;
}


extern double fto_function_eval2d_smpl(const struct FtoGenericFunc *func, double x_pt, double y_pt)
{
    double result;
    fto_function_eval2d(func, x_pt, y_pt, &result);
    return result;
}



extern enum FtoError fto_function_dot(
        struct FtoVectorFunc *func1,
        struct FtoVectorFunc *func2,
        struct FtoGenericFunc *dot_out)
{
    enum FtoError ret;
    if (func1->order != func2->order)
    {
        return fto_err_set(
                FTO_INVALID_ARG,
                "Can't dot product vector functions of different order: (%d, %d)",
                func1->order, func2->order);
    }
    struct FtoGenericFunc *result = NULL;
    for (int dim = 0; dim < func1->order; ++dim)
    {
        struct FtoGenericFunc *prod;
        if ((ret = fto_function_mult(&func1->funcs[dim], &func2->funcs[dim], &prod)) != FTO_OK) return ret;
        if (result == NULL)
        {
            result = prod;
        }
        else
        {
            if ((ret = fto_function_add(result, prod, &result)) != FTO_OK) return ret;
        }
    }
    if (result == NULL)
    {
        return fto_err_set(FTO_INVALID_ARG, "Invalid function of order 0");
    }
    *dot_out = *result;
    return FTO_OK;
}


extern enum FtoError fto_function_mult(
        const struct FtoGenericFunc *func1,
        const struct FtoGenericFunc *func2,
        struct FtoGenericFunc **func_out)
{
    enum FtoError ret;
    struct FtoGenericFunc *result = fto_malloc(sizeof *result);
    if (func1->type == FTO_POLYNOMIAL_2D && func2->type == FTO_POLYNOMIAL_2D)
    {
        struct FtoPoly2D *poly_result = fto_malloc(sizeof *poly_result);
        if ((ret = fto_poly2d_mult(func1->state, func2->state, poly_result)) != FTO_OK) return ret;
        if ((ret = fto_function_fromPoly2D(poly_result, result)) != FTO_OK) return ret;
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
            *result = (struct FtoGenericFunc){
                .type = FTO_FUNC_COMBINE_MULTIPLY_2D,
                .state = combined
            };
        }
        else
        {
            return fto_err_set(FTO_INVALID_ARG, "Not supported");
        }
    }
    *func_out = result;
    return FTO_OK;
}


extern enum FtoError fto_function_add(
        const struct FtoGenericFunc *func1,
        const struct FtoGenericFunc *func2,
        struct FtoGenericFunc **func_out)
{
    enum FtoError ret;
    struct FtoGenericFunc *result = fto_malloc(sizeof *result);
    if (func1->type == FTO_POLYNOMIAL_2D && func2->type == FTO_POLYNOMIAL_2D)
    {
        struct FtoPoly2D *poly_result = fto_malloc(sizeof *poly_result);
        if ((ret = fto_poly2d_add(func1->state, func2->state, poly_result)) != FTO_OK) return ret;
        if ((ret = fto_function_fromPoly2D(poly_result, result)) != FTO_OK) return ret;
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
            *result = (struct FtoGenericFunc){
                    .type = FTO_FUNC_COMBINE_ADD_2D,
                    .state = combined
            };
        }
    }
    *func_out = result;
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
        case FTO_FUNC_COMBINE_ADD_2D:
            return 2;
    }
    return -1;
}


extern enum FtoError fto_function_print(const struct FtoGenericFunc *func)
{
    enum FtoError ret;
    switch (func->type)
    {
        case FTO_POLYNOMIAL_2D:
            if ((ret = fto_poly2d_print(func->state)) != FTO_OK) return ret;
            break;
        case FTO_POLY_PIECEWISE_2D:
            if ((ret = fto_poly_piecewise2d_print(func->state)) != FTO_OK) return ret;
            break;
        default:
            return fto_err_set(FTO_INVALID_ARG, "Can't print function of type %d", func->type);
    }
    return FTO_OK;
}


extern enum FtoError fto_function_from2DPointer(
        Fto2DFunctionPtr func_ptr,
        void *user_data,
        struct FtoGenericFunc *func_out)
{
    struct Fto2DFunction *func_state = fto_malloc(sizeof *func_state);
    *func_state = (struct Fto2DFunction){
        .function = func_ptr,
        .user_data = user_data
    };
    *func_out = (struct FtoGenericFunc){
        .type = FTO_FUNC_PTR_2D,
        .state = func_state
    };
    return FTO_OK;
}


extern bool fto_function_isEqual(const struct FtoGenericFunc *func1, const struct FtoGenericFunc *func2)
{
    return func1->state == func2->state && func1->type == func2->type;
}
