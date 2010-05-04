/*
 * main.cpp
 *  Sample codes for typeconv.hpp
 *
 *  written by janus_wel<janus.wel.3@gmail.com>
 *  This source code is in public domain, and has NO WARRANTY.
 * */

#include "../../header/typeconv.hpp"
#include <iostream>
#include <iomanip>
#include <locale>

using namespace std;
using namespace util::string;

// user-defined type
struct Complex {
    int r;  // real number
    int i;  // imaginary number
    Complex() {};
    Complex(int r, int i) : r(r), i(i) {};
};

// overloads of operator<< and operator>>
// Now, typeconv can affect user-defined type Complex
template<typename T>
basic_ostream<T>& operator<<(basic_ostream<T>& out, const Complex& comp) {
    return out << comp.r << '+' << comp.i << 'i';
}
template<typename T>
basic_istream<T>& operator>>(basic_istream<T>& in,  Complex& comp) {
    return in >> comp.r >> comp.i;
}

int main(const int argc, const char* argv[]) {
    // source
    Complex src(1, 3);

    // converter (classic locale)
    typeconverter conv;
    string converted = conv.strfrom(src);
    cout << left
        << setw(16) << "source"         << src << "\n"
        << setw(16) << "converted"      << converted << "\n"
        << setw(16) << "re-converted"   << conv.strto<Complex>(converted) << "\n"
        << endl;

    // converter (system locale)
    typeconverter sysconv(locale(""));
    cout
        << setw(16) << "classic locale" << conv.strfrom(1000000) << "\n"
        << setw(16) << "system locale"  << sysconv.strfrom(1000000) << "\n"
        << endl;

    // instead of old aton(1)
    if (argc >= 3) {
        int first = conv.strto<int>(argv[1]);
        int second = conv.strto<int>(argv[2]);

        cout
            << "argv[1] = " << first << "\n"
            << "argv[2] = " << second << "\n"
            << "argv[1] + argv[2] = " << first + second << "\n"
            << "argv[1] - argv[2] = " << first - second << "\n"
            << "argv[1] * argv[2] = " << first * second << "\n"
            << endl;
    }

    unsigned int n = 255;
    double d = 0.12345678;
    conv << std::fixed << std::setprecision(3) << std::hex;
    cout
        << n << " -> " << conv.strfrom(n) << "\n"
        << d << " -> " << conv.strfrom(d) << "\n"
        << endl;

    return 0;
}

