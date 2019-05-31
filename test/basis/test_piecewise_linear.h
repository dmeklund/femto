#ifndef TEST_PIECEWISE_LINEAR_H
#define TEST_PIECEWISE_LINEAR_H

#include "femto/util.h"
#include "fto_test.h"

extern enum FtoError fto_test_basis_piecewiseLinear_addAll(struct FtoArray *tests);

extern void fto_test_basis_pwl2d_eval(void **state);

#endif /* TEST_PIECEWISE_LINEAR_H */

