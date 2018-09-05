#ifndef TEST_POLY_POLY_H
#define TEST_POLY_POLY_H

#include "femto/util.h"
#include "femto/util/array.h"
#include "femto/poly/poly.h"
#include "fto_test.h"

extern enum FtoError fto_test_poly_poly_addAll(struct FtoArray *tests);

extern void fto_test_poly_poly2d_init(void **state);
extern void fto_test_poly_poly2d_eval(void **state);
extern void fto_test_poly_poly2d_diff(void **state);
extern void fto_test_poly_poly2d_substitute(void **state);


#endif /* TEST_POLY_POLY_H */
