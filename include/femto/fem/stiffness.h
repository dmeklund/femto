#ifndef FTO_FEM_STIFFNESS_H
#define FTO_FEM_STIFFNESS_H

#include "femto/util.h"

#include "femto/basis/set.h"
#include "femto/linalg/matrix.h"
#include "femto/mesh/mesh.h"

extern enum FtoError fto_stiffness_elementMatrix2D(
        const struct Fto2DMesh *mesh,
        const struct FtoBasisSet *basis_set,
        int triangle_ind,
        struct FtoMatrix *matrix_out);
extern enum FtoError fto_stiffness_calculateStiffnessMatrix2D(
        const struct Fto2DMesh *mesh,
        const struct FtoBasisSet *basis_set,
        struct FtoMatrix *matrix_out);

#endif // FTO_FEM_STIFFNESS_H
