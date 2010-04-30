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
         *          - is_in_charge(const data_type&)
         *              - must return true if the sub class has the
         *                responsibility to handle the passed data.
         *              - otherwise false.
         *          - handle_responsibility(const data_type&)
         *              - process the data.
         *
         *      3. Create objects of the sub classes and enlink(1) by
         *         basic_chain.
         *  */
        template<typename Return, typename Data> class basic_handler {
            public:
                // typedefs
                typedef Return  return_type;
                typedef Data    data_type;

            public:
                // destructor
                virtual ~basic_handler(void) {}

                // This must return true if the data can be handled by the
                // class.  Otherwise false.
                virtual bool is_in_charge(const data_type&) const = 0;

                // the procedure to handle the data.
                virtual return_type
                    handle_responsibility(const data_type&) = 0;
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
         *         execute request(const data_type&).
         *  */
        template<typename Return, typename Data> class basic_chain {
            public:
                typedef Return                                  return_type;
                typedef Data                                    data_type;
                typedef basic_handler<return_type, data_type>   handler_type;

            protected:
                typedef std::list<handler_type*>                handler_array_type;
                typedef typename handler_array_type::iterator   handler_array_iterator;

            private:
                typedef basic_chain<return_type, data_type>     this_type;

            public:
                // destructor
                virtual ~basic_chain(void) {}

            protected:
                // member variables
                handler_array_type chain;

            protected:
                // the function to be run at the end of the chain
                // Override this if you need other processings.
                virtual return_type at_end_of_chain(const data_type&) = 0;

            public:
                // enlink objects
                this_type& enlink_chain(handler_type& h) {
                    chain.push_back(&h);
                    return *this;
                }

                this_type& enlink_chain(handler_type* h) {
                    chain.push_back(h);
                    return *this;
                }

                // the function to follow the chain
                return_type request_to_chain(const data_type& data) {
                    for (handler_array_iterator itr = chain.begin();
                            itr != chain.end(); ++itr) {
                        if ((*itr)->is_in_charge(data)) {
                            return (*itr)->handle_responsibility(data);
                        }
                    }
                    return at_end_of_chain(data);
                }
        };

        // the partial specialization where Return = void
        template<typename Data> class basic_chain<void, Data> {
            public:
                typedef void                            return_type;
                typedef Data                            data_type;
                typedef basic_handler<void, data_type>  handler_type;

            protected:
                typedef std::list<handler_type*>                handler_array_type;
                typedef typename handler_array_type::iterator   handler_array_iterator;

            private:
                typedef basic_chain<void, data_type>           this_type;

            public:
                // destructor
                virtual ~basic_chain(void) {}

            protected:
                // member variables
                handler_array_type chain;

            protected:
                // the function to be run at the end of the chain
                // Override this if you need other processings.
                virtual void at_end_of_chain(const data_type&) = 0;

            public:
                // enlink objects
                this_type& enlink_chain(handler_type& h) {
                    chain.push_back(&h);
                    return *this;
                }

                this_type& enlink_chain(handler_type* h) {
                    chain.push_back(h);
                    return *this;
                }

                // the function to follow the chain
                void request_to_chain(const data_type& data) {
                    for (handler_array_iterator itr = chain.begin();
                            itr != chain.end();
                            ++itr) {
                        if ((*itr)->is_in_charge(data)) {
                            (*itr)->handle_responsibility(data);
                        }
                    }
                    at_end_of_chain(data);
                }
        };
    }
}

#endif // COR_HPP

