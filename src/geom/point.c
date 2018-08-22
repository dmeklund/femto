#include "femto/util.h"
#include "femto/geom/point.h"

#include <stdio.h>


extern struct Fto2DPoint fto_2dpoint_new(double x, double y)
{
    struct Fto2DPoint *pt = fto_malloc(sizeof *pt);
    *pt = (struct Fto2DPoint){
        .x = x,
        .y = y
    };
    return *pt;
}


extern enum FtoError fto_2dpoint_print(const struct Fto2DPoint point)
{
    printf("(%g, %g)", point.x, point.y);
    return FTO_OK;
}
