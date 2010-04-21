/*
 * getopt.hpp
 *  base classes to handle options that is specified as the command line
 *  parameters
 *
 *  written by janus_wel<janus.wel.3@gmail.com>
 *  This source code is in public domain, and has NO WARRANTY.
 * */

#ifndef GETOPT_HPP
#define GETOPT_HPP

#include "cor.hpp"

#include <stdexcept>
#include <string>
#include <vector>

namespace util {
    namespace getopt {
        /*
         *  a class to refer the parameters
         *  getters
         *      current()   returns the const_iterator that points the
         *                  parameter to be processed at that time.
         *      begin()     returns the const_iterator that points the start of
         *                  the array of parameters.
         *      end()       returns the const_iterator that points the end of
         *                  the array of parameters.
         *      operator()  returns array itself.
         *
         *  And this class has the member function advance(1) to change current
         *  subject forward, but this is called by the class basic_getopt
         *  automatically, so don't call this manually.
         *  */
        template<typename charT> class basic_parameters {
            public:
                typedef charT                                   char_t;
                typedef std::basic_string<char_t>               string_t;
                typedef std::vector<string_t>                   string_array_t;
                typedef typename string_array_t::const_iterator const_iterator;

            private:
                const string_array_t* mv_array;
                const_iterator mv_current;

            public:
                // constructor
                basic_parameters(string_array_t& array)
                    : mv_array(&array), mv_current(array.begin()) {}
                basic_parameters(string_array_t* array)
                    : mv_array(array), mv_current(array->begin()) {}

                // getters
                const const_iterator& current(void) const { return mv_current; }
                const const_iterator begin(void) const { return mv_array->begin(); }
                const const_iterator end(void) const { return mv_array->end(); }
                const string_array_t& operator()(void) const { return *mv_array; }

                // positioning
                void advance(unsigned int n) { std::advance(mv_current, n); }
        };

        /*
         *  classes to represent option's traits
         *  For Microsoft environment, you can define the new trait that has
         *  the prefix "/".
         * */
        template<typename charT> struct option_traits;
        template<> struct option_traits<char> {
            typedef char char_t;
            struct shortname {
                static const char_t* prefix(void) { return "-"; }
                static const unsigned int prefix_length = 1;
            };
            struct longname {
                static const char_t* prefix(void) { return "--"; }
                static const unsigned int prefix_length = 2;
            };
        };
        template<> struct option_traits<wchar_t> {
            typedef wchar_t char_t;
            struct shortname {
                static const char_t* prefix(void) { return L"-"; }
                static const unsigned int prefix_length = 1;
            };
            struct longname {
                static const char_t* prefix(void) { return L"--"; }
                static const unsigned int prefix_length = 2;
            };
        };

        /*
         *  a class to handle parameters as option name and its values
         *  A derived class from this processes a option.
         *  To use:
         *
         *      1. Define a class or struct that is derived from this class.
         *      2. Override the following member functions
         *
         *          - shortname(void) and/or longname(void)
         *              - return option names. shortname() should return one
         *                character as a matter of principle.
         *              - Overriding the only one of two functions is OK.
         *          - handle_params(const parameters_t&)
         *              - handles the current parameter (or more parameters).
         *              - This member function must return a number of
         *                processed parameters. e.g.: the option class that
         *                handles "-v" means "show version information" will
         *                eat up "-v" itself and return 1, the option class
         *                that handles "-s <size>" means "set size to <size>"
         *                will eat up "-v" and "<size>", and return 2.
         * */
        template<typename charT, typename traitsT = option_traits<charT> >
        class basic_option
            : public pattern::cor::basic_handler<unsigned int, basic_parameters<charT> > {
            public:
                typedef charT                       char_t;
                typedef traitsT                     traits_t;
                typedef std::basic_string<char_t>   string_t;
                typedef unsigned int                return_t;
                typedef basic_parameters<char_t>    parameters_t;

            protected:
                typedef typename traits_t::longname     longname_traits_t;
                typedef typename traits_t::shortname    shortname_traits_t;

            protected:
                // member functions to be overridden
                virtual const char_t* shortname(void) const { return NULL; }
                virtual const char_t* longname(void) const { return NULL; }
                virtual return_t handle_params(const parameters_t&) = 0;

            public:
                // typical destructor
                ~basic_option(void) {}

            public:
                // implementations for the virtual member functions of the
                // super class
                bool is_in_charge(const parameters_t& params) const {
                    const string_t& current = *(params.current());
                    if (
                               (
                                is_opt<longname_traits_t>(current)
                             && is_known<longname_traits_t>(current, longname()))
                            || (
                                is_opt<shortname_traits_t>(current)
                             && is_known<shortname_traits_t>(current, shortname()))) {
                        return true;
                    }
                    return false;
                }

                // just delegate
                unsigned int handle_responsibility(const parameters_t& params) {
                    return handle_params(params);
                }

            protected:
                // utilities
                template<typename trT>
                static bool is_opt(const string_t& current) {
                    return current.compare(
                                0,
                                trT::prefix_length,
                                trT::prefix()) == 0;
                }

                template<typename trT>
                static bool is_known(const string_t& current, const char_t* name) {
                    return (
                               name != NULL
                            && current.compare(
                                trT::prefix_length,
                                current.size() - 1,
                                name) == 0);
                }

            public:
                static bool has_opt_prefix(const string_t& current) {
                    return (
                               is_opt<longname_traits_t>(current)
                            || is_opt<shortname_traits_t>(current));
                }
        };

        /*
         *  a class to analyze options
         *  To use:
         *
         *      1. Define a class or struct that is derived from this class.
         *      2. Override the following member functions
         *
         *          - handle_nonopt(const parameters_t&, bool)
         *              - handles non-option parameters.
         *              - The second parameter whose type is bool represents
         *                whether the "current" parameter is unknown option or
         *                not. If it has the option prefix this value is true,
         *                otherwise false.
         *
         *      3. Define some classes that is derived from the class
         *         basic_option.
         *      4. Build the objects of the class that is refered in to step. 1
         *         and 3, and register the subclasses's objects of the class
         *         basic_option on the subclass's object of this class by using
         *         the member function register_option().
         *      5. Call the member function analyze() with an object of
         *         std::vector<string_t> that has parameters.
         * */
        template<typename charT>
        class basic_getopt
            : public pattern::cor::basic_chain<unsigned int, basic_parameters<charT> > {
            public:
                typedef charT                       char_t;
                typedef std::basic_string<char_t>   string_t;
                typedef unsigned int                return_t;
                typedef basic_parameters<char_t>    parameters_t;
                typedef basic_option<char_t>        option_t;
                typedef std::vector<string_t>       string_array_t;

            protected:
                typedef basic_getopt    this_t;

            protected:
                // member functions to be overridden
                virtual unsigned int handle_nonopt(const parameters_t&, bool) = 0;

            public:
                // typical destructor
                ~basic_getopt(void) {}

            protected:
                // implementations for the virtual member functions of the
                // super class
                unsigned int at_end_of_chain(const parameters_t& params) {
                    const string_t& current = *(params.current());
                    bool is_unknown_opt = option_t::has_opt_prefix(current)
                        ? true
                        : false;
                    return handle_nonopt(params, is_unknown_opt);
                }

            public:
                // utilities
                this_t& register_option(option_t& option) {
                    enlink_chain(option);
                    return *this;
                }

                this_t& register_option(option_t* option) {
                    enlink_chain(option);
                    return *this;
                }

                // main function
                unsigned int analyze(string_array_t& data) {
                    parameters_t params(data);
                    unsigned int processed = 0;
                    while (params.current() != params.end()) {
                        unsigned int n = request_to_chain(params);
                        if (n == 0)
                            throw std::logic_error("the function basic_option::handle_params() or basic_getopt::handle_nonopt() must return a non-zero value.");
                        params.advance(n);
                        processed += n;
                    }
                    return processed;
                }

                unsigned int analyze(const int argc, const char_t* const argv[]) {
                    string_array_t data;
                    data.reserve(argc);
                    for (int i = 1; i < argc; ++i) data.push_back(argv[i]);
                    return analyze(data);
                }
        };

        // for convenience
        typedef basic_parameters<char>      parameters;
        typedef basic_option<char>          option;
        typedef basic_getopt<char>          getopt;
        typedef basic_parameters<wchar_t>   wparameters;
        typedef basic_option<wchar_t>       woption;
        typedef basic_getopt<wchar_t>       wgetopt;
    }
}

#endif // GETOPT_HPP

