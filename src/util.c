#include <stdarg.h>
#include <stdio.h>

#include <gc.h>

#include "femto/util.h"

static char _global_error_buf[255] = {'\0'};
static enum FtoError _global_error_val = FTO_OK;

void* fto_malloc(size_t sz)
{
    return GC_MALLOC(sz);
}


enum FtoError fto_err_set_v(enum FtoError err, const char *err_msg, va_list ap)
{
    vsnprintf(_global_error_buf, 255, err_msg, ap);
    _global_error_val = err;
    return err;
}


enum FtoError fto_err_set(enum FtoError err, const char *err_msg, ...)
{
    enum FtoError ret;
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
    if (val < 0) return fto_assert_fail("Value is negative: %d", val);
    return FTO_OK;
}


enum FtoError fto_assert_lessThan(int val1, int val2)
{
    if (val1 >= val2) return fto_assert_fail("%d is not less than %d", val1, val2);
    return FTO_OK;
}
