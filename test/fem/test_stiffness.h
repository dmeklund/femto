#ifndef TEST_FEM_STIFFNESS_H
#define TEST_FEM_STIFFNESS_H

#include "femto/util.h"
#include "fto_test.h"
#include "femto/util/array.h"

extern enum FtoError fto_test_fem_stiffness_addAll(struct FtoArray *tests);

extern void fto_test_fem_stiffness_calculateStiffnessMatrix2D(void **state);
extern void fto_test_fem_stiffness_elementMatrix2D(void **state);

#endif /* TEST_FEM_STIFFNESS_H */
