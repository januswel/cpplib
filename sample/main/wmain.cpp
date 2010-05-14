/*
 * main.cpp
 *  sample codes for the class util::wmain at main.hpp
 *
 *  written by janus_wel<janus.wel.3@gmail.com>
 *  This source code is in public domain, and has NO WARRANTY.
 * */

#include "../../header/main.hpp"
#include "../../header/typeconv.hpp"
#include "../../header/wexcept.hpp"

#include <iomanip>
#include <iostream>
#include <string>
#include <locale>

class Main : public util::main::wmain {
    public:
        static util::string::wtypeconverter& tconv(void) {
            static util::string::wtypeconverter tconv;
            return tconv;
        }

    private:
        class opt_version_type : public option_type {
            private:
                bool state;

            public:
                opt_version_type(void) : state(false) {}

            public:
                bool operator()(void) { return state; }

            protected:
                const char_type* shortname(void) const { return L"v"; }
                const char_type* longname(void) const { return L"version"; }
                unsigned int handle_params(const parameters_type&) {
                    state = true;
                    return 1;
                }
        } opt_version;

        class opt_help_type : public option_type {
            private:
                bool state;

            public:
                opt_help_type(void) : state(false) {}

            public:
                bool operator()(void) { return state; }

            protected:
                const char_type* shortname(void) const { return L"h"; }
                const char_type* longname(void) const { return L"help"; }
                unsigned int handle_params(const parameters_type&) {
                    state = true;
                    return 1;
                }
        } opt_help;

        class opt_size_type : public option_type {
            private:
                unsigned int state;
                static const unsigned int size_default = 4096;

            public:
                opt_size_type(void) : state(size_default) {}

            public:
                unsigned int operator()(void) { return state; }

            protected:
                const char_type* shortname(void) const { return L"s"; }
                const char_type* longname(void) const { return L"size"; }
                unsigned int handle_params(const parameters_type& params) {
                    parameters_type::const_iterator next =
                        params.current() + 1;
                    if (next != params.end()) {
                        state = Main::tconv().strto<unsigned int>(*next);
                        return 2;
                    }
                    else {
                        throw util::exception::wruntime_error(L"specify size: "
                                + *(params.current()));
                    }
                }
        } opt_size;

        class opt_output_type : public option_type {
            private:
                string_type state;

            public:
                const string_type& operator()(void) { return state; }

            protected:
                const char_type* shortname(void) const { return L"o"; }
                const char_type* longname(void) const { return L"output"; }
                unsigned int handle_params(const parameters_type& params) {
                    parameters_type::const_iterator next =
                        params.current() + 1;
                    if (next != params.end()) {
                        state = *next;
                        return 2;
                    }
                    else throw util::exception::wruntime_error(L"specify output: "
                            + *(params.current()));
                }
        } opt_output;

    public:
        Main(void) {
            register_option(opt_version);
            register_option(opt_help);
            register_option(opt_size);
            register_option(opt_output);
        }

    public:
        int start(void) {
            if (!unknown_opt_params.empty()) {
                throw util::exception::wruntime_error(L"unknown option: "
                        + tconv().join(
                            unknown_opt_params.begin(),
                            unknown_opt_params.end(), L", "));
            }

            if (nonopt_parameters.size() > 1) {
                throw util::exception::wruntime_error(L"too many parameters");
            }

            const string_type& input = (nonopt_parameters.empty()
                ? std::wstring()
                : nonopt_parameters[0]);

            if (util::main::is_redirected()) {
                std::wcout.rdbuf(std::wcerr.rdbuf());
            }

            std::wcout << std::left << std::boolalpha
                << std::setw(10) << L"version"    << opt_version() << L"\n"
                << std::setw(10) << L"help"       << opt_help() << L"\n"
                << std::setw(10) << L"size"       << opt_size() << L"\n"
                << std::setw(10) << L"input"      << input << L"\n"
                << std::setw(10) << L"output"     << opt_output() << L"\n"
                << std::endl;

            return 0;
        }
};

int main(const int argc, const char* const argv[]) {
    try {
        std::locale::global(std::locale(""));
        Main main;
        main.do_parameters(argc, argv);
        return main.start();
    }
    catch (const util::exception::wexception& ex) {
        std::wcerr << L"error: " << ex.what() << std::endl;
    }
}

