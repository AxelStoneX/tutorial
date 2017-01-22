#ifndef __TYPES_H__
#define __TYPES_H__

struct coordinates_t
{
    float x;
    float y;

    coordinates_t(float _x, float _y)
        :x(_x)
        ,y(_y)
    {};
};

#endif
