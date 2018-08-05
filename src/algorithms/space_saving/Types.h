#ifndef _Types_SpaceSaving_H_
#define _Types_SpaceSaving_H_

#include <list>
#include <string>

namespace SpaceSaving {

template <class T>
struct Element;

template <class T>
struct ElementLocator {
    typedef std::shared_ptr<Element<T>> type;
};

template <class T>
struct Bucket {
    int count;

    typedef typename std::list<typename ElementLocator<T>::type> ElementList;
    ElementList elements;

    Bucket(int count) {
        this->count = count;
    }
};

template <class T>
struct StreamSummary {
    typedef std::list<Bucket<T>> type;
};

template <class T>
struct Element {
    T id;
    int over_estimation;

    typedef typename StreamSummary<T>::type::iterator BucketIterator;
    typedef typename Bucket<T>::ElementList::iterator ElementIterator;

    BucketIterator bucket_iterator;
    ElementIterator element_iterator;

    Element(const T& id, int over_estimation) {
        this->id = id;
        this->over_estimation = over_estimation;
    }
};


}

#endif //_Types_SpaceSaving_H_
