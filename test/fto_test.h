#ifndef _FTO_TEST_H
#define _FTO_TEST_H

#include "femto/util.h"

#include <setjmp.h>
#include "cmocka.h"

#define FTO_ASSERT(expression) mock_assert((int)(expression), #expression, __FILE__, __LINE__)
#define AOK(expr) FTO_ASSERT((expr) == FTO_OK);

#endif /* _FTO_TEST_H */
