#ifndef FTO_LINALG_H
#define FTO_LINALG_H

#include "femto/util.h"

#define FTO_MAT_IDX(mat, i, j) ((mat)->values[(j) + (mat)->num_cols * (i)])

struct FtoMatrix
{
    int num_rows;
    int num_cols;
    double *values;
};

enum FtoError fto_mat_setval(struct FtoMatrix *mat, int i, int j, double val);
enum FtoError fto_mat_getval(struct FtoMatrix *mat, int i, int j, double *val_out);
enum FtoError fto_mat_new(int num_rows, int num_cols, struct FtoMatrix **result_out);
enum FtoError fto_mat_init(int num_rows, int num_cols, struct FtoMatrix *result_out);

#endif // FTO_LINALG_H