#ifndef FTO_FUNCTION_H
#define FTO_FUNCTION_H

#include "femto/basis/poly.h"

//typedef double (*Fto1DFunc)(double x);
enum FtoFunctionType
{
    FTO_POLYNOMIAL
};

struct FtoGeneric1DFunc
{
    enum FtoFunctionType type;
    void *state;
};

extern enum FtoError fto_function_eval1d(struct FtoGeneric1DFunc *func, double x_pt, double *result_out);
extern enum FtoError fto_function_fromPoly1D(struct FtoPoly1D *poly, struct FtoGeneric1DFunc *func_out);

#endif /* FTO_FUNCTION_H */
