/*
 * main.cpp
 *  Sample codes for wexcept.hpp
 *
 *  Copyright (C) 2010 janus_wel<janus.wel.3@gmail.com>
 *  see LICENSE for redistributing, modifying, and so on.
 * */

#include "../../header/wexcept.hpp"
#include "../../header/typeconv.hpp"
#include <iostream>
#include <locale>

using namespace std;
using namespace util::exception;
using namespace util::string;

int main(const int argc, const char* argv[]) {
    // use system locale
    std::locale::global(std::locale(""));
    typeconverter conv;

    try {
        if (argc < 3)
            throw wruntime_error(L"\u5b9f\u884c\u6642\u932f\u8aa4: Specify two numbers.");

        int first =  conv.strto<int>(argv[1]);
        int second = conv.strto<int>(argv[2]);
        if (second == 0)
            throw wruntime_error(L"\u5b9f\u884c\u6642\u932f\u8aa4: Zero divide.");
        else
            wcout << first / second << endl;
    }
    catch (wexception& ex) {
        wcerr << ex.what() << endl;
    }

    return 0;
}

