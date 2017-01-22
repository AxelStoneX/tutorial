#include <iostream>
#include <iomanip>
#include <cstdlib>
#include "thrown_body.h"

int verbose = 1;

int main(void)
{
    std::cout << "====================================================="
              << std::endl;
    std::cout << "This program calculates coordinates" << std::endl
              << "of body, thrown at angle, at given time" << std::endl;
    std::cout << "====================================================="
              << std::endl;

    std::cout << "Enter speed (from " << thrown_body_t::MIN_SPEED
              << " to " << thrown_body_t::MAX_SPEED << ") = ";
    float speed;
    std::cin >> speed;

    std::cout << "Enter angle (from " << thrown_body_t::MIN_ANGLE
              << " to " << thrown_body_t::MAX_ANGLE << ")  = ";
    float angle;
    std::cin >> angle;
    
    thrown_body_t thrown_body(speed, angle, verbose);
    std::cout << "Enter moment of time" << "        = ";
    float time;
    std::cin >> time;
    std::cout << "Results:" << std::endl;
    coordinates_t coordinates = thrown_body.get_coordinates(time);
    std::cout << "x = " << std::setw(6) << coordinates.x
                        << " m" << std::endl
              << "y = " << std::setw(6) << coordinates.y 
                        << " m" << std::endl;
    return EXIT_SUCCESS;
}

