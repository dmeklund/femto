#include "femto/util.h"
#include "femto/mesh/mesh.h"

extern enum FtoError fto_2dmesh_fromRectangle(struct Fto2DRectangle *rect)
{
    struct Fto2DMesh *mesh = fto_malloc(sizeof *mesh);
    const int num_nodes = 4;
    const int num_triangles = 2;
    *mesh = (struct Fto2DMesh){
        .num_nodes = num_nodes,
        .num_triangles = num_triangles,
        .nodes = fto_malloc(num_nodes * sizeof *mesh->nodes),
        .triangles = fto_malloc(num_triangles * sizeof *mesh->triangles)
    };
    FTO_C_ORDER_2D_IDX(mesh->nodes, mesh->num_nodes, 2, 0, 0) = rect->corner1->x;
    FTO_C_ORDER_2D_IDX(mesh->nodes, mesh->num_nodes, 2, 0, 1) = rect->corner1->y;
    FTO_C_ORDER_2D_IDX(mesh->nodes, mesh->num_nodes, 2, 1, 0) = rect->corner2->x;
    FTO_C_ORDER_2D_IDX(mesh->nodes, mesh->num_nodes, 2, 1, 1) = rect->corner2->y;
    FTO_C_ORDER_2D_IDX(mesh->nodes, mesh->num_nodes, 2, 2, 0) = rect->corner2->x;
    FTO_C_ORDER_2D_IDX(mesh->nodes, mesh->num_nodes, 2, 2, 1) = rect->corner1->y;
    FTO_C_ORDER_2D_IDX(mesh->nodes, mesh->num_nodes, 2, 3, 0) = rect->corner2->x;
    FTO_C_ORDER_2D_IDX(mesh->nodes, mesh->num_nodes, 2, 3, 1) = rect->corner1->y;
    FTO_C_ORDER_2D_IDX(mesh->triangles, mesh->num_triangles, 3, 0, 0) = 0;
    FTO_C_ORDER_2D_IDX(mesh->triangles, mesh->num_triangles, 3, 0, 1) = 1;
    FTO_C_ORDER_2D_IDX(mesh->triangles, mesh->num_triangles, 3, 0, 2) = 2;
    FTO_C_ORDER_2D_IDX(mesh->triangles, mesh->num_triangles, 3, 1, 0) = 0;
    FTO_C_ORDER_2D_IDX(mesh->triangles, mesh->num_triangles, 3, 1, 1) = 1;
    FTO_C_ORDER_2D_IDX(mesh->triangles, mesh->num_triangles, 3, 1, 2) = 3;
}
