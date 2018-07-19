#ifndef FTO_FUNCTION_H
#define FTO_FUNCTION_H

#include "femto/basis/poly.h"

typedef double (*Fto1DFunctionPtr)(double x);
typedef double (*Fto2DFunctionPtr)(double x, double y);

enum FtoFunctionType
{
    FTO_POLYNOMIAL_1D,
    FTO_POLYNOMIAL_2D,
    FTO_FUNC_PTR_1D,
    FTO_FUNC_PTR_2D
};

struct FtoGenericFunc
{
    enum FtoFunctionType type;
    void *state;
};

struct FtoVectorFunc
{
    struct FtoGenericFunc *funcs;
    int order;
};


extern enum FtoError fto_function_eval1d(struct FtoGenericFunc *func, double x_pt, double *result_out);
extern enum FtoError fto_function_eval2d(struct FtoGenericFunc *func, double x_pt, double y_pt, double *result_out);
extern enum FtoError fto_function_fromPoly1D(struct FtoPoly1D *poly, struct FtoGenericFunc *func_out);
extern enum FtoError fto_function_fromPoly2D(struct FtoPoly2D *poly, struct FtoGenericFunc *func_out);
extern enum FtoError fto_function_dot(
        struct FtoVectorFunc *func1,
        struct FtoVectorFunc *func2);
extern enum FtoError fto_function_mult(
        const struct FtoGenericFunc *func1,
        const struct FtoGenericFunc *func2,
        struct FtoGenericFunc *func_out);

#endif /* FTO_FUNCTION_H */
