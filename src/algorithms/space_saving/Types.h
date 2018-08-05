#ifndef _Types_SpaceSaving_H_
#define _Types_SpaceSaving_H_

#include <list>
#include <string>

namespace SpaceSaving {

template <class T>
struct Element {
    T id;
    int over_estimation;

    Element(const T& id, int over_estimation) {
        this->id = id;
        this->over_estimation = over_estimation;
    }
};

template <class T>
struct Bucket {
    int count;
    std::list<Element<T>> elements;

    Bucket(int count) {
        this->count = count;
    }
};

template <class T>
struct StreamSummary {
    typedef std::list<Bucket<T>> type;
};

template <class T>
struct ElementLocator {
    typename StreamSummary<T>::type::iterator bucket_iterator;
    typename std::list<Element<T>>::iterator element_iterator;
};


}

#endif //_Types_SpaceSaving_H_
