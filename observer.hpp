/*
 * Base classes for observer pattern of GoF design patterns
 *
 * written by janus_wel<janus.wel.3@gmail.com>
 * This source code is in public domain, and has NO WARRANTY.
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
         * The class to change states
         * To use, you must define the class or struct that is derived from this class.
         * */
        template<typename T>
            class basic_subject {
                public:
                    // typedefs
                    typedef T                       state_t;
                    typedef basic_observer<state_t> observer_t;

                    /* I think that std::list is best container to contain observers, because:
                     *
                     *  - The order of items that are added is kept, so the user of this class
                     *    can handle the order of notifications.
                     *      - For now, I exclude a sort of the list and sorted addition of a
                     *        new items for simplicity.
                     *  - There is a need for successive accesses to all items from start to
                     *    end once, so it isn't the disadvantage that randome access is slow.
                     *  - Adding and removing items are fast.
                     *
                     *  */
                    typedef std::list<observer_t*>              observer_list_t;
                    typedef typename observer_list_t::iterator  observer_it;

                protected:
                    // the list of observers
                    observer_list_t observers;

                protected:
                    // the member function to get current (latest) states
                    virtual const state_t& state(void) const = 0;

                public:
                    // register an observer
                    inline basic_subject& attach(observer_t& o) {
                        observers.push_back(&o);
                        return *this;
                    }

                    // release an observer
                    inline basic_subject& detach(observer_t& o) {
                        observers.remove(&o);
                        return *this;
                    }

                    // notice the current states to all of observers
                    // Should we use std::for_each(3) ?
                    void notify(void) {
                        state_t s = state();
                        for (observer_it it = observers.begin(); it != observers.end(); ++it) {
                            (*it)->update(s);
                        }
                    }

                    // typical destructor
                    virtual ~basic_subject(void) {}
            };

        /*
         * The class to observe children of basic_subject class
         * To use, you must define the class or struct that is derived from this class.
         * */
        template<typename T>
            class basic_observer {
                public:
                    // typedefs
                    typedef T state_t;

                public:
                    // Objects of this class are identified by the memory address of its instance.
                    inline bool operator==(const basic_observer& rhs) const { return this == &rhs; }
                    inline bool operator!=(const basic_observer& rhs) const { return !(*this == rhs); }

                    // typical destructor
                    virtual ~basic_observer(void) {}

                public:
                    // virtual functions
                    virtual void update(const state_t& s) = 0;
            };
    }
}

#endif // OBSERBER_HPP

