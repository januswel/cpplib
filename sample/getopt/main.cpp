/*
 * main.cpp
 *  sample codes for getopt.hpp
 *
 *  compile option
 *      - debug
 *          - g++ -Wall --pedantic main.cpp
 *          - cl /EHsc /W4 main.cpp
 *      - optimized
 *          - g++ -Wall --pedantic -O2 -DNDEBUG main.cpp
 *          - cl /EHsc /W4 /O2 /DNDEBUG main.cpp
 *
 *  written by janus_wel<janus.wel.3@gmail.com>
 *  This source code is in public domain, and has NO WARRANTY.
 * */

#include "../../header/dlogger.hpp"
#include "../../header/event.hpp"
#include "../../header/getopt.hpp"
#include "../../header/tconv.hpp"

#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

// event definition
// event kinds
enum event_kind_t {
    VERSION,
    HELP,
    SIZE,
    OUTPUT
};

// instantiations of event types
typedef pattern::event::basic_event<event_kind_t, void> EventVoid;
typedef pattern::event::event_listener<EventVoid>       EventListenerVoid;
typedef pattern::event::event_source<EventVoid>         EventSourceVoid;

typedef pattern::event::basic_event<event_kind_t, unsigned int> EventUint;
typedef pattern::event::event_listener<EventUint>               EventListenerUint;
typedef pattern::event::event_source<EventUint>                 EventSourceUint;

typedef pattern::event::basic_event<event_kind_t, std::string>  EventStr;
typedef pattern::event::event_listener<EventStr>                EventListenerStr;
typedef pattern::event::event_source<EventStr>                  EventSourceStr;

// my getopt definition
class MyGetOpt : public util::getopt::getopt, EventListenerVoid, EventListenerUint, EventListenerStr {
    private:
        // variables to contain option values
        bool mv_version;
        bool mv_help;
        unsigned int mv_size;
        std::string mv_input;
        std::string mv_output;

    public:
        // getter
        bool version(void) { return mv_version; }
        bool help(void) { return mv_help; }
        unsigned int size(void) { return mv_size; }
        const std::string& input(void) { return mv_input; }
        const std::string& output(void) { return mv_output; }

    public:
        // event handlers
        void handle_event(const EventVoid& event) {
            DBGLOG("handle_event(const EventVoid&)");
            switch (event.kind) {
                case VERSION:   mv_version = true;
                                break;
                case HELP:      mv_help = true;
                                break;
                default:        throw std::logic_error("unknown event");
            }
        }

        void handle_event(const EventUint& event) {
            DBGLOG("handle_event(const EventUint&)");
            mv_size = event.data;
        }

        void handle_event(const EventStr& event) {
            DBGLOG("handle_event(const EventStr&)");
            mv_output = event.data;
        }

    private:
        // option definition
        class OptVersion : public option_t, public EventSourceVoid {
            public:
                OptVersion(void) { DBGLOG("OptVersion"); }
                ~OptVersion(void) { DBGLOG("~OptVersion"); }

            protected:
                const char_t* shortname(void) const { return "v"; }
                const char_t* longname(void) const { return "version"; }

                unsigned int handle_params(const parameters_t&) {
                    DBGLOG("OptVersion::handle_params");
                    EventVoid event = { VERSION };
                    dispatch_event(event);
                    return 1;
                }
        } opt_version;

        class OptHelp : public option_t, public EventSourceVoid {
            public:
                OptHelp(void) { DBGLOG("OptHelp"); }
                ~OptHelp(void) { DBGLOG("~OptHelp"); }

            protected:
                const char_t* shortname(void) const { return "h"; }
                const char_t* longname(void) const { return "help"; }

                unsigned int handle_params(const parameters_t&) {
                    DBGLOG("OptHelp::handle_params");
                    EventVoid event = { HELP };
                    dispatch_event(event);
                    return 1;
                }
        } opt_help;

        class OptSize : public option_t, public EventSourceUint {
            public:
                OptSize(void) { DBGLOG("OptSize"); }
                ~OptSize(void) { DBGLOG("~OptSize"); }

            protected:
                const char_t* shortname(void) const { return "s"; }
                const char_t* longname(void) const { return "size"; }

                unsigned int handle_params(const parameters_t& params) {
                    DBGLOG("OptSize::handle_params");
                    parameters_t::const_iterator next = params.current() + 1;
                    if (next != params.end()) {
                        util::string::tconv tconv;
                        EventUint event = { SIZE, tconv.strto<unsigned int>(*next) };
                        dispatch_event(event);
                        return 2;
                    }
                    else {
                        throw std::runtime_error("specify size: " + *(params.current()));
                    }
                }
        } opt_size;

        class OptOutput : public option_t, public EventSourceStr {
            public:
                OptOutput(void) { DBGLOG("OptOutput"); }
                ~OptOutput(void) { DBGLOG("~OptOutput"); }

            protected:
                const char_t* shortname(void) const { return "o"; }
                const char_t* longname(void) const { return "output"; }

                unsigned int handle_params(const parameters_t& params) {
                    DBGLOG("OptOutput::handle_params");
                    parameters_t::const_iterator next = params.current() + 1;
                    if (next != params.end()) {
                        EventStr event = { OUTPUT, *next };
                        dispatch_event(event);
                        return 2;
                    }
                    else throw std::runtime_error("specify output: " + *(params.current()));
                }
        } opt_output;

    protected:
        // non-option handler
        unsigned int handle_nonopt(const parameters_t& params) {
            DBGLOG("handle_nonopt");

            const string_t& current = *(params.current());

            if (option_t::has_opt_prefix(current)) {
                throw std::runtime_error("unknown option: " + current);
            }

            if (params.current() + 1 == params.end()) {
                mv_input = current;
                return 1;
            }
            else {
                throw std::runtime_error("too many parameters.");
            }
        }

    public:
        // constructor
        MyGetOpt(void)
            : mv_version(false), mv_help(false), mv_size(4096), mv_input("") {
                DBGLOG("MyGetOpt::MyGetOpt");

                // register event listners
                opt_version.add_event_listener(this);
                opt_help.add_event_listener(this);
                opt_size.add_event_listener(this);
                opt_output.add_event_listener(this);

                // register options
                register_option(opt_version);
                register_option(opt_help);
                register_option(opt_size);
                register_option(opt_output);
            }

        // destructor
        ~MyGetOpt(void) {
            DBGLOG("MyGetOpt::~MyGetOpt");
        }
};

int main(const int argc, const char* const argv[]) {
    try {
        MyGetOpt opt;
        opt.analyze(argc, argv);

        std::cout << std::left << std::boolalpha
            << std::setw(10) << "version"    << opt.version() << "\n"
            << std::setw(10) << "help"       << opt.help() << "\n"
            << std::setw(10) << "size"       << opt.size() << "\n"
            << std::setw(10) << "input"      << opt.input() << "\n"
            << std::setw(10) << "output"     << opt.output() << "\n"
            << std::endl;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    }

    return 0;
}

