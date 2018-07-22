#ifndef FTO_UTIL_H
#define FTO_UTIL_H

#define _GNU_SOURCE /* strcasestr */
#define __PURE_INTEL_C99_HEADERS__

#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

#define FTO_C_ORDER_2D_OFFSET(nx, ny, ix, iy) ((iy) + (ny)*(ix))
#define FTO_C_ORDER_2D_IDX(arr, nx, ny, ix, iy) arr[FTO_C_ORDER_2D_OFFSET(nx, ny, ix, iy)]

#define FTO_DBL_SWAP(x1, x2) {const double tmp = x1; x1 = x2; x2 = tmp;}
#define FTO_SQUARE(x) ((x)*(x))

#define FTO_DEFAULT_RTOL 1e-7
#define FTO_DEFAULT_ATOL 1e-13

#define USE_GC 0


enum FtoError
{
    FTO_OK,
    FTO_ASSERTION_FAILED,
    FTO_INVALID_ARG
};

// memory allocation
extern void* fto_malloc(size_t sz);
extern void* fto_malloc_atomic(size_t sz);
extern void* fto_realloc(void *ptr, size_t new_sz);

// error handling
extern enum FtoError fto_err_set(enum FtoError err, const char *err_msg, ...);
extern enum FtoError fto_err_set_v(enum FtoError err, const char *err_msg, va_list ap);
extern void fto_err_clear();
extern const char* fto_err_get();

// assertions
extern enum FtoError fto_assert_fail(const char *err_msg, ...);
extern enum FtoError fto_assert_nonnegative(int val);
extern enum FtoError fto_assert_lessThan(int val1, int val2);
extern enum FtoError fto_assert_greaterThanEqual(int val1, int val2);
extern bool fto_assertClose(double val1, double val2, double rtol, double atol);

// other utilities
extern bool fto_isClose(double val1, double val2, double rtol, double atol);


#endif // FTO_UTIL_H
