/*
 * main.hpp
 *  classes to start a program
 *
 *  written by janus_wel<janus.wel.3@gmail.com>
 *  This source code is in public domain, and has NO WARRANTY.
 * */

#ifndef MAIN_HPP
#define MAIN_HPP

#include <algorithm>
#include <functional>
#include <iterator>
#include <string>
#include <vector>

#include <cstdio>       // for isatty(1) or _fileno(1)

#ifdef _MSC_VER
#   include <io.h>      // for _isatty(1), _setmode(2)
#   include <fcntl.h>   // for _setmode(2)
#else
#   include <unistd.h>  // for fileno(1)
#endif

#include "getopt.hpp"
#include "nwconv.hpp"
#include "wexcept.hpp"

namespace util {
    namespace main {
        /*
         *  a class to start a program
         * */
        template<typename charT> class basic_main;

        template<> class basic_main<char>
        : public util::getopt::basic_getopt<char> {
            public:
                typedef char                                    char_type;
                typedef std::basic_string<char_type>            string_type;
                typedef std::vector<string_type>                string_array_type;
                typedef util::getopt::basic_option<char_type>   option_type;

            protected:
                // member variables
                string_type program_name;
                string_array_type parameters;
                string_array_type nonopt_parameters;

            public:
                // virtual functions to be implemented at sub class
                virtual int start(void) = 0;

            public:
                // destructor
                virtual ~basic_main(void) {}

                // pack parameters as std::string and analyze options
                unsigned int do_parameters(const int argc, const char* const argv[]) {
                    program_name = argv[0];
                    parameters.resize(argc - 1);
                    parameters.reserve(argc - 1);
                    std::copy(argv + 1, argv + argc, parameters.begin());

                    nonopt_parameters.reserve(argc - 1);
                    return this->analyze(parameters);
                }

            protected:
                // implementations for member functions from super class
                unsigned int handle_nonopt(const parameters_type& params) {
                    nonopt_parameters.push_back(*(params.current()));
                    return 1;
                }
        };

        template<> class basic_main<wchar_t>
        : public util::getopt::basic_getopt<wchar_t> {
            public:
                typedef wchar_t                                 char_type;
                typedef std::basic_string<char_type>            string_type;
                typedef std::vector<string_type>                string_array_type;
                typedef util::getopt::basic_option<char_type>   option_type;

            protected:
                // member variables
                string_type program_name;
                string_array_type parameters;
                string_array_type nonopt_parameters;
                util::string::nwconv nwconv;

            public:
                // virtual functions to be implemented at sub class
                virtual int start(void) = 0;

            public:
                // destructor
                virtual ~basic_main(void) {}

                // pack parameters as std::wstring and analyze options
                unsigned int do_parameters(const int argc, const char* const argv[]) {
                    program_name = nwconv.ntow(argv[0]);
                    parameters.reserve(argc);
                    for (int i = 1; i < argc; ++i) {
                        parameters.push_back(nwconv.ntow(argv[i]));
                    }

                    nonopt_parameters.reserve(argc - 1);
                    return this->analyze(parameters);
                }

            protected:
                // implementations for member functions from super class
                unsigned int handle_nonopt(const parameters_type& params) {
                    nonopt_parameters.push_back(*(params.current()));
                    return 1;
                }
        };

        // for convenience
        typedef basic_main<char>    main;
        typedef basic_main<wchar_t> wmain;

        // utilities
        inline bool is_redirected(void) {
#ifdef _MSC_VER
            return _isatty(_fileno(stdout)) == 0;
#else
            return isatty(fileno(stdout)) == 0;
#endif
        }

        inline void set_stdout_binary(void) {
#ifdef _MSC_VER
            _setmode(_fileno(stdout), _O_BINARY);
#endif
        }
    }
}

#endif // MAIN_HPP

