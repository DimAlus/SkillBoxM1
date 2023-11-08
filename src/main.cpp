#include <iostream>
#include "main.h"

int main()
{
    Vector v1(1, 2, 3), v2(4, 5, 6), v3;
    std::cout << "Please, enter vector: \n";
    std::cin >> v3;
    std::cout << "\nYou enter: " << v3 << "\n";
    std::cout << "Length: " << float(v3) << "\n";
    std::cout << v3 << " < " << v1 << ": " << (v3 < v1) << "\n";
    std::cout << v2 << " > " << v3 << ": " << (v2 > v3) << "\n";
    std::cout << v2 << " * " << float(v3) << ": " << (v2 * float(v3)) << "\n";
}
