/*
 * main.cpp
 *  sample codes for math.hpp
 *
 *  Copyright (C) 2010 janus_wel<janus.wel.3@gmail.com>
 *  see LICENSE for redistributing, modifying, and so on.
 * */

#include <iostream>
#include "../../header/math.hpp"

int main(void) {
    std::cout << std::fixed
        << util::math::round_nth(3.141592) << "\n"
        << util::math::round_nth(3.141592, 3) << "\n"
        << util::math::round_nth(-273.15) << "\n"
        << util::math::round_nth(299792458, -4) << "\n"
        << util::math::round_even_nth(0.5) << "\n"
        << util::math::round_even_nth(1.5) << "\n"
        << util::math::round_even_nth(-0.5) << "\n"
        << util::math::round_even_nth(-1.5) << "\n"
        << util::math::round_even_nth(1.23456789, 4) << "\n"
        << util::math::round_even_nth(-1.23456789, 4) << "\n"
        << util::math::round_even_nth(123456789, -4) << "\n"
        << util::math::round_even_nth(-123456789, -4) << "\n"
        << util::math::round_even_nth(3.141592, 3) << "\n"
        << util::math::round_even_nth(3.141592) << "\n"
        << util::math::round_even_nth(-273.15) << "\n"
        << util::math::round_even_nth(299792458, -4) << "\n"
        << std::endl;

    return 0;
}

