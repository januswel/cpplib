/*
 * sample.hpp
 *  Declarations of basic elements for the program
 *
 *  written by janus_wel<janus.wel.3@gmail.com>
 *  This source code is in public domain, and has NO WARRANTY.
 * */

#ifndef SAMPLE_HPP
#define SAMPLE_HPP

#include <stdexcept>

#include "helper/typeconv.hpp"

// enumerations for return expression
enum return_type {
    OK = 0,
    BAD_ARGUMENT,
    UNKNOWN
};

enum priority_type {
    VERSION,
    HELP,
    UNSPECIFIED
};

// global objects
extern util::string::typeconverter tconv;

// functions to give meta informations
const char* name(void);
const char* version(void);
void usage(std::ostream& out);
void about(std::ostream& out);

// customized exception class
class sample_error : public std::domain_error {
    private:
        return_type mv_return_value;

    public:
        sample_error(const return_type return_value, const std::string& msg)
            : std::domain_error(msg), mv_return_value(return_value) {}
        return_type return_value(void) const { return mv_return_value; }
};

#endif // SAMPLE_HPP

