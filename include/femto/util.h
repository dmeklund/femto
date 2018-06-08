#ifndef FTO_UTIL_H
#define FTO_UTIL_H

#include <stdlib.h>
#include <stdarg.h>

enum FtoError
{
    FTO_OK,
    FTO_ASSERTION_FAILED
};

// memory allocation
void* fto_malloc(size_t sz);

// error handling
enum FtoError fto_err_set(enum FtoError err, const char *err_msg, ...);
enum FtoError fto_err_set_v(enum FtoError err, const char *err_msg, va_list ap);
void fto_err_clear();

// assertions
enum FtoError fto_assert_fail(const char *err_msg, ...);
enum FtoError fto_assert_nonnegative(int val);
enum FtoError fto_assert_lessThan(int val1, int val2);

#endif // FTO_UTIL_H