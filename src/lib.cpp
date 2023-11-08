#include "lib.h"

#include <iostream>

Fraction::Fraction(int numerator, int denominator) {
    if (denominator == 0) {
        throw std::runtime_error("denumenator equal 0!");
    }
    this->numerator = numerator;
    this->denominator = denominator;
}
