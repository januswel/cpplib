/*
 * main.cpp
 *  sample codes for cast.hpp
 *
 *  Copyright (C) 2010 janus_wel<janus.wel.3@gmail.com>
 *  see LICENSE for redistributing, modifying, and so on.
 * */

#include "../../header/cast.hpp"
#include <iostream>

using namespace std;
using namespace util::cast;

int main(void) {
    int i = 'a';
    const char* c = pointer_cast<char*>(&i);
    const int* const j = constpointer_cast<const int*>(c);

    cout
        <<  i << "\n"
        << *c << "\n"
        << *j << "\n"
        << endl;

    return 0;
}

