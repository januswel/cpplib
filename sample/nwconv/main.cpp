/*
 * main.cpp
 *  Sample codes for nwconv.hpp
 *
 *  Copyright (C) 2010 janus_wel<janus.wel.3@gmail.com>
 *  see LICENSE for redistributing, modifying, and so on.
 * */

#include <iostream>
#include <locale>
#include <string>

#include "../../header/algorithm.hpp"
#include "../../header/nwconv.hpp"
#include "../../header/wexcept.hpp"

using namespace std;
using namespace util::algorithm;
using namespace util::string;
using namespace util::exception;

string second_or_later(const char* const src) {
    static nwconv conv(std::locale(""));

    // convert an argument narrow to wide
    wstring arg = conv.ntow(src);
    // discard a first character
    arg.erase(0, 1);
    // convert it wide to narrow and print
    return (arg.size() > 1) ? conv.wton(arg) : string("");
}

int main(const int argc, const char* const argv[]) {
    try {
        print_op<string>(argv, argv + argc, std::cout, "\n", second_or_later);
    }
    catch (exception& ex) {
        // for nwconv::ntow(1)
        cerr << ex.what() << endl;
    }
    catch (wexception& ex) {
        // for nwconv::wton(1)
        wcerr << ex.what() << endl;
    }

    return 0;
}

