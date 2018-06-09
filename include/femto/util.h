#ifndef FTO_UTIL_H
#define FTO_UTIL_H

#include <stdlib.h>
#include <stdarg.h>

#define FTO_C_ORDER_2D_OFFSET(nx, ny, ix, iy) ((iy) + (ny)*(ix))
#define FTO_C_ORDER_2D_IDX(arr, nx, ny, ix, iy) arr[FTO_C_ORDER_2D_OFFSET(nx, ny, ix, iy)]

enum FtoError
{
    FTO_OK,
    FTO_ASSERTION_FAILED
};

struct FtoArray
{
    void **values;
    int length;
    int capacity;
};

// memory allocation
void* fto_malloc(size_t sz);
void* fto_realloc(void *ptr, size_t new_sz);

// error handling
enum FtoError fto_err_set(enum FtoError err, const char *err_msg, ...);
enum FtoError fto_err_set_v(enum FtoError err, const char *err_msg, va_list ap);
void fto_err_clear();

// assertions
enum FtoError fto_assert_fail(const char *err_msg, ...);
enum FtoError fto_assert_nonnegative(int val);
enum FtoError fto_assert_lessThan(int val1, int val2);

// arrays
struct FtoArray* fto_array_new();
struct FtoArray* fto_array_new_capacity(int capacity);
enum FtoError fto_array_append(struct FtoArray *array, void *value);

#endif // FTO_UTIL_H