#ifndef _SortedList_H_
#define _SortedList_H_

#include "utils/Misc.h"
#include <list>

namespace SortedList {


using namespace std;

// Internal types

typedef unsigned int KeyType;

template<class Element>
using ElementList = list<Element*>;

template<class Element>
struct Bucket {
    KeyType key;
    ElementList<Element> elements;

    Bucket(KeyType key) {
        this->key = key;
    }
};

template<class Element>
using BucketList = list<Bucket<Element>>;




template<class Element>
struct Iterator : public iterator<bidirectional_iterator_tag, Element> {

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

template<class Element>
using Locator = Iterator<Element>;



template<class Element, ClassField<Element, Locator<Element>> locator_field>
class SortedList {
// - Constant time insertion element with lowest key already in the container.
// - Constant time deletion arbitrary element.
// - Constant time key increment (1 unit).
// - Linear time arbitrary key modification (not implemented nor used).
// - Linear time ordered traversal through iterators.

private:

    BucketList<Element> bucket_list;
    unsigned long element_count = 0;

public:

    void insert_element(Element* element);

    void remove_element(Element* element);

    Element* pop_back();

    Element* replace_back(Element* element);

    void increase_key(Element* element);

    KeyType get_key(Element* element) const;

    Iterator<Element> begin();

    Iterator<Element> end();

    unsigned long size() const;
};


}

#include "data_structures/SortedList.ipp"

#endif //_SortedList_H_
