#include "femto/util.h"
#include "fto_test.h"

extern enum FtoError fto_test_appendTest(struct FtoArray *test_list, FtoTestFunc test_func, const char *test_name)
{
    enum FtoError ret;
    struct CMUnitTest *test = fto_malloc(sizeof *test);
    *test = (struct CMUnitTest){
            .test_func = test_func,
            .name = test_name
    };
    if ((ret = fto_array_append(test_list, test)) != FTO_OK) return ret;
    return FTO_OK;
}
