#ifndef _SortedList_H_
#define _SortedList_H_

#include <list>
#include "utils/Misc.h"


namespace SortedList {

// Internal types

typedef unsigned int KeyType;

template<class Element>
using ElementList = std::list<Element*>;

template<class Element>
struct Bucket {
    KeyType key;
    ElementList<Element> elements;

    Bucket(KeyType key) {
        this->key = key;
    }
};

template<class Element>
using BucketList = std::list<Bucket<Element>>;




template<class Element>
struct Iterator {

private:
    BucketList<Element>* bucket_list;

public:
    typename BucketList<Element>::iterator bucket_iterator;
    typename ElementList<Element>::iterator element_iterator;

    Iterator() {
        this->bucket_list = nullptr;
    }

    Iterator(BucketList<Element>* bucket_list) {
        this->bucket_list = bucket_list;
    }

    Element*& operator*() const;

    Iterator operator++();

    Iterator operator--();

    bool operator==(const Iterator& iterator);

    bool operator!=(const Iterator& iterator);
};


template<class Element, ClassField<Element, Iterator<Element>> iterator_field>
class SortedList {
// - Constant time insertion element with lowest key already in the container.
// - Constant time deletion arbitrary element.
// - Constant time key increment (1 unit).
// - Linear time arbitrary key modification (not implemented nor used).
// - Linear time ordered traversal through iterators.

private:
    BucketList<Element> bucket_list;

public:

    void insert_element(Element* element);

    Element* pop_and_push(Element* element);

    void increment_key(Element* element);

    KeyType get_key(Element* element) const;

    Iterator<Element> begin();

    Iterator<Element> end();
};


}

#include "SortedList.ipp"

#endif //_SortedList_H_
