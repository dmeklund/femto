#ifndef FTO_GEOM_POINT_H
#define FTO_GEOM_POINT_H

struct Fto2DPoint
{
    double x;
    double y;
};

extern struct Fto2DPoint* fto_2dpoint_new(double x, double y);

#endif // FTO_GEOM_POINT_H