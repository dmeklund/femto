#ifndef TEST_BASIS_SET_H
#define TEST_BASIS_SET_H

#include "femto/util.h"
#include "fto_test.h"

extern enum FtoError fto_test_basis_set_addAll(struct FtoArray *tests);

extern void fto_test_basis_set_getBasisFunctionForNode(void **state);

#endif /* TEST_BASIS_SET_H */
