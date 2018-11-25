#ifndef FTO_FEM_SOLVER_H
#define FTO_FEM_SOLVER_H

#include "femto/util.h"
#include "femto/mesh/mesh.h"
#include "femto/basis/set.h"
#include "femto/function.h"

extern enum FtoError fto_fem_solve(
        const struct Fto2DMesh *mesh,
        const struct FtoBasisSet *basis_set,
        const struct FtoGenericFunc *f_func,
        const struct FtoGenericFunc *g_func
);

#endif /* FTO_FEM_SOLVER_H */