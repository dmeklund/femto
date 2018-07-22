#ifndef FTO_BASIS_PIECEWISE_POLY
#define FTO_BASIS_PIECEWISE_POLY

#include "femto/util.h"
#include "femto/basis/set.h"
#include "femto/mesh/mesh.h"

extern enum FtoError fto_basis_piecewisePoly_construct(
        const struct Fto2DMesh *mesh,
        const struct Fto2DNodeToTriangleSet *nodeToTriangles,
        int poly_order,
        struct FtoBasisSet *set_out);

#endif /* FTO_BASIS_PIECEWISE_POLY */
