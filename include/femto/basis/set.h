#ifndef FEMTO_BASIS_SET_H
#define FEMTO_BASIS_SET_H

#include "femto/util.h"

#include "femto/function.h"

/**
 * A set of basis functions.
 */
struct FtoBasisSet
{
    /**
     * An array of elements of type FtoGenericFunc, representing the basis functions.
     */
    struct FtoArray *basis_functions; // elements of type FtoGenericFunc
};

/**
 * Return the basis function associated with a given node.
 * @param set the basis set
 * @param node_ind the index of the desired node
 * @param func_out is assigned the associated basis function
 * @return FTO_OK if successful, FTO_INVALID_ARG if node_ind is invalid
 */
extern enum FtoError fto_basis_set_getBasisFunctionForNode(
        const struct FtoBasisSet *set,
        int node_ind,
        struct FtoGenericFunc *func_out);

/**
 * Print the set of basis functions in the provided basis set
 * @param set a basis set
 * @return FTO_OK
 */
extern enum FtoError fto_basis_set_print(const struct FtoBasisSet *set);

#endif /* FEMTO_BASIS_SET_H */
