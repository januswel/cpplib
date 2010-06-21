/*
 * main.cpp
 *  Sample codes for dlogger.hpp
 *
 *  In order to disable, define the symbol NDEBUG:
 *
 *      > g++ -Wall --pedantic -DNDEBUG main.cpp
 *      > cl /EHsc /W4 /DNDEBUG main.cpp
 *
 *  Copyright (C) 2010 janus_wel<janus.wel.3@gmail.com>
 *  see LICENSE for redistributing, modifying, and so on.
 * */

#include <iostream>
#include "../../header/dlogger.hpp"

using namespace std;

int main(const int argc, const char* const argv[]) {
    if (argc >= 2) {
        DBGLOG("argc=" << argc);
        DBGLOG(argv[1]);

        std::cout << argv[1] << std::endl;
    }
    else {
        WDBGLOG(L"no parameters");

        std::cout << "no parameters" << std::endl;
    }

    return 0;
}

