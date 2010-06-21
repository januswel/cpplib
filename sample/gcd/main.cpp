/*
 * main.cpp
 *  Sample codes for gcd.hpp
 *
 *  Copyright (C) 2010 janus_wel<janus.wel.3@gmail.com>
 *  see LICENSE for redistributing, modifying, and so on.
 * */

#include "../../header/gcd.hpp"
#include "../../header/random.hpp"
#include <iostream>
#include <limits>

using namespace std;
using namespace util::math;

int main(void) {
    util::math::random rand;

    unsigned char max08 = numeric_limits<unsigned char>::max();
    unsigned char a08 = static_cast<unsigned char>(rand() * max08);
    unsigned char b08 = static_cast<unsigned char>(rand() * max08);
    cout
        << "gcd(" << static_cast<int>(a08)
        << ", " << static_cast<int>(b08)
        << ") = " << calc_gcd(a08, b08) << endl;

    unsigned short int max16 = numeric_limits<unsigned short int>::max();
    unsigned short int a16 = static_cast<unsigned short int>(rand() * max16);
    unsigned short int b16 = static_cast<unsigned short int>(rand() * max16);
    cout
        << "gcd(" << a16
        << ", " << b16
        << ") = " << calc_gcd(a16, b16) << endl;

    unsigned int max32 = numeric_limits<unsigned int>::max();
    unsigned int a32 = static_cast<unsigned int>(rand() * max32);
    unsigned int b32 = static_cast<unsigned int>(rand() * max32);
    cout
        << "gcd(" << a32
        << ", " << b32
        << ") = " << calc_gcd(a32, b32) << endl;

    unsigned long int max64 = numeric_limits<unsigned long int>::max();
    unsigned int a64 = static_cast<unsigned long int>(rand() * max64);
    unsigned int b64 = static_cast<unsigned long int>(rand() * max64);
    cout
        << "gcd(" << a64
        << ", " << b64
        << ") = " << calc_gcd(a64, b64) << endl;

/*
 *  Following operations can't be defined

    calc_gcd(bool,        bool)
    calc_gcd(float,       float)
    calc_gcd(double,      double)
    calc_gcd(long double, long double)
*/

    return 0;
}

