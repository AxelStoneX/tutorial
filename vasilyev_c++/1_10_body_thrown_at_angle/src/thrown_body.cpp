#include "thrown_body.h"

thrown_body_t::thrown_body_t(double _speed, double _angle, int _verbose)
    : verbose(_verbose)
{
    if(_speed < MAX_SPEED && _speed > MIN_SPEED)
        speed = _speed;
    else
    { 
        speed = DEF_SPEED;
        if(verbose > 0)
        {
            std::cout << "Speed value exceed limits" << std::endl
                      << "Default value of " << DEF_SPEED
                      << " has been set" << std::endl;
        }
    }

    if(_angle < MAX_ANGLE && _angle > MIN_ANGLE)
        angle_deg = _angle;
    else
    {
        angle_deg = DEF_ANGLE;
        std::cout << "Angle value exceed limits" << std::endl
                  << "Default value of " << DEF_ANGLE
                  << " has been set" << std::endl;
    }

    angle_rad = (MATH_PI * angle_deg) / 180.0F;
    max_time  = 2 * speed * sin(angle_rad) / PHYS_G;
}

coordinates_t thrown_body_t::get_coordinates(double time)
{
    if(time > max_time)
    {
        if(verbose > 0)
            std::cout << "Thrown body has landed" << std::endl;
        double x = speed * cos(angle_rad) * max_time;
        double y = speed * sin(angle_rad) * max_time
                   - (PHYS_G * (max_time) * (max_time) / 2);
        return coordinates_t(x, y);
    }

    double x = speed * cos(angle_rad) * time;
    double y = speed * sin(angle_rad) * time
               - ((PHYS_G * time * time) / 2);
    return coordinates_t(x, y);
}
