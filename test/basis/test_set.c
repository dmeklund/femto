#include <femto/basis/set.h>
#include "femto/util.h"

#include "basis/test_set.h"


extern enum FtoError fto_test_basis_set_addAll(struct FtoArray *tests)
{
    enum FtoError ret;
    if ((ret = FTO_TEST_APPEND(tests, fto_test_basis_set_getBasisFunctionForNode)) != FTO_OK)
        return ret;
    return FTO_OK;
}


extern void fto_test_basis_set_getBasisFunctionForNode(void **state)
{
    struct FtoBasisSet *set = fto_malloc(sizeof *set);
    struct FtoGenericFunc *dummy_func = fto_malloc(sizeof *dummy_func);
    set->basis_functions = fto_array_new();
    AOK(fto_array_append(set->basis_functions, dummy_func));
    struct FtoGenericFunc *func_result = fto_malloc(sizeof *func_result);
    AOK(fto_basis_set_getBasisFunctionForNode(set, 0, func_result))
    FTO_ASSERT(fto_function_isEqual(func_result, dummy_func));
    FTO_ASSERT(fto_basis_set_getBasisFunctionForNode(set, 1, func_result) == FTO_INVALID_ARG);
    fto_err_clear();
    FTO_ASSERT(fto_basis_set_getBasisFunctionForNode(set, -1, func_result) == FTO_INVALID_ARG);
    fto_err_clear();
}
