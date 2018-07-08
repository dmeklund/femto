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


void* fto_malloc_atomic(size_t sz)
{
    return GC_MALLOC_ATOMIC(sz);
}


void* fto_realloc(void *ptr, const size_t new_sz)
{
    void *result = GC_REALLOC(ptr, new_sz);
    return result;
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


struct FtoArray* fto_array_new()
{
    const int default_capacity = 4;
    struct FtoArray *array = fto_array_new_capacity(default_capacity);
    return array;
}


struct FtoArray* fto_array_new_capacity(const int capacity)
{
    struct FtoArray *array = fto_malloc(sizeof *array);
    *array = (struct FtoArray){
        .capacity = capacity,
        .length = 0,
        .values = fto_malloc(capacity * sizeof *array->values)
    };
}


enum FtoError fto_array_append(struct FtoArray *array, void *value)
{
    enum FtoError ret;
    const double scaleFactor = 1.5;
    if ((ret = fto_assert_nonnegative(array->capacity)) != FTO_OK) return ret;
    if (array->length == array->capacity)
    {
        const int new_capacity = (int)(array->capacity * scaleFactor) + 1;
        array->values = fto_realloc(array->values, (size_t)new_capacity);
    }
    array->values[array->length] = value;
    array->length += 1;
    return FTO_OK;
}


bool fto_intArray_contains(const int *int_array, int val, int length)
{
    for (size_t ind = 0; ind < length; ++ind)
    {
        if (int_array[ind] == val)
            return true;
    }
    return false;
}


enum FtoError fto_assert_greaterThanEqual(int val1, int val2)
{
    if (val1 < val2)
    {
        return fto_err_set(FTO_ASSERTION_FAILED, "%d not greater than or equal to %d", val1, val2);
    }
    return FTO_OK;
}