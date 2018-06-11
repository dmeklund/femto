#include "femto/util.h"
#include "femto/linalg/matrix.h"

enum FtoError fto_mat_new(const int num_rows, const int num_cols, struct FtoMatrix **result_out)
{
    enum FtoError ret;
    struct FtoMatrix *result = fto_malloc(sizeof *result);
    if ((ret = fto_mat_init(num_rows, num_cols, result)) != FTO_OK) return ret;
    *result_out = result;
    return FTO_OK;
}


enum FtoError fto_mat_assertValidIndex(const struct FtoMatrix *mat, int i, int j)
{
    enum FtoError ret;
    if ((ret = fto_assert_nonnegative(i)) != FTO_OK) return ret;
    if ((ret = fto_assert_nonnegative(j)) != FTO_OK) return ret;
    if ((ret = fto_assert_lessThan(i, mat->num_rows)) != FTO_OK) return ret;
    if ((ret = fto_assert_lessThan(j, mat->num_cols)) != FTO_OK) return ret;
    return FTO_OK;
}

enum FtoError fto_mat_setval(struct FtoMatrix *mat, int i, int j, double val)
{
    enum FtoError ret;
    if ((ret = fto_mat_assertValidIndex(mat, i, j)) != FTO_OK) return ret;
    FTO_MAT_IDX(mat, i, j) = val;
}


enum FtoError fto_mat_getval(struct FtoMatrix *mat, int i, int j, double *val_out)
{
    enum FtoError ret;
    if ((ret = fto_mat_assertValidIndex(mat, i, j)) != FTO_OK) return ret;
    return FTO_MAT_IDX(mat, i, j);
}


enum FtoError fto_mat_init(int num_rows, int num_cols, struct FtoMatrix *result_out)
{
    *result_out = (struct FtoMatrix){
        .num_rows = num_rows,
        .num_cols = num_cols,
        .values = fto_malloc_atomic(num_rows * num_cols * sizeof *result_out->values)
    };
    return FTO_OK;
}
