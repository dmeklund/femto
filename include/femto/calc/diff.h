#ifndef FTO_CALC_DIFF_H
#define FTO_CALC_DIFF_H

#include "femto/util.h"
#include "femto/poly/poly.h"
#include "femto/function.h"

extern enum FtoError fto_calc_grad_poly2d(const struct FtoPoly2D *poly, struct FtoVectorFunc *grad_out);
extern enum FtoError fto_calc_grad(struct FtoGenericFunc *function, struct FtoVectorFunc *grad_out);

#endif /* FTO_CALC_DIFF_H */
