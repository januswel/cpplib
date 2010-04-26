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
        template<typename Event> class event_source;
        template<typename Event> class event_listener;

        /*
         *  a base class to send events
         *  To use:
         *
         *      1. Define the class or struct that is derived from this class.
         *      2. Define a new member function that creates some event object
         *         and send it by using the member function dispatch_event(1).
         *
         *  where event object is user-defined.
         * */
        template<typename Event> class event_source {
            public:
                typedef Event                       event_type;
                typedef event_listener<event_type>  listener_type;

            private:
                typedef event_source<event_type>    this_type;

            protected:
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
                typedef std::list<listener_type*>               listener_array_type;
                typedef typename listener_array_type::iterator  listener_array_iterator;

            protected:
                // an array of listners;
                listener_array_type listeners;

            protected:
                // default constructor to forbid building objects of this type
                event_source(void) {};

            public:
                // typical destructor
                virtual ~event_source(void) {}

                // register an event listner
                this_type& add_event_listener(listener_type& listener) {
                    listeners.push_back(&listener);
                    return *this;
                }

                this_type& add_event_listener(listener_type* listener) {
                    listeners.push_back(listener);
                    return *this;
                }

                // release an event listner
                this_type& remove_event_listener(listener_type& listener) {
                    listeners.remove(&listener);
                    return *this;
                }

                this_type& remove_event_listener(listener_type* listener) {
                    listeners.remove(listener);
                    return *this;
                }

            protected:
                // send the event to event listners
                void dispatch_event(const event_type& e) {
                    for (listener_array_iterator it = listeners.begin();
                            it != listeners.end();
                            ++it) {
                        (*it)->handle_event(e);
                    }
                }
        };

        /*
         *  a base class to listen events and handle them
         *  To use:
         *
         *      1. Define the class or struct that is derived from this class.
         *      2. Implement the member function handle_event(1).
         * */
        template<typename Event> class event_listener {
            public:
                typedef Event                       event_type;

            private:
                typedef event_listener<event_type>  this_type;

            public:
                // typical destructor
                virtual ~event_listener(void) {}

                // Objects of this class are identified by the memory address
                // of its instance.
                bool operator==(const this_type& rhs) const {
                    return this == &rhs;
                }
                bool operator!=(const this_type& rhs) const {
                    return !(*this == rhs);
                }

            public:
                // a virtual function which should be implemented by derived
                // classes
                virtual void handle_event(const event_type&) = 0;
        };

        // event template
        template<typename Kind, typename Data> struct basic_event {
            typedef Kind    kind_type;
            typedef Data    data_type;
            kind_type kind;
            data_type data;
        };

        // partial specialization where Data = void
        template<typename Kind> struct basic_event<Kind, void> {
            typedef Kind    kind_type;
            typedef void    data_type;
            kind_type kind;
        };
    }
}

#endif // EVENT_HPP

