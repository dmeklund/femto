#include "femto/util.h"
#include "femto/basis/set.h"

#include "femto/function.h"

#include <stdio.h>

extern enum FtoError fto_basis_set_getBasisFunctionForNode(
        const struct FtoBasisSet *set,
        int node_ind,
        struct FtoGenericFunc *func_out)
{
    if (node_ind < 0 || node_ind >= set->basis_functions->length)
        return fto_err_set(FTO_INVALID_ARG, "Invalid node index: %d", node_ind);
    *func_out = *((struct FtoGenericFunc*)set->basis_functions->values[node_ind]);
    return FTO_OK;
}


extern enum FtoError fto_basis_set_print(const struct FtoBasisSet *set)
{
    enum FtoError ret;
    for (int basis_ind = 0; basis_ind < set->basis_functions->length; ++basis_ind)
    {
        struct FtoGenericFunc *func = set->basis_functions->values[basis_ind];
        printf("Basis function %d: ", basis_ind);
        if ((ret = fto_function_print(func)) != FTO_OK) return ret;
        printf("\n");
    }
    return FTO_OK;
}
