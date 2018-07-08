#include "femto/util.h"
#include "femto/quad/gauss.h"


const double nodes2[] = {-0.5773502691896257, 0.5773502691896257};
const double weights2[] = {1, 1};
const int max_num_nodes = 2;
const int min_num_nodes = 2;
const double *all_nodes[] = {nodes2};
const double *all_weights[] = {weights2};

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
}
