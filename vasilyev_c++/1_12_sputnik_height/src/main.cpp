#include <iostream>
#include <cmath>

const double G  = 6.672E-11;
const double M  = 5.96E24;
const double R  = 6.37E6;
const double PI = 3.1415;

int main(void)
{
    std::cout << "===================================================="
              << std::endl;
    std::cout << "This program calculates height of sputnik orbit"
              << std::endl
              << "with given period of circulation (T)"
              << std::endl;
    std::cout << "===================================================="
              << std::endl;

    double T;
    std::cout << "Enter T (hours) = ";
    std::cin >> T;
    T = T *3600;
    double H;
    H = pow((G*M*T*T)/(4*PI*PI), (double) 1/3) - R;
    std::cout << "Results:" << std::endl;
    H /= 1000;
    std::cout << "H = " << H << " km" << std::endl;
    return 0;  
}
