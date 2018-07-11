#include "femto/util.h"
#include "femto/quad/gauss.h"
#include "femto/geom/shapes.h"


const double nodes2[] = {-0.5773502691896257, 0.5773502691896257};
const double weights2[] = {1, 1};
const double nodes3[] = {0, -0.7745966692414834, 0.7745966692414834};
const double weights3[] = {0.8888888888888888, 0.5555555555555556, 0.5555555555555556};
const double nodes4[] = {-0.3399810435848563, 0.3399810435848563, -0.8611363115940526, 0.8611363115940526};
const double weights4[] = {0.6521451548625461, 0.6521451548625461, 0.3478548451374538, 0.3478548451374538};
const double nodes5[] = {0, -0.5384693101056831, 0.5384693101056831, -0.9061798459386640, 0.9061798459386640};
const double weights5[] = {0.5688888888888889, 0.4786286704993665, 0.4786286704993665, 0.2369268850561891, 0.2369268850561891};
const double nodes6[] = {-0.6612093864662645, 0.6612093864662645, -0.2386191860831969, 0.2386191860831969, -0.9324695142031521, 0.9324695142031521};
const double weights6[] = {0.3607615730481386, 0.3607615730481386, 0.4679139345726910, 0.4679139345726910, 0.1713244923791704, 0.1713244923791704};
const int max_num_nodes = 6;
const int min_num_nodes = 2;
const double *all_nodes[] = {nodes2, nodes3, nodes4, nodes5, nodes6};
const double *all_weights[] = {weights2, weights3, weights4, weights5, weights6};


static enum FtoError getNodesAndWeights(int num_nodes, const double **nodes_out, const double **weights_out)
{
    if (num_nodes < min_num_nodes || num_nodes > max_num_nodes)
    {
        return fto_err_set(
                FTO_INVALID_ARG,
                "Number of gaussian nodes (%d) must be between %d and %d (inclusive)",
                num_nodes, min_num_nodes, max_num_nodes);
    }
    *nodes_out = all_nodes[num_nodes-min_num_nodes];
    *weights_out = all_weights[num_nodes-min_num_nodes];
    return FTO_OK;
}


static double transform2dNodeToXY(const double *node, const struct Fto2DTriangle *triangle, double *x_out, double *y_out)
{
    const double xi = node[0];
    const double eta = node[1];
    const double n1 = 1-xi-eta;
    const double n2 = xi;
    const double n3 = eta;
    *x_out = triangle->point1->x * n1 + triangle->point2->x * n2 + triangle->point3->x * n3;
    *y_out = triangle->point1->y * n1 + triangle->point2->y * n2 + triangle->point3->y * n3;
}


static enum FtoError getNodesAndWeights2d(int num_nodes, const double **nodes_out, const double **weights_out)
{
    return FTO_OK;
}


extern enum FtoError fto_gauss_integrate1d(struct FtoGeneric1DFunc *func, double a, double b, int num_nodes, double *result_out)
{
    enum FtoError ret;
    const double *nodes;
    const double *weights;
    if ((ret = getNodesAndWeights(num_nodes, &nodes, &weights)) != FTO_OK) return ret;
    double result = 0;
    for (int node_ind = 0; node_ind < num_nodes; ++node_ind)
    {
        double evaled;
        if ((ret = fto_function_eval1d(func, a + (b-a)/2 * (1 + nodes[node_ind]), &evaled)) != FTO_OK) return ret;
        result += weights[node_ind] * evaled;
    }
    result *= (b-a) / 2;
    *result_out = result;
    return FTO_OK;
}


extern enum FtoError fto_gauss_integrate2d_triangle(
        struct FtoGeneric2DFunc *func,
        struct Fto2DTriangle *triangle,
        int num_nodes,
        double *result_out)
{
    enum FtoError ret;
    const double *nodes;
    const double *weights;
    if ((ret = getNodesAndWeights2d(num_nodes, &nodes, &weights)) != FTO_OK) return ret;
    double result = 0;
    const double triangle_area = 0;
    for (int node_ind = 0; node_ind < num_nodes; ++node_ind)
    {
        double evaled[2];
        const double *node = &nodes[2*node_ind];
        double x, y;
        transform2dNodeToXY(node, triangle, &x, &y);
        if ((ret = fto_function_eval2d(func, x, y, evaled)) != FTO_OK) return ret;
        result += triangle_area;
    }
    return FTO_OK;
}
