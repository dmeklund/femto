#include "femto/util.h"
#include "femto/basis/set.h"

#include "femto/function.h"

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
