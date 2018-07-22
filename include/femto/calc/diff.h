#ifndef FTO_CALC_DIFF_H
#define FTO_CALC_DIFF_H

#include "femto/util.h"
#include "femto/poly/poly.h"
#include "femto/function.h"

extern enum FtoError fto_basis_poly2d_grad(const struct FtoPoly2D *poly, struct FtoVectorFunc *grad_out);

#endif /* FTO_CALC_DIFF_H */
