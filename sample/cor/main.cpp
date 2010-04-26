/*
 * main.cpp
 *  sample codes of cor.hpp
 *
 *  written by janus_wel<janus.wel.3@gmail.com>
 *  This source code is in public domain, and has NO WARRANTY.
 * */

#include "../../header/cor.hpp"
#include "../../header/dlogger.hpp"
#include "../../header/tconv.hpp"
#include <iostream>

// TODO: use reference counting pointer for pointers to objects of PrimeNumber

// a class that represents a prime number
class PrimeNumber : public pattern::cor::basic_handler<bool, unsigned int> {
    private:
        const unsigned int value;

    public:
        // constructor
        PrimeNumber(unsigned int value) : value(value) {
            DBGLOG("PrimeNumber(" << value << ")");
        }
        // destructor
        ~PrimeNumber(void) {
            DBGLOG("~PrimeNumber(" << value << ")");
        }
        // assignment operator isn't defined
        PrimeNumber& operator=(const PrimeNumber&);

        // implementations of virtual functions
        bool is_in_charge(const unsigned int& n) const {
            return (n % value) == 0;
        }
        bool is_need_data(void) const { return false; }
        bool handle_responsibility(const data_type&) {
            DBGLOG("handle(" << value << ")");
            return false;
        }
};

// a sequence of prime numbers
class PrimeNumbers : public pattern::cor::basic_chain<bool, unsigned int> {
    public:
        // constructor
        PrimeNumbers(void) {
            DBGLOG("PrimeNumbers()");
        }
        // destructor
        ~PrimeNumbers(void) {
            DBGLOG("~PrimeNumbers()");
            // TODO
            for (handler_array_iterator it = chain.begin(); it != chain.end(); ++it) {
                delete *it;
            }
        }
        // assignment operator isn't defined
        PrimeNumbers& operator=(const PrimeNumbers&);

        // check whether a specified number is prime number or not
        bool is_prime_number(const unsigned int n) { return request_to_chain(n); }
        // add a new prime number
        // TODO
        void add(unsigned int n) { enlink_chain(new PrimeNumber(n)); }

    protected:
        // implementations of virtual functions
        bool at_end_of_chain(const data_type&) { return true; }
};

int main(const int argc, const char* const argv[]) {
    // a max of calculation range
    util::string::tconv tconv;
    unsigned int max = (argc < 2)
        ? 100
        : tconv.strto<unsigned int>(argv[1]);

    // pick up prime numbers
    PrimeNumbers pn;
    for (unsigned int n = 2; n < max; ++n) {
        if (pn.is_prime_number(n)) {
            std::cout << n << std::endl;
            pn.add(n);
        }
    }

    return 0;
}

