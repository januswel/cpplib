// endian.cpp

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
    __int32 n = 0x12345678;

    // non-destructive but slow
    __int32 r = reverse(n);
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
