#include "femto/node/nodeset.h"
#include "femto/mesh/mesh.h"

struct PiecewiseLinearBasisFunction
{
    struct Uniform1DPartitionNodeSet node_set;
    int index;
};


struct PiecewiseLinearBasisFunction2D
{
    struct Fto2DMesh *mesh;
    int node_index;
};
