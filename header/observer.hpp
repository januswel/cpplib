/*
 * observer.hpp
 *  base classes for observer pattern of GoF design patterns
 *
 *  written by janus_wel<janus.wel.3@gmail.com>
 *  This source code is in public domain, and has NO WARRANTY.
 * */

#ifndef OBSERBER_HPP
#define OBSERBER_HPP

#include <algorithm>
#include <functional>
#include <list>

namespace pattern {
    namespace observer {
        // forward declarations
        template<typename State> class basic_subject;
        template<typename State> class basic_observer;

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
        template<typename State> class basic_subject {
            public:
                // typedefs
                typedef State                       state_type;
                typedef basic_observer<state_type>  observer_type;

            private:
                typedef basic_subject<state_type>   this_type;

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
                typedef std::list<observer_type*>              observer_array_type;
                typedef typename observer_array_type::iterator observer_array_iterator;

            protected:
                // a list of observers
                observer_array_type observers;

            public:
                // typical destructor
                virtual ~basic_subject(void) {}

                // register an observer
                this_type& attach_observer(observer_type& o) {
                    observers.push_back(&o);
                    return *this;
                }

                this_type& attach_observer(observer_type* o) {
                    observers.push_back(o);
                    return *this;
                }

                // release an observer
                this_type& detach_observer(observer_type& o) {
                    observers.remove(&o);
                    return *this;
                }

                this_type& detach_observer(observer_type* o) {
                    observers.remove(o);
                    return *this;
                }

                // notice the current states to all of observers
                // Should we use std::for_each(3) ?
                void notify_state(void) {
                    state_type s = subject_state();
                    std::for_each(
                            observers.begin(), observers.end(),
                            std::bind2nd(
                                std::mem_fun(&observer_type::update_state),
                                s));
                }

            protected:
                // the member function to get current (latest) states
                virtual const state_type& subject_state(void) const = 0;
        };

        /*
         *  a base class to observe children of basic_subject class
         *  To use:
         *
         *      1. Define the class or struct that is derived from this class.
         *      2. Implement the member function handle(1).
         * */
        template<typename State> class basic_observer {
            public:
                // typedefs
                typedef State                       state_type;
                typedef basic_observer<state_type>  this_type;

            public:
                // typical destructor
                virtual ~basic_observer(void) {}

                // Objects of this class are identified by the memory
                // address of its instance.
                bool operator==(const this_type& rhs) const {
                    return this == &rhs;
                }
                bool operator!=(const this_type& rhs) const {
                    return !(*this == rhs);
                }

            public:
                // a virtual function which should be implemented by
                // derived classes
                virtual void update_state(const state_type& s) = 0;
        };
    }
}

#endif // OBSERBER_HPP

