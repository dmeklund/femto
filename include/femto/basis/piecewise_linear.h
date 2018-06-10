#ifndef FTO_BASIS_PIECEWISE_LINEAR
#define FTO_BASIS_PIECEWISE_LINEAR

#include "femto/node/nodeset.h"
#include "femto/mesh/mesh.h"

struct FtoPwl1DFunction
{
    struct Uniform1DPartitionNodeSet node_set;
    int index;
};

struct FtoPwl2DFunction
{
    struct Fto2DMesh *mesh;
    int node_index;
};

extern double fto_basis_pwl2d_eval(const struct FtoPwl2DFunction *func, const struct Fto2DPoint *point);
extern double fto_basis_pwl2d_eval_tri(const struct FtoPwl2DFunction *func, int triangle_ind, const struct Fto2DPoint *point);

#endif // defined FTO_BASIS_PIECEWISE_LINEAR