#include "femto/util.h"
#include "fto_test.h"

#include "fem/test_stiffness.h"


extern enum FtoError fto_test_fem_stiffness_addAll(struct FtoArray *tests)
{
    AOK(FTO_TEST_APPEND(tests, fto_test_fem_stiffness_calculateStiffnessMatrix2D));
    return FTO_OK;
}


extern void fto_test_fem_stiffness_calculateStiffnessMatrix2D(void **state)
{
    (void)state;
}

