#include "femto/util.h"

#include "femto/basis/piecewise_linear.h"


extern enum FtoError fto_test_basis_piecewiseLinear_addAll(struct FtoArray *tests)
{
    return FTO_OK;
}


extern void fto_test_basis_pwl2d_eval(void **state)
{
    struct FtoPwl2DFunction *func;
    fto_pwl2d
    fto_basis_pwl2d_eval(func, point);
}
