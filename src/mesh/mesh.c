#include "femto/util.h"
#include "femto/mesh/mesh.h"

#include "femto/util/array.h"

extern enum FtoError fto_2dmesh_fromRectangle(struct Fto2DRectangle *rect, struct Fto2DMesh *mesh_out)
{
    const int num_nodes = 4;
    const int num_triangles = 2;
    *mesh_out = (struct Fto2DMesh){
        .num_nodes = num_nodes,
        .num_triangles = num_triangles,
        .nodes = fto_malloc(num_nodes * sizeof *mesh_out->nodes),
        .triangles = fto_malloc(num_triangles * sizeof *mesh_out->triangles)
    };
    FTO_C_ORDER_2D_IDX(mesh_out->nodes, mesh_out->num_nodes, 2, 0, 0) = rect->corner1->x;
    FTO_C_ORDER_2D_IDX(mesh_out->nodes, mesh_out->num_nodes, 2, 0, 1) = rect->corner1->y;
    FTO_C_ORDER_2D_IDX(mesh_out->nodes, mesh_out->num_nodes, 2, 1, 0) = rect->corner2->x;
    FTO_C_ORDER_2D_IDX(mesh_out->nodes, mesh_out->num_nodes, 2, 1, 1) = rect->corner2->y;
    FTO_C_ORDER_2D_IDX(mesh_out->nodes, mesh_out->num_nodes, 2, 2, 0) = rect->corner2->x;
    FTO_C_ORDER_2D_IDX(mesh_out->nodes, mesh_out->num_nodes, 2, 2, 1) = rect->corner1->y;
    FTO_C_ORDER_2D_IDX(mesh_out->nodes, mesh_out->num_nodes, 2, 3, 0) = rect->corner2->x;
    FTO_C_ORDER_2D_IDX(mesh_out->nodes, mesh_out->num_nodes, 2, 3, 1) = rect->corner1->y;
    FTO_C_ORDER_2D_IDX(mesh_out->triangles, mesh_out->num_triangles, 3, 0, 0) = 0;
    FTO_C_ORDER_2D_IDX(mesh_out->triangles, mesh_out->num_triangles, 3, 0, 1) = 1;
    FTO_C_ORDER_2D_IDX(mesh_out->triangles, mesh_out->num_triangles, 3, 0, 2) = 2;
    FTO_C_ORDER_2D_IDX(mesh_out->triangles, mesh_out->num_triangles, 3, 1, 0) = 0;
    FTO_C_ORDER_2D_IDX(mesh_out->triangles, mesh_out->num_triangles, 3, 1, 1) = 1;
    FTO_C_ORDER_2D_IDX(mesh_out->triangles, mesh_out->num_triangles, 3, 1, 2) = 3;
    return FTO_OK;
}


extern enum FtoError fto_2dmesh_constructNodeToTriangleSet(
        struct Fto2DMesh *mesh,
        struct Fto2DNodeToTriangleSet *map_out)
{
    enum FtoError ret;
    struct FtoArray *nodeToTriangles = fto_array_new_capacity(mesh->num_nodes);
    for (int triangle_ind = 0; triangle_ind < mesh->num_triangles; ++triangle_ind)
    {
        for (int triangleNode_ind = 0; triangleNode_ind < mesh->num_nodesPerTriangle; ++triangleNode_ind)
        {
            const int node_ind = mesh->triangles[mesh->num_nodesPerTriangle*triangle_ind + triangleNode_ind];
            struct FtoValueArray *node_values = nodeToTriangles->values[node_ind];
            if (node_values == NULL)
            {
                node_values = fto_valueArray_new_capacity(sizeof(int), 1);
                nodeToTriangles->values[node_ind] = node_values;
            }
            if ((ret = fto_valueArray_appendCopy(node_values, &triangle_ind)) != FTO_OK) return ret;
        }
    }
    nodeToTriangles->length = mesh->num_nodes;
    map_out->nodeToTriangles = nodeToTriangles;
    return FTO_OK;
}


extern enum FtoError fto_2dmesh_getTrianglesForNode(
        const struct Fto2DNodeToTriangleSet *nodeToTriangles,
        int node_ind,
        struct FtoValueArray **triangleInds_out)
{
    if (node_ind < 0 || node_ind >= nodeToTriangles->nodeToTriangles->length)
    {
        return fto_err_set(FTO_INVALID_ARG, "Invalid node index: %d", node_ind);
    }
    *triangleInds_out = nodeToTriangles->nodeToTriangles->values[node_ind];
    return FTO_OK;
}


extern enum FtoError fto_2dmesh_toTriangle(
        const struct Fto2DMesh *mesh,
        int triangle_ind,
        struct Fto2DTriangle *triangle_out)
{
    if (triangle_ind < 0 || triangle_ind >= mesh->num_triangles)
        return fto_err_set(FTO_INVALID_ARG, "Invalid triangle index: %d", triangle_ind);
    int *triangleNodes = &mesh->triangles[2*triangle_ind];
    double *node1 = &mesh->nodes[2*triangleNodes[0]];
    double *node2 = &mesh->nodes[2*triangleNodes[1]];
    double *node3 = &mesh->nodes[2*triangleNodes[2]];
    *triangle_out = (struct Fto2DTriangle){
        .point1 = {.x = node1[0], .y = node1[1]},
        .point2 = {.x = node2[0], .y = node2[1]},
        .point3 = {.x = node3[0], .y = node3[1]}
    };
    return FTO_OK;
}


extern enum FtoError fto_2dmesh_fromTriangle(struct Fto2DTriangle *triangle, struct Fto2DMesh *mesh_out)
{
    int *triangles = fto_intArray_new(3, 0, 1, 2);
    double *nodes = fto_doubleArray_new(
            6,
            triangle->point1.x, triangle->point1.y,
            triangle->point2.x, triangle->point2.y,
            triangle->point3.x, triangle->point3.y);
    *mesh_out = (struct Fto2DMesh){
        .num_nodesPerTriangle = 3,
        .num_nodes = 3,
        .num_triangles = 1,
        .triangles = triangles,
        .nodes = nodes
    };
    return FTO_OK;
}
