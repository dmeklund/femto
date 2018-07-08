#ifndef FTO_QUAD_GAUSS_H
#define FTO_QUAD_GAUSS_H

#include "femto/util.h"
#include "femto/function.h"

extern enum FtoError fto_gauss_integrate1d(struct FtoGeneric1DFunc *func, double a, double b, int num_nodes, double *result_out);

#endif /* FTO_QUAD_GAUSS_H */
