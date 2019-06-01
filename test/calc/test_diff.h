#ifndef TEST_CALC_DIFF_H
#define TEST_CALC_DIFF_H

#include "femto/util.h"
#include "fto_test.h"

extern enum FtoError fto_test_calc_diff_addAll(struct FtoArray *tests);

extern void fto_test_calc_grad_poly2d(void **state);

#endif /* TEST_CALC_DIFF_H */
