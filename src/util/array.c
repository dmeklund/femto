#include "femto/util.h"
#include "femto/util/array.h"

#include <string.h>

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
    return array;
}


enum FtoError fto_array_append(struct FtoArray *array, void *value)
{
    enum FtoError ret;
    const double scaleFactor = 1.5;
    if ((ret = fto_assert_nonnegative(array->capacity)) != FTO_OK) return ret;
    if (array->length == array->capacity)
    {
        const int new_capacity = (int)(array->capacity * scaleFactor) + 1;
        array->values = fto_realloc(array->values, (size_t)new_capacity * sizeof *array->values);
    }
    array->values[array->length] = value;
    array->length += 1;
    return FTO_OK;
}


extern int fto_intArray_offset(const int *int_array, int val, int length)
{
    for (int ind = 0; ind < length; ++ind)
    {
        if (int_array[ind] == val)
            return ind;
    }
    return -1;
}


extern bool fto_intArray_contains(const int *int_array, int val, int length)
{
    return fto_intArray_offset(int_array, val, length) >= 0;
}


extern struct FtoValueArray* fto_valueArray_new(const size_t element_sz)
{

    const int default_capacity = 4;
    struct FtoValueArray *array = fto_valueArray_new_capacity(element_sz, default_capacity);
    return array;
}


extern struct FtoValueArray* fto_valueArray_new_capacity(size_t element_sz, int capacity)
{
    struct FtoValueArray *array = fto_malloc(sizeof *array);
    *array = (struct FtoValueArray){
            .capacity = capacity,
            .length = 0,
            .element_sz = element_sz,
            .values = fto_malloc(capacity * element_sz)
    };
    return array;
}


extern enum FtoError fto_valueArray_appendCopy(struct FtoValueArray *array, const void *value)
{
    enum FtoError ret;
    const double scaleFactor = 1.5;
    if ((ret = fto_assert_nonnegative(array->capacity)) != FTO_OK) return ret;
    if (array->length == array->capacity)
    {
        const int new_capacity = (int)(array->capacity * scaleFactor) + 1;
        array->values = fto_realloc(array->values, (size_t)new_capacity * array->element_sz);
    }
    memcpy(array->values + array->element_sz*array->length, value, array->element_sz);
    array->length += 1;
    return FTO_OK;
}



extern enum FtoError fto_valueArray_get(struct FtoValueArray *array, int index, void *value_out)
{
    if (index < 0 || index >= array->length)
    {
        return fto_err_set(FTO_INVALID_ARG, "Index out of range: %d", index);
    }
    memcpy(value_out, array->values + index*array->element_sz, array->element_sz);
    return FTO_OK;
}


extern int* fto_intArray_new(int length, ...)
{
    int *result = fto_malloc_atomic(length * sizeof *result);
    va_list args;
    va_start(args, length);
    for (int element_ind = 0; element_ind < length; ++element_ind)
    {
        result[element_ind] = va_arg(args, int);
    }
    va_end(args);
    return result;
}


extern double* fto_doubleArray_new(int length, ...)
{
    double *result = fto_malloc_atomic(length * sizeof *result);
    va_list args;
    va_start(args, length);
    for (int element_ind = 0; element_ind < length; ++element_ind)
    {
        result[element_ind] = va_arg(args, double);
    }
    va_end(args);
    return result;
}


