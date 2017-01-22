#include <iostream>
#include <cstdlib>

int main(void)
{
    std::cout << "====================================================="
              << std::endl;
    std::cout << "This program calculates average speed of motorcycler."
              << std::endl
              << "Given values: S1, S2, t1, t2"
              << std::endl
              << "|     t1     |    t2   | " << std::endl
              << "A ---------- B ------- C " << std::endl
              << "|     S1     |    S2   | " << std::endl;
    std::cout << "===================================================="
              <<  std::endl; 
    float S1, S2, t1, t2;
    std::cout << "Enter S1 = ";
    std::cin  >> S1;
    std::cout << "Enter S2 = ";
    std::cin  >> S2;
    std::cout << "Enter t1 = ";
    std::cin  >> t1;
    std::cout << "Enter t2 = ";
    std::cin  >> t2;
    float average_speed = (S1 + S2)/(t1 + t2);
    std::cout << "Average speed = " << average_speed
              << " m/s" << std::endl;
    return EXIT_SUCCESS;
}
