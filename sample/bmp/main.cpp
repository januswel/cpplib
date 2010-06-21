/*
 * main.cpp
 *  sample codes for bmp.hpp
 *
 *  Copyright (C) 2010 janus_wel<janus.wel.3@gmail.com>
 *  see LICENSE for redistributing, modifying, and so on.
 * */

#include <fstream>
#include <iostream>

#include "../../header/bmp.hpp"
#include "../../header/io.hpp"

int main(const int argc, const char* const argv[]) {
    if (argc < 2) {
        // creating
        format::windows_bitmap::elements_type outelements = {1, 1};
        format::windows_bitmap::header_type outheader(outelements);
        if (!outheader.validate()) {
            std::cerr
                << "It seems something is wrong\n"
                << std::endl;
            return 1;
        }
        util::io::set_stdout_binary();
        std::cout
            << outheader
            << static_cast<char>(-1)
            << static_cast<char>(0)
            << static_cast<char>(0)
            << static_cast<char>(0);

        return 0;
    }

    // validating
    std::ifstream bin(argv[1], std::ios::binary);
    if (!bin.good()) {
        std::cerr
            << "bad file: " << argv[1] << "\n"
            << std::endl;
        return 1;
    }

    format::windows_bitmap::header_type inheader;
    bin >> inheader;
    if (!inheader.validate() || !inheader.validate(bin)) {
        std::cerr
            << "bad bmp file: " << argv[1] << "\n"
            << std::endl;
        return 1;
    }

    std::cout
        << "valid bmp file: " << argv[1] << "\n"
        << std::endl;

    return 0;
}

