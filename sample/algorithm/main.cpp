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
#include <fstream>

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

struct o {
    o(void) { std::cout << "o(void)\n"; }
    ~o(void) { std::cout << "~o(void)\n"; }
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
    util::algorithm::print_op<std::string::size_type>(
            params.begin(), params.end(),
            std::cout, "\n", std::mem_fun_ref(&std::string::size));
    std::cout << std::endl;

    std::cout << "argv that has any digits\n";
    util::algorithm::print_if(argv, argv + argc, coitr, has<char>(std::ctype_base::digit));
    std::cout << std::endl;

    std::cout << "packed argv as std::string that has any punctuations\n";
    util::algorithm::print_if(params.begin(), params.end(), soitr, has<char>(std::ctype_base::punct));
    std::cout << std::endl;

    std::cout << "grep std::strings that have any alphabets from packed argv\n";
    string_array_type greped;
    util::algorithm::grep(params.begin(), params.end(), greped, has<char>(std::ctype_base::alpha));
    util::algorithm::print(greped.begin(), greped.end(), soitr);

    std::cout << "grep std::strings that have any alphabets from packed argv\n";
    std::list<std::string> lgreped;
    util::algorithm::grep(params.begin(), params.end(), lgreped, has<char>(std::ctype_base::alpha));
    util::algorithm::print(lgreped.begin(), lgreped.end(), soitr);

    std::cout << "deleting objects by util::algorithm::sweeper\n";
    std::vector<o*> os;
    for (int i = 0; i < 3; ++i) os.push_back(new o);
    std::for_each(os.begin(), os.end(), util::algorithm::sweeper());

    /*  to get accurate outputs, execute as following
     *
     *      > main foo bar buz > nul
     *      > ./a.out foo bar buz > /dev/null
     * */
    std::cerr << "with progress\n";
    util::algorithm::print_op<std::string>(
            params.begin(), params.end(),
            std::cout, "\n",
            util::algorithm::basic_progress<std::string, unsigned int>(
                std::cerr, 1, params.size()));

    std::cerr << std::endl;

    return 0;
}

