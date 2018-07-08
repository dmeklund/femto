#include "femto/util.h"

#include <stdio.h>
#include "quad/test_gauss.h"

static void print_help()
{
    printf("help\n");
}

extern int main(int argc, char *argv[])
{
    struct FtoArray *test_array = fto_array_new();
    if (argc > 2)
    {
        print_help();
        exit(1);
    }

}
