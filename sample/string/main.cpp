/*
 * main.cpp
 *  sample codes for string.hpp
 *
 *  Copyright (C) 2010 janus_wel<janus.wel.3@gmail.com>
 *  see LICENSE for redistributing, modifying, and so on.
 * */

#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <iterator>
#include <functional>

#include "../../header/string.hpp"

int main(const int argc, const char* const argv[]) {
    std::vector<std::string> ss(argc);
    std::copy(argv, argv + argc, ss.begin());

    for (   std::vector<std::string>::iterator itr = ss.begin();
            itr != ss.end(); ++itr) {
        std::cout << util::string::count(*itr, "ai") << "\n";
    }
    std::cout << std::endl;

    return 0;
}

