#ifndef _FTO_TEST_H
#define _FTO_TEST_H

#include "femto/util.h"

#include <setjmp.h>
#include "cmocka.h"
#include "femto/util/array.h"

#if FTO_RAISE_SIGTRAP
#   include <signal.h>
#   define FTO_ASSERT(expression) {int __result = (int)(expression); if (!__result) {raise(SIGTRAP);} mock_assert(__result, #expression, __FILE__, __LINE__);}
#else
#   define FTO_ASSERT(expression) mock_assert((int)(expression), #expression, __FILE__, __LINE__)
#endif
#define AOK(expr) FTO_ASSERT((expr) == FTO_OK);
#define FTO_TEST_APPEND(test_list, test_func) fto_test_appendTest(test_list, test_func, #test_func)

typedef void (*FtoTestFunc)(void **state);

extern enum FtoError fto_test_appendTest(struct FtoArray *test_list, FtoTestFunc test_func, const char *test_name);

#endif /* _FTO_TEST_H */
