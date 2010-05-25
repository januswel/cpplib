/*
 * option.hpp
 *  Declarations and definitions of option classes
 *
 *  written by janus_wel<janus.wel.3@gmail.com>
 *  This source code is in public domain, and has NO WARRANTY.
 * */

#ifndef OPTION_HPP
#define OPTION_HPP

#include "helper/getopt.hpp"
#include "helper/event.hpp"

#include "sample.hpp"

enum opt_event_kind {
    OPT_SIZE
};

typedef pattern::event::basic_event<opt_event_kind, unsigned int> event_opt_uint;

// option definitions
// example for flag-type option
class opt_version_type
    : public util::getopt::option,
      public pattern::event::event_source<priority_type> {
    protected:
        const char_type* shortname(void) const { return "v"; }
        const char_type* longname(void) const { return "version"; }
        unsigned int handle_params(const parameters_type&) {
            dispatch_event(VERSION);
            return 1;
        }
};

// example for binding-argument[s]-type option
class opt_size_type
    : public util::getopt::option,
      public pattern::event::event_source<event_opt_uint> {
    protected:
        const char_type* shortname(void) const { return "s"; }
        const char_type* longname(void) const { return "size"; }
        unsigned int handle_params(const parameters_type& params) {
            parameters_type::const_iterator next = params.current() + 1;

            if (next == params.end()) {
                throw sample_error(BAD_ARGUMENT,
                        "specify size: " + *(params.current()) + "\n");
            }

            event_opt_uint event = {OPT_SIZE, tconv.strto<unsigned int>(*next)};
            dispatch_event(event);
            return 2;
        }
};

#endif // OPTION_HPP

