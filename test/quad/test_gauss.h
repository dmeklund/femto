#ifndef _TEST_GAUSS_H
#define _TEST_GAUSS_H

#include "femto/util.h"
#include "femto/quad/gauss.h"

extern enum FtoError fto_test_quad_gauss_addAll(struct FtoArray *tests);

extern void fto_test_quad_gauss_integrate1d(void **state);
extern void fto_test_quad_gauss_integrate2d_triangle(void **state);

#endif /* _TEST_GAUSS_H */
