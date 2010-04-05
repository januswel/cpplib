/*
 * main.cpp
 *  Sample codes for elapsed.hpp
 *
 *  written by janus_wel<janus.wel.3@gmail.com>
 *  This source code is in public domain, and has NO WARRANTY.
 * */

#include "../../header/elapsed.hpp"
#include <iostream>

using namespace std;

int main(void) {
    util::time::elapsed elapsed;
    util::time::elapsed all;

    char i;
    cout << "Hit the enter key to measure time:";
    cin.get(i);

    cout << elapsed()  << " sec" << endl;

    elapsed.reset();

    cout << "One more:";
    cin.get(i);
    cout
        << elapsed()  << " sec\n"
        << all() << " sec for all\n"
        << endl;

    return 0;
}

