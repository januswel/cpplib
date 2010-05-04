/*
 * main.cpp
 *  sample codes for the class util::main at main.hpp
 *
 *  written by janus_wel<janus.wel.3@gmail.com>
 *  This source code is in public domain, and has NO WARRANTY.
 * */

#include "../../header/main.hpp"
#include "../../header/typeconv.hpp"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>

#include <iterator>
#include <sstream>

// copy_if
template<typename InputIterator, typename OutputIterator, typename Predicate>
inline OutputIterator
copy_if(InputIterator first, InputIterator last, OutputIterator result, Predicate pred) {
    for (; first != last; ++first) if (pred(*first)) *result++ = *first;
    return result;
}

// grep
template<typename SequenceContainer, typename Predicate>
SequenceContainer& grep(const SequenceContainer& input, SequenceContainer& output, Predicate pred) {
    std::iterator_traits<SequenceContainer::iterator>::difference_type count =
        std::count_if(input.begin(), input.end(), pred);
    if (count > 0) {
        output.resize(count);
        copy_if(input.begin(), input.end(), output.begin(), pred);
    }
    return output;
}

template<typename Value, typename Predicate>
std::vector<Value>& grep(const std::vector<Value>& input, std::vector<Value>& output, Predicate pred) {
    std::iterator_traits<std::vector<Value>::iterator>::difference_type count =
        std::count_if(input.begin(), input.end(), pred);
    if (count > 0) {
        output.resize(count);
        output.reserve(count);
        copy_if(input.begin(), input.end(), output.begin(), pred);
    }
    return output;
}

template<typename Char, typename ForwardIterator>
std::basic_string<Char> join(const Char* delimeter, ForwardIterator first, ForwardIterator last) {
    if (first == last) return std::basic_string<Char>("");

    std::basic_stringstream<Char> ss;
    ss << *first++;
    while (first != last) ss << delimeter << *first++;
    return ss.str();
}

class Main : public util::main::main {
    private:
        class opt_version_t : public option_t {
            private:
                bool state;

            public:
                opt_version_t(void) : state(false) {}

            public:
                bool operator()(void) { return state; }

            protected:
                const char_t* shortname(void) const { return "v"; }
                const char_t* longname(void) const { return "version"; }
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
                const char_t* shortname(void) const { return "h"; }
                const char_t* longname(void) const { return "help"; }
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
                const char_t* shortname(void) const { return "s"; }
                const char_t* longname(void) const { return "size"; }
                unsigned int handle_params(const parameters_t& params) {
                    parameters_t::const_iterator next = params.current() + 1;
                    if (next != params.end()) {
                        util::string::typeconverter conv;
                        state = conv.strto<unsigned int>(*next);
                        return 2;
                    }
                    else {
                        throw std::runtime_error("specify size: " + *(params.current()));
                    }
                }
        } opt_size;

        class opt_output_t : public option_t {
            private:
                string_t state;

            public:
                const string_t& operator()(void) { return state; }

            protected:
                const char_t* shortname(void) const { return "o"; }
                const char_t* longname(void) const { return "output"; }
                unsigned int handle_params(const parameters_t& params) {
                    parameters_t::const_iterator next = params.current() + 1;
                    if (next != params.end()) {
                        state = *next;
                        return 2;
                    }
                    else throw std::runtime_error("specify output: " + *(params.current()));
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
            const string_array_t& np = nonopt_parameters();

            string_array_t unknown_opt;
            grep(np, unknown_opt, option_t::has_opt_prefix);
            if (!unknown_opt.empty()) {
                std::string unknown_opts = join(", ", unknown_opt.begin(), unknown_opt.end());
                throw std::runtime_error("unknown option: " + unknown_opts);
            }
            if (np.size() > 1) {
                throw std::runtime_error("too many parameters");
            }
            const string_t& input = np.empty()
                ? std::string("")
                : np[0];

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
        Main main;
        main.pack_parameters(argc, argv);
        main.analyze();
        return main.start();
    }
    catch (const std::exception& ex) {
        std::cerr << "error: " << ex.what() << std::endl;
    }
}

