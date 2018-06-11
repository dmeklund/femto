#ifndef FTO_FEM_STIFFNESS_H
#define FTO_FEM_STIFFNESS_H

#include "femto/util.h"
#include "femto/mesh/mesh.h"
#include "femto/linalg/matrix.h"

extern enum FtoError fto_stiffness_elementMatrix2D(const struct Fto2DMesh *mesh, int triangle_ind, struct FtoMatrix *matrix_out);

#endif // FTO_FEM_STIFFNESS_H