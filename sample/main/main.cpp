/*
 * main.cpp
 *  sample codes for the class util::main at main.hpp
 *
 *  written by janus_wel<janus.wel.3@gmail.com>
 *  This source code is in public domain, and has NO WARRANTY.
 * */

#include "../../header/main.hpp"
#include "../../header/typeconv.hpp"

#include <iomanip>
#include <iostream>
#include <list>
#include <locale>
#include <stdexcept>
#include <string>

class Main : public util::main::main {
    public:
        static util::string::typeconverter& tconv(void) {
            static util::string::typeconverter tconv;
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
                const char_type* shortname(void) const { return "v"; }
                const char_type* longname(void) const { return "version"; }
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
                const char_type* shortname(void) const { return "h"; }
                const char_type* longname(void) const { return "help"; }
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
                const char_type* shortname(void) const { return "s"; }
                const char_type* longname(void) const { return "size"; }
                unsigned int handle_params(const parameters_type& params) {
                    parameters_type::const_iterator next =
                        params.current() + 1;
                    if (next != params.end()) {
                        state = Main::tconv().strto<unsigned int>(*next);
                        return 2;
                    }
                    else {
                        throw std::runtime_error("specify size: "
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
                const char_type* shortname(void) const { return "o"; }
                const char_type* longname(void) const { return "output"; }
                unsigned int handle_params(const parameters_type& params) {
                    parameters_type::const_iterator next =
                        params.current() + 1;
                    if (next != params.end()) {
                        state = *next;
                        return 2;
                    }
                    else throw std::runtime_error("specify output: "
                            + *(params.current()));
                }
        } opt_output;

    private:
        string_type input;
        std::list<string_type> unknown_opt;

    protected:
        unsigned int handle_unknown_opt(const parameters_type& params) {
            unknown_opt.push_back(*(params.current()));
            return 1;
        }
        unsigned int handle_behind_parameters(const parameters_type&) {
            throw std::runtime_error(
                    "don't specify anything behind the nonopt parameter.");
        }
        unsigned int handle_nonopt(const parameters_type& params) {
            input = *(params.current());
            return 1;
        }

    public:
        Main(void) {
            register_option(opt_version);
            register_option(opt_help);
            register_option(opt_size);
            register_option(opt_output);
        }

    public:
        int start(void) {
            if (!unknown_opt.empty()) {
                std::cerr
                    << "unknown options: "
                    << tconv().join(
                            unknown_opt.begin(),
                            unknown_opt.end(), ", ") << "\n"
                    << std::endl;
                return 1;
            }

            if (util::main::is_redirected()) {
                std::cout.rdbuf(std::cerr.rdbuf());
            }

            std::cout << std::left << std::boolalpha
                << std::setw(10) << "version"    << opt_version() << "\n"
                << std::setw(10) << "help"       << opt_help() << "\n"
                << std::setw(10) << "size"       << opt_size() << "\n"
                << std::setw(10) << "input"      << input << "\n"
                << std::setw(10) << "output"     << opt_output() << "\n"
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
    catch (const std::exception& ex) {
        std::cerr << "error: " << ex.what() << std::endl;
    }
}

