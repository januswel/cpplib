/*
 * event.hpp
 *  base classes to send and receive events
 *
 *  written by janus_wel<janus.wel.3@gmail.com>
 *  This source code is in public domain, and has NO WARRANTY.
 * */

#ifndef EVENT_HPP
#define EVENT_HPP

#include <list>

namespace pattern {
    namespace event {
        // forward declarations
        template<typename T> class event_source;
        template<typename T> class event_listener;

        /*
         *  a base class to send events
         *  To use:
         *
         *      1. Define the class or struct that is derived from this class.
         *      2. Define a new member function that creates some event object
         *         and send it by using the member function dispatch(1).
         *
         *  where event object is user-defined.
         * */
        template<typename T> class event_source {
            public:
                // typedefs
                typedef T                           event_t;
                typedef event_source<event_t>       this_t;
                typedef event_listener<event_t>     listener_t;

                /*
                 * I think that std::list is best container to contain event
                 * listeners, because:
                 *
                 *      - The order of items that are added is kept, so the
                 *        user of this class can handle the order of
                 *        notifications.
                 *          - For now, I exclude a sort of the list and sorted
                 *            addition of a new items for simplicity.
                 *      - There is a need for successive accesses to all items
                 *        from start to end once, so it isn't the disadvantage
                 *        that randome access is slow.
                 *      - Adding and removing items are fast.
                 *  */
                typedef std::list<listener_t*>              listener_array_t;
                typedef typename listener_array_t::iterator listener_array_iterator;

            protected:
                // an array of listners;
                listener_array_t listeners;

            protected:
                // default constructor to forbid building objects of this type
                event_source(void) {};

            public:
                // typical destructor
                virtual ~event_source(void) {}

                // register an event listner
                this_t& add_event_listener(listener_t& l) {
                    listeners.push_back(&l);
                    return *this;
                }

                this_t& add_event_listener(listener_t* l) {
                    listeners.push_back(l);
                    return *this;
                }

                // release an event listner
                this_t& remove_event_listener(listener_t& l) {
                    listeners.remove(&l);
                    return *this;
                }

                this_t& remove_event_listener(listener_t* l) {
                    listeners.remove(l);
                    return *this;
                }

            protected:
                // send the event to event listners
                void dispatch(const event_t& e) {
                    for (listener_array_iterator it = listeners.begin();
                            it != listeners.end();
                            ++it) {
                        (*it)->handle(e);
                    }
                }
        };

        /*
         *  a base class to listen events and handle them
         *  To use:
         *
         *      1. Define the class or struct that is derived from this class.
         *      2. Implement the member function handle(1).
         * */
        template<typename T> class event_listener {
            public:
                // typedefs
                typedef T                       event_t;
                typedef event_listener<event_t> this_t;

            public:
                // typical destructor
                virtual ~event_listener(void) {}

                // Objects of this class are identified by the memory address
                // of its instance.
                bool operator==(const this_t& rhs) const {
                    return this == &rhs;
                }
                bool operator!=(const this_t& rhs) const {
                    return !(*this == rhs);
                }

            public:
                // a virtual function which should be implemented by derived
                // classes
                virtual void handle(const event_t&) = 0;
        };
    }
}

#endif // EVENT_HPP

