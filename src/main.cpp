#include "main.h"

#include <iostream>
#include <string>

int main() {
    std::cout << "Please, two integer number: \n";
    bool isFail = false;
    int numerator, denominator;
    do {
        isFail = false;
        std::string nstr, dstr;

        std::cin >> nstr;
        std::cin >> dstr;
        try {
            numerator = std::stoi(nstr);
            denominator = std::stoi(dstr);
        } 
        catch (std::exception e) {
            isFail = true;
            std::cout << "\n\nYou enter uncorrect numbers. Please try again: \n";
        }
    } while (isFail);

    try{
        Fraction frac(numerator, denominator);
    }
    catch (std::exception e){
        std::cout << "В качестве denominator был передан 0 " << std::endl;
    }
}
