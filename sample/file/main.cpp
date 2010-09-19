/*
 * main.cpp
 *  sample codes for file.hpp
 *
 *  written by janus_wel<janus.wel.3@gmail.com>
 *  This source code is in public domain, and has NO WARRANTY.
 * */

#include "../../header/file.hpp"

#include <iomanip>
#include <iostream>
#include <fstream>

int main(const int argc, const char* const argv[]) {
    if (argc < 2) {
        if (util::file::is_file(std::cin)) {
            std::cout
                << "file: "
                << util::file::size(std::cin)
                << std::endl;
        }
        else {
            std::cout
                << "stdin: "
                << util::file::size(std::cin)
                << std::endl;
        }

        return 0;
    }

    std::ifstream fin(argv[1]);
    if (fin) {
        std::cout
            << argv[1] << ": "
            << util::file::size(fin)
            << std::endl;
    }

    return 0;
}

