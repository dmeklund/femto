#ifndef FEMTO_BASIS_SET_H
#define FEMTO_BASIS_SET_H

#include "femto/util.h"

#include "femto/function.h"

struct FtoBasisSet
{
    struct FtoArray *basis_functions; // elements of type FtoGenericFunc
};

extern enum FtoError fto_basis_set_getBasisFunctionForNode(
        const struct FtoBasisSet *set,
        int node_ind,
        struct FtoGenericFunc *func_out);

#endif /* FEMTO_BASIS_SET_H */
