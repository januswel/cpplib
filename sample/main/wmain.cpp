/*
 * wmain.cpp
 *  sample codes for the class util::wmain at main.hpp
 *
 *  written by janus_wel<janus.wel.3@gmail.com>
 *  This source code is in public domain, and has NO WARRANTY.
 * */

#include "../../header/main.hpp"
#include "../../header/tconv.hpp"
#include "../../header/wexcept.hpp"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

class Main : public util::main::wmain {
    private:
        class opt_version_t : public option_t {
            private:
                bool state;

            public:
                opt_version_t(void) : state(false) {}

            public:
                bool operator()(void) { return state; }

            protected:
                const char_t* shortname(void) const { return L"v"; }
                const char_t* longname(void) const { return L"version"; }
                unsigned int handle_params(const parameters_t&) {
                    state = true;
                    return 1;
                }
        } opt_version;

        class opt_help_t : public option_t {
            private:
                bool state;

            public:
                opt_help_t(void) : state(false) {}

            public:
                bool operator()(void) { return state; }

            protected:
                const char_t* shortname(void) const { return L"h"; }
                const char_t* longname(void) const { return L"help"; }
                unsigned int handle_params(const parameters_t&) {
                    state = true;
                    return 1;
                }
        } opt_help;

        class opt_size_t : public option_t {
            private:
                unsigned int state;
                static const unsigned int size_default = 4096;

            public:
                opt_size_t(void) : state(size_default) {}

            public:
                unsigned int operator()(void) { return state; }

            protected:
                const char_t* shortname(void) const { return L"s"; }
                const char_t* longname(void) const { return L"size"; }
                unsigned int handle_params(const parameters_t& params) {
                    parameters_t::const_iterator next = params.current() + 1;
                    if (next != params.end()) {
                        util::string::wtconv conv;
                        state = conv.strto<unsigned int>(*next);
                        return 2;
                    }
                    else {
                        throw util::exception::wruntime_error(L"specify size: " + *(params.current()));
                    }
                }
        } opt_size;

        class opt_output_t : public option_t {
            private:
                string_t state;

            public:
                const string_t& operator()(void) { return state; }

            protected:
                const char_t* shortname(void) const { return L"o"; }
                const char_t* longname(void) const { return L"output"; }
                unsigned int handle_params(const parameters_t& params) {
                    parameters_t::const_iterator next = params.current() + 1;
                    if (next != params.end()) {
                        state = *next;
                        return 2;
                    }
                    else throw util::exception::wruntime_error(L"specify output: " + *(params.current()));
                }
        } opt_output;

    public:
        Main(void) {
            getopt.register_option(opt_version);
            getopt.register_option(opt_help);
            getopt.register_option(opt_size);
            getopt.register_option(opt_output);
        }

    public:
        int start(void) {
            string_array_const_iterator it = std::find_if(
                    nonopt_parameters().begin(),
                    nonopt_parameters().end(),
                    option_t::has_opt_prefix);
            if (it != nonopt_parameters().end()) {
                throw util::exception::wruntime_error(L"unknown option: " + *it);
            }
            if (nonopt_parameters().size() > 1) {
                throw util::exception::wruntime_error(L"too many parameters");
            }
            const string_t& input = nonopt_parameters()[0];

            std::cout << std::left << std::boolalpha
                << std::setw(10) << "version"    << opt_version() << "\n"
                << std::setw(10) << "help"       << opt_help() << "\n"
                << std::setw(10) << "size"       << opt_size() << "\n"
                << std::setw(10) << "input"      << nwconv.wton(input) << "\n"
                << std::setw(10) << "output"     << nwconv.wton(opt_output()) << "\n"
                << std::endl;

            return 0;
        }
};

int main(const int argc, const char* const argv[]) {
    try {
        Main main;
        main.pack_parameters(argc, argv);
        main.analyze();
        return main.start();
    }
    catch (const util::exception::wexception& ex) {
        std::wcerr << "error: " << ex.what() << std::endl;
    }
}

