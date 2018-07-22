#ifndef FTO_MESH_H
#define FTO_MESH_H

#include "femto/util.h"
#include "femto/util/array.h"
#include "femto/geom/point.h"
#include "femto/geom/shapes.h"

struct Fto2DMesh
{
    double *nodes; /* C order 2D array num_nodes x 2 */
    int *triangles; /* C order 2D array num_triangles x num_nodesPerTriangle, where each element triangles[i,j]
                     * contains the index (into the nodes array) of the j'th node belonging to the i'th triangle.
                     * First 3 node indexes denote corners of the triangle, in counter-clockwise order.*/
    int num_nodes; /* not equal to num_nodesPerTriangle*num_triangles, since nodes are shared between triangles */
    int num_nodesPerTriangle; /* must be >= 3 */
    int num_triangles;
};

// supplemental data structure to allow lookup of affected triangles by node index
struct Fto2DNodeToTriangleSet
{
    // array of FtoValueArrays, where each FtoValueArray contains integer offsets of all the relevant triangles
    struct FtoArray *nodeToTriangles;
};

extern enum FtoError fto_2dmesh_fromRectangle(struct Fto2DRectangle *rect);
extern enum FtoError fto_2dmesh_constructNodeToTriangleSet(struct Fto2DMesh *mesh, struct Fto2DNodeToTriangleSet *map_out);
extern enum FtoError fto_2dmesh_getTrianglesForNode(
        const struct Fto2DNodeToTriangleSet *nodeToTriangles,
        int node_ind,
        struct FtoValueArray **triangleInds_out);
extern enum FtoError fto_2dmesh_toTriangle(
        const struct Fto2DMesh *mesh,
        int triangle_ind,
        struct Fto2DTriangle *triangle_out);

#endif // FTO_MESH_H
