#ifndef _SortedVector_H_
#define _SortedVector_H_

#include "utils/Misc.h"
#include <vector>
#include <list>

namespace SortedVector {


typedef unsigned int KeyType;

template<class Element>
using Iterator = typename std::vector<Element*>::iterator;

template<class Element>
using IteratorConst = typename std::vector<Element*>::const_iterator;

struct Bucket {
    KeyType key;
    unsigned int boundary;

    Bucket(KeyType key, unsigned int boundary) {
        this->key = key;
        this->boundary = boundary;
    }
};

typedef std::list<Bucket> BucketList;

typedef BucketList::iterator BucketListIterator;

struct Locator {
    unsigned int pos;
    BucketListIterator bucket_iterator;
};



template<class Element, ClassField<Element, Locator> locator_field>
class SortedVector {
// - Constant time insertion element with lowest key already in the container.
// - Linear time deletion arbitrary element (not implemented not used).
// - Constant time deletion element with lowest key.
// - Constant time key increment (1 unit).
// - Linear time arbitrary key modification (not implemented nor used).
// - Linear time ordered traversal through iterators.

private:

    std::vector<Element*> v;
    BucketList bucket_list;

    bool is_bucket_size_one(const Bucket& bucket) const;

public:

    SortedVector() {}

    SortedVector(unsigned int m);

    void insert_element(Element* element);

    Element* replace_last(Element* element);

    void replace_element(Element* replaced_element, Element* element);

    void increase_key(Element* element);

    KeyType get_key(Element* element) const;

    unsigned int size() const;

    IteratorConst<Element> begin() const;

    IteratorConst<Element> end() const;

    Iterator<Element> begin();

    Iterator<Element> end();
};


}

#include "data_structures/SortedVector.ipp"

#endif //_SortedVector_H_
