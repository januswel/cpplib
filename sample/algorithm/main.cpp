/*
 * main.cpp
 *  sample codes for algorithm.hpp
 *
 *  written by janus_wel<janus.wel.3@gmail.com>
 *  This source code is in public domain, and has NO WARRANTY.
 * */

#include "../../header/algorithm.hpp"

#include <algorithm>
#include <cstring>
#include <functional>
#include <iostream>
#include <list>
#include <locale>
#include <string>
#include <vector>

// We can't use std::mem_fun_ref() to compose functions for a non-static member
// function in g++ 4.3.3, so it is needed to wrap it.
#ifndef _MSC_VER
inline size_t length(const std::string& str) { return str.size(); }
#endif

template<typename Char>
inline bool is(const std::ctype_base::mask mask, const Char c) {
    static const std::ctype<Char>& ctype =
        std::use_facet<std::ctype<Char> >(std::locale::classic());
    return ctype.is(mask, c);
}

template<typename Char>
class has {
    private:
        const std::ctype_base::mask mask;
        has& operator=(const has&);

    public:
        has(const std::ctype_base::mask mask) : mask(mask) {}
        bool operator()(const std::basic_string<Char>& str) {
            return std::find_if(str.begin(), str.end(),
                    std::bind1st(std::ptr_fun(&(is<Char>)), mask))
                != str.end();
        }
};

int main(const int argc, const char* const argv[]) {
    typedef std::vector<std::string>    string_array_type;
    //typedef std::list<std::string>  string_array_type;

    std::cout << "argv\n";
    std::ostream_iterator<const char* const> coitr =
        util::algorithm::print<const char* const>(argv, argv + argc, std::cout, "\n");
// Above expression is equivalent to following codes
//    std::ostream_iterator<const char* const> coitr(std::cout, "\n");
//    util::algorithm::print(argv, argv + argc, coitr);
    std::cout << std::endl;

    string_array_type params(argc);
    std::copy(argv, argv + argc, params.begin());

    std::cout << "packed argv as std::string\n";
    std::ostream_iterator<std::string> soitr =
        util::algorithm::print<std::string>(params.begin(), params.end(), std::cout, "\n");
    std::cout << std::endl;

    std::cout << "lengths of argv\n";
    util::algorithm::print_op<size_t>(argv, argv + argc, std::cout, "\n", std::strlen);
    std::cout << std::endl;

    std::cout << "lengths of packed argv as std::string\n";
#ifdef _MSC_VER
    // In g++ 4.3.3, this expression can't be passed.
    util::algorithm::print_op<size_t>(
            params.begin(), params.end(),
            std::cout, "\n", std::mem_fun_ref(&(std::string::size)));
#else
    util::algorithm::print_op<size_t>(
            params.begin(), params.end(),
            std::cout, "\n", length);
#endif
    std::cout << std::endl;

    std::cout << "argv that has any digits\n";
    util::algorithm::print_if(argv, argv + argc, coitr, has<char>(std::ctype_base::digit));
    std::cout << std::endl;

    std::cout << "packed argv as std::string that has any punctuations\n";
    util::algorithm::print_if(params.begin(), params.end(), soitr, has<char>(std::ctype_base::punct));
    std::cout << std::endl;

// emulation of "grep" statement in Perl
//    string_array_type greped(std::count_if(params.begin(), params.end(), has_digit<char>()));
//    util::algorithm::copy_if(params.begin(), params.end(), greped.begin(), has_digit<char>());
//    util::algorithm::print(greped.begin(), greped.end(), soitr);

    return 0;
}

