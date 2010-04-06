/*
 * main.cpp
 *  Sample codes for nwconv.hpp
 *
 *  written by janus_wel<janus.wel.3@gmail.com>
 *  This source code is in public domain, and has NO WARRANTY.
 * */

#include <iostream>
#include <string>

#include "../../header/nwconv.hpp"
#include "../../header/wexcept.hpp"

using namespace std;
using namespace util::string;
using namespace util::exception;

int main(const int argc, const char* const argv[]) {
    try {
        nwconv conv;

        for (int i = 0; i < argc; ++i) {
            // convert an argument narrow to wide
            wstring arg = conv.ntow(argv[i]);
            // discard a first character
            arg.erase(0, 1);
            // convert it wide to narrow and print
            cout << conv.wton(arg) << endl;
        }
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

