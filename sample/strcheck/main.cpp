/*
 * main.cpp
 *  sample codes for strcheck.hpp
 *
 *  written by janus_wel<janus.wel.3@gmail.com>
 *  This source code is in public domain, and has NO WARRANTY.
 * */

#include "../../header/strcheck.hpp"
#include "../../header/nwconv.hpp"

#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <vector>

int main(const int argc, const char* const argv[]) {
    std::cout << std::boolalpha;

    util::string::check checker;
    typedef bool (util::string::check::*char_arg)(const char* const) const;

    std::cout << "integer numbers: \n";
    std::remove_copy_if(argv, argv + argc,
            std::ostream_iterator<const char*>(std::cout, "\n"),
            std::not1(std::bind1st(
                std::mem_fun_ref(
                    static_cast<char_arg>(&util::string::check::is_integer)),
                checker)));
    std::cout << std::endl;

    std::cout << "decimal numbers: \n";
    std::remove_copy_if(argv, argv + argc,
            std::ostream_iterator<const char*>(std::cout, "\n"),
            std::not1(std::bind1st(
                std::mem_fun_ref(
                    static_cast<char_arg>(&util::string::check::is_decimal)),
                checker)));
    std::cout << std::endl;

    std::cout << "real numbers: \n";
    std::remove_copy_if(argv, argv + argc,
            std::ostream_iterator<const char*>(std::cout, "\n"),
            std::not1(std::bind1st(
                std::mem_fun_ref(
                    static_cast<char_arg>(&util::string::check::is_real)),
                checker)));
    std::cout << std::endl;

    // We must denote procedures in the form of classical iteration by using
    // "for" statements, because the STL's constraint that the combination of
    // std::bind1st and the member function that expects an argument that is a
    // reference of any types (e.g.: std::string&) is unusable.
    std::wcout << "\nin wchar_t\n";
    util::string::wcheck wchecker;
    std::vector<std::wstring> wss;
    util::string::nwconv conv;
    for (int i = 0; i < argc; ++i) wss.push_back(conv.ntow(argv[i]));
    for (std::vector<std::wstring>::const_iterator itr = wss.begin();
            itr != wss.end(); ++itr) {
        std::wcout << *itr << ":\n";
        if (wchecker.is_integer(*itr)) std::wcout << L"integer\n";
        if (wchecker.is_decimal(*itr)) std::wcout << L"decimal\n";
        if (wchecker.is_real(*itr)) std::wcout << L"real\n";
        std::cout << std::endl;
    }
    std::cout << std::endl;

    return 0;
}

