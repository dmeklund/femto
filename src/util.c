#include "femto/util.h"

#include <math.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include <gc.h>


static char _global_error_buf[255] = {'\0'};
static enum FtoError _global_error_val = FTO_OK;

void* fto_malloc(size_t sz)
{
#if USE_GC
    return GC_MALLOC(sz);
#else
    void *result = malloc(sz);
    memset(result, 0, sz);
    return result;
#endif
}


void* fto_malloc_atomic(size_t sz)
{
#if USE_GC
    return GC_MALLOC_ATOMIC(sz);
#else
   return malloc(sz);
#endif
}


void* fto_realloc(void *ptr, const size_t new_sz)
{
#if USE_GC
    void *result = GC_REALLOC(ptr, new_sz);
    return result;
#else
    return realloc(ptr, new_sz);
#endif
}


enum FtoError fto_err_set_v(const enum FtoError err, const char *err_msg, va_list ap)
{
    vsnprintf(_global_error_buf, 255, err_msg, ap);
    _global_error_val = err;
    return err;
}


enum FtoError fto_err_set(const enum FtoError err, const char *err_msg, ...)
{
    enum FtoError ret;
#if FTO_RAISE_SIGTRAP > 0
    raise(SIGTRAP);
#endif
    va_list ap;
    va_start(ap, err_msg);
    ret = fto_err_set_v(err, err_msg, ap);
    va_end(ap);
    return ret;
}

void fto_err_clear()
{
    _global_error_val = FTO_OK;
    _global_error_buf[0] = '\0';
}


const char* fto_err_get()
{
    return _global_error_buf;
}


enum FtoError fto_assert_fail(const char *err_msg, ...)
{
    va_list ap;
    va_start(ap, err_msg);
    enum FtoError ret = fto_err_set_v(FTO_ASSERTION_FAILED, err_msg, ap);
    va_end(ap);
    return ret;
}


enum FtoError fto_assert_nonnegative(int val)
{
    if (val < 0)
        return fto_assert_fail("Value is negative: %d", val);
    return FTO_OK;
}


enum FtoError fto_assert_lessThan(int val1, int val2)
{
    if (val1 >= val2) return fto_assert_fail("%d is not less than %d", val1, val2);
    return FTO_OK;
}


enum FtoError fto_assert_greaterThanEqual(int val1, int val2)
{
    if (val1 < val2)
    {
        return fto_err_set(FTO_ASSERTION_FAILED, "%d not greater than or equal to %d", val1, val2);
    }
    return FTO_OK;
}


extern bool fto_isClose(double val1, double val2, double rtol, double atol)
{
    double diff = fabs(val1 - val2);
    bool result = (diff <= atol || (val1 != 0 && diff / fabs(val1) <= rtol));
    return result;
}


extern bool fto_assertClose(double val1, double val2, double rtol, double atol)
{
    if (!fto_isClose(val1, val2, rtol, atol))
    {
        return fto_err_set(
                FTO_ASSERTION_FAILED,
                "Values %g, %g not close within tolerance (abs: %g, rel: %g)",
                val1, val2, atol, rtol);
    }
    return FTO_OK;
}


extern bool fto_isClose_default(double val1, double val2)
{
    return fto_isClose(val1, val2, FTO_DEFAULT_RTOL, FTO_DEFAULT_ATOL);
}


extern bool fto_nearlyNonNegative(double val)
{
    return (val + 1e-8) > 0;
}


extern enum FtoError fto_assert_notNull(const void *ptr)
{
    if (ptr == NULL)
    {
        return fto_err_set(FTO_NULL, "Value is NULL");
    }
    return FTO_OK;
}


extern enum FtoError fto_assert_int_equal(int val1, int val2)
{
    if (val1 != val2)
    {
        return fto_err_set(FTO_ASSERTION_FAILED, "%d != %d", val1, val2);
    }
    return FTO_OK;
}
