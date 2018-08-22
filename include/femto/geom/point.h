#ifndef FTO_GEOM_POINT_H
#define FTO_GEOM_POINT_H

struct Fto2DPoint
{
    double x;
    double y;
};

extern struct Fto2DPoint fto_2dpoint_new(double x, double y);

extern enum FtoError fto_2dpoint_print(const struct Fto2DPoint point);

#endif // FTO_GEOM_POINT_H
