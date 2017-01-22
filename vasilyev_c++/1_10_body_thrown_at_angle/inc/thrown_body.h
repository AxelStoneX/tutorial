#ifndef __THROWN_BODY_T__
#define __THROWN_BODY_T__

#include <iostream>
#include <cmath>
#include "types.h"

class thrown_body_t
{
public:
    enum limits_t
    {
        //--------------------
        MAX_SPEED = 100,
        DEF_SPEED = 10,
        MIN_SPEED = 0,
        //--------------------
        MAX_ANGLE = 90,
        DEF_ANGLE = 45,
        MIN_ANGLE = 0,
    };
    //----------------------------------
    static const float MATH_PI = 3.1415F;
    static const float PHYS_G  = 9.807F;
    //----------------------------------
    thrown_body_t(double speed, double angle, int verbose = 0);
    ~thrown_body_t()
    {};
    coordinates_t get_coordinates(double time);
private:
    double speed;
    double angle_deg;
    double angle_rad;
    double max_time;
    int    verbose;
};

#endif
