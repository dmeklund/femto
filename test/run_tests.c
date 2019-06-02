#include "femto/util.h"
#include "fto_test.h"
#include "all_tests.h"

#include <stdio.h>
#include <string.h>

static void print_help()
{
    printf("help\n");
}

static void addAll(struct FtoArray *test_array)
{
    AOK(fto_test_basis_set_addAll(test_array));
    AOK(fto_test_basis_piecewisePoly_addAll(test_array));
    AOK(fto_test_calc_diff_addAll(test_array));
    AOK(fto_test_fem_solver_addAll(test_array));
    AOK(fto_test_fem_stiffness_addAll(test_array));
    AOK(fto_test_poly_poly_addAll(test_array));
    AOK(fto_test_quad_gauss_addAll(test_array));
}

extern int main(int argc, char *argv[])
{
    struct FtoArray *test_array = fto_array_new();
    if (argc > 2)
    {
        print_help();
        exit(1);
    }
    addAll(test_array);
    const char *filter = "";
    if (argc == 2)
    {
        filter = argv[1];
    }
    struct CMUnitTest *tests = fto_malloc(test_array->length * sizeof *tests);
    size_t actual_count = 0;
    for (int test_ind = 0; test_ind < test_array->length; ++test_ind)
    {
        struct CMUnitTest *test = test_array->values[test_ind];
        if (strcasestr(test->name, filter) != NULL)
        {
            tests[actual_count] = *test;
            actual_count += 1;
        }
    }
    if (getenv("CMOCKA_XML_FILE") != NULL)
    {
        cmocka_set_message_output(CM_OUTPUT_XML);
    }
    int result = _cmocka_run_group_tests("all_tests", tests, actual_count, NULL, NULL);
    const char *err_msg = fto_err_get();
    if (err_msg != NULL && err_msg[0] != '\0')
    {
        printf("Error message: %s\n", err_msg);
    }
    return result;
}
