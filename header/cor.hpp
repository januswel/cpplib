/*
 * cor.hpp
 *  the base classes for Chain of Responsibility pattern of GoF design patterns
 *
 *  written by janus_wel<janus.wel.3@gmail.com>
 *  This source code is in public domain, and has NO WARRANTY.
 * */

#ifndef COR_HPP
#define COR_HPP

#include <list>
#include <string>
#include <stdexcept>

namespace pattern {
    namespace cor {
        /*
         *  a base class to handle data
         *  The objects of the class types that are sub classes of this make a
         *  chain.  To use:
         *
         *      1. Define a class or struct that is derived from this class.
         *      2. Override the following member functions
         *
         *          - is_in_charge(const data_t&)
         *              - must return true if the sub class has the
         *                responsibility to handle the passed data.
         *              - otherwise false.
         *          - handle_responsibility(const data_t&)
         *              - process the data.
         *
         *      3. Create objects of the sub classes and enlink(1) by
         *         basic_chain.
         *  */
        template<typename retT, typename dataT> class basic_handler {
            public:
                // typedefs
                typedef retT    return_t;
                typedef dataT   data_t;

            public:
                // destructor
                virtual ~basic_handler(void) {}

                // This must return true if the data can be handled by the
                // class.  Otherwise false.
                virtual bool is_in_charge(const data_t&) const = 0;

                // the procedure to handle the data.
                virtual return_t handle_responsibility(const data_t&) = 0;
        };

        /*
         *  a base class to handle a chain
         *  To use:
         *
         *      1A. Define a class or struct that is derived from this class.
         *          And override the member functions
         *
         *          - at_end_of_chain(void)
         *              - is run when the specified data isn't handled by any
         *                sub classes of basic_handler.
         *              - Override if you want to change the behavior of this
         *                function.
         *
         *      1B. Or, only instantiate by specifying template parameters.
         *          Overmore give it handy name by using typedef.
         *
         *      2. Create objects of the sub or instantiated class and
         *         enlink(1) objects of the sub classes of basic_handler, then
         *         execute request(const data_t&).
         *  */
        template<typename retT, typename dataT> class basic_chain {
            public:
                typedef retT                                return_t;
                typedef dataT                               data_t;
                typedef basic_handler<return_t, data_t>     handler_t;

            protected:
                typedef handler_t*                          handler_ptr_t;
                typedef std::list<handler_ptr_t>            handler_array_t;
                typedef typename handler_array_t::iterator  handler_array_iterator;

            private:
                typedef basic_chain<return_t, data_t>       this_t;

            public:
                // destructor
                virtual ~basic_chain(void) {}

            protected:
                // member variables
                handler_array_t chain;

            protected:
                // the function to be run at the end of the chain
                // Override this if you need other processings.
                virtual return_t at_end_of_chain(const data_t&) {
                    throw std::logic_error("The requested data has reached the end of the chain.");
                }

            public:
                // enlink objects
                this_t& enlink_chain(handler_t& h) {
                    chain.push_back(&h);
                    return *this;
                }

                this_t& enlink_chain(handler_t* h) {
                    chain.push_back(h);
                    return *this;
                }

                // the function to follow the chain
                return_t request_to_chain(const data_t& data) {
                    for (handler_array_iterator it = chain.begin();
                            it != chain.end();
                            ++it) {
                        const handler_ptr_t& item = *it;
                        if (item->is_in_charge(data)) {
                            return item->handle_responsibility(data);
                        }
                    }
                    return at_end_of_chain(data);
                }
        };

        // the partial specialization where retT = void
        template<typename dataT> class basic_chain<void, dataT> {
            public:
                typedef void                                return_t;
                typedef dataT                               data_t;
                typedef basic_handler<void, data_t>         handler_t;

            protected:
                typedef handler_t*                          handler_ptr_t;
                typedef std::list<handler_ptr_t>            handler_array_t;
                typedef typename handler_array_t::iterator  handler_array_iterator;

            private:
                typedef basic_chain<void, data_t>           this_t;

            public:
                // destructor
                virtual ~basic_chain(void) {}

            protected:
                // member variables
                handler_array_t chain;

            protected:
                // the function to be run at the end of the chain
                // Override this if you need other processings.
                virtual void at_end_of_chain(const data_t&) {
                    throw std::logic_error("The requested data has reached the end of the chain.");
                }

            public:
                // enlink objects
                this_t& enlink_chain(handler_t& h) {
                    chain.push_back(&h);
                    return *this;
                }

                this_t& enlink_chain(handler_t* h) {
                    chain.push_back(h);
                    return *this;
                }

                // the function to follow the chain
                void request_to_chain(const data_t& data) {
                    for (handler_array_iterator it = chain.begin();
                            it != chain.end();
                            ++it) {
                        const handler_ptr_t& item = *it;
                        if (item->is_in_charge(data)) {
                            item->handle_responsibility(data);
                        }
                    }
                    at_end_of_chain(data);
                }
        };
    }
}

#endif // COR_HPP

