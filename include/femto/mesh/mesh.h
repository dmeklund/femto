#ifndef FTO_MESH_H
#define FTO_MESH_H

#include "femto/util.h"
#include "femto/geom/point.h"
#include "femto/geom/shapes.h"

struct Fto2DMesh
{
    double *nodes; /* C order 2D array num_nodes x 2 */
    int *triangles; /* C order 2D array num_triangles x 3, where each element triangles[i,j] contains the index (into
                        * the nodes array) of the j'th node belonging to the i'th triangle */
    int num_nodes;
    int num_triangles;
};

extern enum FtoError fto_2dmesh_fromRectangle(struct Fto2DRectangle *rect);

#endif // FTO_MESH_H
