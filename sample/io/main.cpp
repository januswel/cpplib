/*
 * main.cpp
 *  sample codes for io.hpp
 *
 *  Copyright (C) 2010 janus_wel<janus.wel.3@gmail.com>
 *  see LICENSE for redistributing, modifying, and so on.
 * */

#include "../../header/io.hpp"

#include <iostream>

int main(void) {
    // always print something to console!!
    if (util::io::is_redirected()) {
        std::cout.rdbuf(std::cerr.rdbuf());
    }

    std::cout << "annoying message" << std::endl;

    return 0;
}

