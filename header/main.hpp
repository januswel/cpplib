/*
 * main.hpp
 *  remarks
 *
 *  written by janus_wel<janus.wel.3@gmail.com>
 *  This source code is in public domain, and has NO WARRANTY.
 * */

#ifndef MAIN_HPP
#define MAIN_HPP

#include <iterator>
#include <string>
#include <vector>

#include "nwconv.hpp"
#include "getopt.hpp"
#include "wexcept.hpp"

namespace util {
    namespace main {
        /*
         *  a class to start a program
         * */
        template<typename charT> class basic_main;

        template<> class basic_main<char> {
            public:
                typedef char                                char_t;
                typedef std::basic_string<char_t>           string_t;
                typedef std::vector<string_t>               string_array_t;
                typedef string_array_t::iterator            string_array_iterator;
                typedef string_array_t::const_iterator      string_array_const_iterator;
                typedef std::iterator_traits<string_array_const_iterator>::difference_type string_array_difference_t;
                typedef util::getopt::basic_option<char_t>  option_t;

            protected:
                typedef util::getopt::basic_getopt<char_t>              basic_getopt_t;

            protected:
                string_t program_name;
                string_array_t parameters;

                class getopt_t : public basic_getopt_t {
                    private:
                        string_array_t state;

                    public:
                        const string_array_t& nonopt_parameters(void) { return state; }

                    protected:
                        // non-option handler
                        unsigned int handle_nonopt(const parameters_t& params) {
                            state.push_back(*(params.current()));
                            return 1;
                        }
                } getopt;

            public:
                const string_array_t& nonopt_parameters(void) {
                    return getopt.nonopt_parameters();
                }

            public:
                virtual int start(void) = 0;

            public:
                void pack_parameters(const int argc, const char* const argv[]) {
                    program_name = argv[0];
                    parameters.reserve(argc);
                    for (int i = 1; i < argc; ++i) {
                        parameters.push_back(argv[i]);
                    }
                }

                unsigned int analyze(void) {
                    return getopt.analyze(parameters);
                }

                // destructor
                virtual ~basic_main(void) {}
        };

        template<> class basic_main<wchar_t> {
            public:
                typedef wchar_t                             char_t;
                typedef std::basic_string<char_t>           string_t;
                typedef std::vector<string_t>               string_array_t;
                typedef string_array_t::iterator            string_array_iterator;
                typedef string_array_t::const_iterator      string_array_const_iterator;
                typedef std::iterator_traits<string_array_const_iterator>::difference_type string_array_difference_t;
                typedef util::getopt::basic_option<char_t>  option_t;

            protected:
                typedef util::getopt::basic_getopt<char_t>              basic_getopt_t;

            protected:
                string_t program_name;
                string_array_t parameters;

                util::string::nwconv nwconv;

                class getopt_t : public basic_getopt_t {
                    private:
                        string_array_t state;

                    public:
                        const string_array_t& nonopt_parameters(void) { return state; }

                    protected:
                        // non-option handler
                        unsigned int handle_nonopt(const parameters_t& params) {
                            state.push_back(*(params.current()));
                            return 1;
                        }
                } getopt;

            public:
                const string_array_t& nonopt_parameters(void) {
                    return getopt.nonopt_parameters();
                }

            public:
                virtual int start(void) = 0;

            public:
                void pack_parameters(const int argc, const char* const argv[]) {
                    program_name = nwconv.ntow(argv[0]);
                    parameters.reserve(argc);
                    for (int i = 1; i < argc; ++i) {
                        parameters.push_back(nwconv.ntow(argv[i]));
                    }
                }

                unsigned int analyze(void) {
                    return getopt.analyze(parameters);
                }

                // destructor
                virtual ~basic_main(void) {}
        };

        // for convenience
        typedef basic_main<char>    main;
        typedef basic_main<wchar_t> wmain;
    }
}

#endif // MAIN_HPP

