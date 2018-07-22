#ifndef FTO_UTIL_ARRAY_H
#define FTO_UTIL_ARRAY_H

#include "femto/util.h"

struct FtoArray
{
    void **values;
    int length;
    int capacity;
};

struct FtoValueArray
{
    void *values;
    int length;
    int capacity;
    size_t element_sz;
};

extern struct FtoArray* fto_array_new();
extern struct FtoArray* fto_array_new_capacity(int capacity);
extern enum FtoError fto_array_append(struct FtoArray *array, void *value);
extern bool fto_intArray_contains(const int *int_array, int val, int length);

extern struct FtoValueArray* fto_valueArray_new(size_t element_sz);
extern struct FtoValueArray* fto_valueArray_new_capacity(size_t element_sz, int capacity);
extern enum FtoError fto_valueArray_appendCopy(struct FtoValueArray *array, const void *value);

#endif /* FTO_UTIL_ARRAY_H */
