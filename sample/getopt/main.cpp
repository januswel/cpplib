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
#include "../../header/typeconv.hpp"

#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

// event definition
// event kinds
enum event_kind_type {
    VERSION,
    HELP,
    SIZE,
    INPUT,
    OUTPUT
};

// instantiations of event types
typedef pattern::event::basic_event<event_kind_type, void>  EventVoid;
typedef pattern::event::event_listener<EventVoid>           EventListenerVoid;
typedef pattern::event::event_source<EventVoid>             EventSourceVoid;

typedef pattern::event::basic_event<event_kind_type, unsigned int>  EventUint;
typedef pattern::event::event_listener<EventUint>                   EventListenerUint;
typedef pattern::event::event_source<EventUint>                     EventSourceUint;

typedef pattern::event::basic_event<event_kind_type, std::string>   EventStr;
typedef pattern::event::event_listener<EventStr>                    EventListenerStr;
typedef pattern::event::event_source<EventStr>                      EventSourceStr;

// a class to contain states of options
class OptState : public EventListenerVoid, public EventListenerUint, public EventListenerStr {
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
        OptState(void)
            : mv_version(false), mv_help(false), mv_size(4096), mv_input("") {}

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
            switch (event.kind) {
                case INPUT:     mv_input = event.data;
                                break;
                case OUTPUT:    mv_output = event.data;
                                break;
                default:        throw std::logic_error("unknown event");
            }
        }
};

// my getopt definition
class MyGetOpt : public util::getopt::getopt, public EventSourceStr {
    private:
        // option definition
        class OptVersion : public option_type, public EventSourceVoid {
            public:
                OptVersion(void) { DBGLOG("OptVersion"); }
                ~OptVersion(void) { DBGLOG("~OptVersion"); }

            protected:
                const char_type* shortname(void) const { return "v"; }
                const char_type* longname(void) const { return "version"; }

                unsigned int handle_params(const parameters_type&) {
                    DBGLOG("OptVersion::handle_params");
                    EventVoid event = { VERSION };
                    dispatch_event(event);
                    return 1;
                }
        } opt_version;

        class OptHelp : public option_type, public EventSourceVoid {
            public:
                OptHelp(void) { DBGLOG("OptHelp"); }
                ~OptHelp(void) { DBGLOG("~OptHelp"); }

            protected:
                const char_type* shortname(void) const { return "h"; }
                const char_type* longname(void) const { return "help"; }

                unsigned int handle_params(const parameters_type&) {
                    DBGLOG("OptHelp::handle_params");
                    EventVoid event = { HELP };
                    dispatch_event(event);
                    return 1;
                }
        } opt_help;

        class OptSize : public option_type, public EventSourceUint {
            public:
                OptSize(void) { DBGLOG("OptSize"); }
                ~OptSize(void) { DBGLOG("~OptSize"); }

            protected:
                const char_type* shortname(void) const { return "s"; }
                const char_type* longname(void) const { return "size"; }

                unsigned int handle_params(const parameters_type& params) {
                    DBGLOG("OptSize::handle_params");
                    parameters_type::const_iterator next = params.current() + 1;
                    if (next != params.end()) {
                        util::string::typeconverter conv;
                        EventUint event = { SIZE, conv.strto<unsigned int>(*next) };
                        dispatch_event(event);
                        return 2;
                    }
                    else {
                        throw std::runtime_error("specify size: " + *(params.current()));
                    }
                }
        } opt_size;

        class OptOutput : public option_type, public EventSourceStr {
            public:
                OptOutput(void) { DBGLOG("OptOutput"); }
                ~OptOutput(void) { DBGLOG("~OptOutput"); }

            protected:
                const char_type* shortname(void) const { return "o"; }
                const char_type* longname(void) const { return "output"; }

                unsigned int handle_params(const parameters_type& params) {
                    DBGLOG("OptOutput::handle_params");
                    parameters_type::const_iterator next = params.current() + 1;
                    if (next != params.end()) {
                        EventStr event = { OUTPUT, *next };
                        dispatch_event(event);
                        return 2;
                    }
                    else throw std::runtime_error("specify output: " + *(params.current()));
                }
        } opt_output;

    protected:
        // unknown-option handler
        unsigned int handle_unknown_opt(const parameters_type& params) {
            DBGLOG("handle_unknown_opt");

            throw std::runtime_error("unknown option: " + *(params.current()));
        }

        // parameters behind nonopt-parameter handler
        unsigned int handle_behind_parameters(const parameters_type&) {
            DBGLOG("handle_behind_parameters");

            throw std::runtime_error("too many parameters.");
        }

        // non-option handler
        unsigned int handle_nonopt(const parameters_type& params) {
            DBGLOG("handle_nonopt");

            EventStr event = {INPUT, *(params.current())};
            dispatch_event(event);
            return 1;
        }

    public:
        // constructor
        MyGetOpt(OptState& os) {
            DBGLOG("MyGetOpt::MyGetOpt");

            // register event listners
            opt_version.add_event_listener(os);
            opt_help.add_event_listener(os);
            opt_size.add_event_listener(os);
            opt_output.add_event_listener(os);
            this->add_event_listener(os);

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
        OptState os;
        MyGetOpt opt(os);
        opt.analyze_option(argc, argv);

        std::cout << std::left << std::boolalpha
            << std::setw(10) << "version"    << os.version() << "\n"
            << std::setw(10) << "help"       << os.help() << "\n"
            << std::setw(10) << "size"       << os.size() << "\n"
            << std::setw(10) << "input"      << os.input() << "\n"
            << std::setw(10) << "output"     << os.output() << "\n"
            << std::endl;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    }

    return 0;
}

