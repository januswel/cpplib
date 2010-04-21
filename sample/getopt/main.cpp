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
// a template structure of event
template<typename valueT>
struct EventOpt {
    typedef valueT  value_t;
    event_kind_t type;
    value_t val;
};
// partial specialization where valueT = void
template<>
struct EventOpt<void> {
    typedef void  value_t;
    event_kind_t type;
};

// instantiations of event types
typedef EventOpt<void>                              EventVoid;
typedef pattern::event::event_listener<EventVoid>   VoidListener;
typedef pattern::event::event_source<EventVoid>     VoidSource;

typedef EventOpt<unsigned int>                      EventUint;
typedef pattern::event::event_listener<EventUint>   UintListener;
typedef pattern::event::event_source<EventUint>     UintSource;

typedef EventOpt<std::string>                       EventString;
typedef pattern::event::event_listener<EventString> StringListener;
typedef pattern::event::event_source<EventString>   StringSource;

// my getopt definition
class MyGetOpt : public util::getopt::getopt, VoidListener, UintListener, StringListener {
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
            switch (event.type) {
                case VERSION:   mv_version = true;
                                break;
                case HELP:      mv_help = true;
                                break;
                default:        throw std::logic_error("unknown event");
            }
        }

        void handle_event(const EventUint& event) {
            DBGLOG("handle_event(const EventUint&)");
            mv_size = event.val;
        }

        void handle_event(const EventString& event) {
            DBGLOG("handle_event(const EventString&)");
            mv_output = event.val;
        }

    private:
        // option definition
        class OptVersion : public option_t, public VoidSource {
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

        class OptHelp : public option_t, public VoidSource {
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

        class OptSize : public option_t, public UintSource {
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

        class OptOutput : public option_t, public StringSource {
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
                        EventString event = { OUTPUT, *next };
                        dispatch_event(event);
                        return 2;
                    }
                    else throw std::runtime_error("specify output: " + *(params.current()));
                }
        } opt_output;

    protected:
        // non-option handler
        unsigned int handle_nonopt(const parameters_t& params, bool is_unknown_opt) {
            DBGLOG("handle_nonopt");

            if (is_unknown_opt) throw std::runtime_error("unknown option: " + *(params.current()));
            if (params.current() + 1 == params.end()) {
                mv_input = *(params.current());
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

