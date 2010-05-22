/*
 * main.cpp
 *  sample codes for io.hpp
 *
 *  written by janus_wel<janus.wel.3@gmail.com>
 *  This source code is in public domain, and has NO WARRANTY.
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

