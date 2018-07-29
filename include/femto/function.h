#ifndef FTO_FUNCTION_H
#define FTO_FUNCTION_H

#include "femto/util.h"

#include "femto/poly/poly.h"
#include "femto/poly/piecewise.h"

typedef double (*Fto1DFunctionPtr)(double x);
typedef double (*Fto2DFunctionPtr)(double x, double y);

enum FtoFunctionType
{
    FTO_POLYNOMIAL_1D,
    FTO_POLYNOMIAL_2D,
    FTO_POLY_PIECEWISE_2D,
    FTO_FUNC_PTR_1D,
    FTO_FUNC_PTR_2D,
    FTO_FUNC_COMBINE_MULTIPLY_2D,
    FTO_FUNC_COMBINE_ADD_2D
};

struct FtoGenericFunc
{
    enum FtoFunctionType type;
    const void *state;
};

struct FtoCombinedFuncData
{
    const struct FtoGenericFunc *func1, *func2;
};

struct FtoVectorFunc
{
    struct FtoGenericFunc *funcs;
    int order;
};

extern enum FtoError fto_function_eval1d(const struct FtoGenericFunc *func, double x_pt, double *result_out);
extern enum FtoError fto_function_eval2d(const struct FtoGenericFunc *func, double x_pt, double y_pt, double *result_out);
extern enum FtoError fto_function_fromPoly1D(const struct FtoPoly1D *poly, struct FtoGenericFunc *func_out);
extern enum FtoError fto_function_fromPoly2D(const struct FtoPoly2D *poly, struct FtoGenericFunc *func_out);
extern enum FtoError fto_function_fromPolyPiecewise2D(struct FtoPolyPiecewise2D *poly, struct FtoGenericFunc *func_out);
extern enum FtoError fto_function_dot(
        struct FtoVectorFunc *func1,
        struct FtoVectorFunc *func2,
        struct FtoGenericFunc *result_out);
extern enum FtoError fto_function_mult(
        const struct FtoGenericFunc *func1,
        const struct FtoGenericFunc *func2,
        struct FtoGenericFunc **func_out);
extern enum FtoError fto_function_add(
        const struct FtoGenericFunc *func1,
        const struct FtoGenericFunc *func2,
        struct FtoGenericFunc **func_out);
extern int fto_function_ndims(const struct FtoGenericFunc *func);

#endif /* FTO_FUNCTION_H */
