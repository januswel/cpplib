/*
 * main.cpp
 *  Sample codes for nwconv.hpp
 *
 *  written by janus_wel<janus.wel.3@gmail.com>
 *  This source code is in public domain, and has NO WARRANTY.
 * */

#include <iostream>
#include <string>

#include "../../header/algorithm.hpp"
#include "../../header/nwconv.hpp"
#include "../../header/wexcept.hpp"

using namespace std;
using namespace util::algorithm;
using namespace util::string;
using namespace util::exception;

string second_or_later(const char* const src) {
    static nwconv conv;

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

