/*
 * observer.hpp
 *  base classes for observer pattern of GoF design patterns
 *
 *  written by janus_wel<janus.wel.3@gmail.com>
 *  This source code is in public domain, and has NO WARRANTY.
 * */

#ifndef OBSERBER_HPP
#define OBSERBER_HPP

#include <list>

namespace pattern {
    namespace observer {
        // forward declarations
        template<typename T> class basic_subject;
        template<typename T> class basic_observer;

        /*
         *  a base class to change states
         *  To use:
         *
         *      1. Define the class or struct that is derived from this class.
         *      2. Define new member variables to indicates the state of the
         *         object and new member functions to change states if those
         *         variables aren't public.
         *      3. Implement the member function subject_state(0). This
         *         function must return the value that is contained in the
         *         derived class as member variables.
         * */
        template<typename T> class basic_subject {
            public:
                // typedefs
                typedef T                       state_t;
                typedef basic_observer<state_t> observer_t;

            private:
                typedef basic_subject<state_t>  this_t;

            protected:
                /*
                 *  I think that std::list is best container to contain
                 *  observers, because:
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
                typedef std::list<observer_t*>              observer_array_t;
                typedef typename observer_array_t::iterator observer_array_iterator;

            protected:
                // a list of observers
                observer_array_t observers;

            public:
                // typical destructor
                virtual ~basic_subject(void) {}

                // register an observer
                this_t& attach_observer(observer_t& o) {
                    observers.push_back(&o);
                    return *this;
                }

                this_t& attach_observer(observer_t* o) {
                    observers.push_back(o);
                    return *this;
                }

                // release an observer
                this_t& detach_observer(observer_t& o) {
                    observers.remove(&o);
                    return *this;
                }

                this_t& detach_observer(observer_t* o) {
                    observers.remove(o);
                    return *this;
                }

                // notice the current states to all of observers
                // Should we use std::for_each(3) ?
                void notify_state(void) {
                    state_t s = subject_state();
                    for (observer_array_iterator it = observers.begin();
                            it != observers.end();
                            ++it) {
                        (*it)->update_state(s);
                    }
                }

            protected:
                // the member function to get current (latest) states
                virtual const state_t& subject_state(void) const = 0;
        };

        /*
         *  a base class to observe children of basic_subject class
         *  To use:
         *
         *      1. Define the class or struct that is derived from this class.
         *      2. Implement the member function handle(1).
         * */
        template<typename T> class basic_observer {
            public:
                // typedefs
                typedef T                       state_t;
                typedef basic_observer<state_t> this_t;

            public:
                // typical destructor
                virtual ~basic_observer(void) {}

                // Objects of this class are identified by the memory
                // address of its instance.
                bool operator==(const this_t& rhs) const {
                    return this == &rhs;
                }
                bool operator!=(const this_t& rhs) const {
                    return !(*this == rhs);
                }

            public:
                // a virtual function which should be implemented by
                // derived classes
                virtual void update_state(const state_t& s) = 0;
        };
    }
}

#endif // OBSERBER_HPP

