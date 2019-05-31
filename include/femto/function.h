#ifndef FTO_FUNCTION_H
#define FTO_FUNCTION_H

#include "femto/util.h"

#include "femto/poly/poly.h"
#include "femto/poly/piecewise.h"

typedef double (*Fto1DFunctionPtr)(double x);
typedef double (*Fto2DFunctionPtr)(double x, double y, void *user_data);

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

struct Fto1DFunction
{
    Fto1DFunctionPtr function;
};

struct Fto2DFunction
{
    Fto2DFunctionPtr function;
    void *user_data;
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
extern double fto_function_eval2d_smpl(const struct FtoGenericFunc *func, double x_pt, double y_pt);

extern enum FtoError fto_function_fromPoly1D(const struct FtoPoly1D *poly, struct FtoGenericFunc *func_out);
extern enum FtoError fto_function_fromPoly2D(const struct FtoPoly2D *poly, struct FtoGenericFunc *func_out);
extern enum FtoError fto_function_fromPolyPiecewise2D(struct FtoPolyPiecewise2D *poly, struct FtoGenericFunc *func_out);
extern enum FtoError fto_function_from2DPointer(Fto2DFunctionPtr func_ptr, void *user_data, struct FtoGenericFunc *func_out);

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

extern enum FtoError fto_function_print(const struct FtoGenericFunc *func);
extern bool fto_function_isEqual(const struct FtoGenericFunc *func1, const struct FtoGenericFunc *func2);

#endif /* FTO_FUNCTION_H */
