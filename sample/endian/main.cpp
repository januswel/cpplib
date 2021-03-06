/*
 * main.cpp
 *  Sample codes for endian.hpp
 *
 *  Copyright (C) 2010 janus_wel<janus.wel.3@gmail.com>
 *  see LICENSE for redistributing, modifying, and so on.
 * */

#include <iostream>
#include "../../header/endian.hpp"
#include "../../header/cast.hpp"

using namespace std;
using namespace util::endian;
using namespace util::cast;

int main(void) {
    cout
        << "An endian of this machine is "
        << (is_little() ? "little" : "big") << "."
        << endl;

    // data
    int32_t n = 0x12345678;

    // non-destructive but slow
    int32_t r = reverse(n);
    cout.write(pointer_cast<char*>(&n), sizeof(n));
    cout.write(pointer_cast<char*>(&r), sizeof(r));
    cout << endl;

    // fast but destructive
    cout.write(pointer_cast<char*>(&n), sizeof(n));
    fast_reverse(n);
    cout.write(pointer_cast<char*>(&n), sizeof(n));
    cout << endl;

    return 0;
}
