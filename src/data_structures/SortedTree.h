#ifndef _SortedTree_H_
#define _SortedTree_H_

#include <set>
#include "utils/Misc.h"

namespace SortedTree {


template<class Element, ComparatorFunction<Element> comparator_func>
using Iterator = typename std::multiset<Element*, PointerComparator<Element, comparator_func>>::iterator;

template<class Element, ComparatorFunction<Element> comparator_func>
using Locator = Iterator<Element, comparator_func>;

template<class Element, ComparatorFunction<Element> comparator_func, ClassField<Element, Locator<Element, comparator_func>> locator_field>
class SortedTree : private std::multiset<Element*, PointerComparator<Element, comparator_func>> {
// - Logarithmic insertions of arbitrary keys.
// - Constant time deletion of arbitrary elements (through its locator).
// - Constant time key increment (1 unit).
// - Logarithmic time arbitrary key modification.
// - Linear time ordered traversal through iterators.

public:

    void insert_element(Element* element);

    void remove_element(Element* element);

    template<class KeyType, class Field>
    void update_key(Element* element, Field key_field, KeyType new_key);

    using std::multiset<Element*, PointerComparator<Element, comparator_func>>::size;

    using std::multiset<Element*, PointerComparator<Element, comparator_func>>::begin;

    using std::multiset<Element*, PointerComparator<Element, comparator_func>>::end;

    using std::multiset<Element*, PointerComparator<Element, comparator_func>>::rbegin;

    using std::multiset<Element*, PointerComparator<Element, comparator_func>>::rend;
};


}

#include "SortedTree.ipp"

#endif //_SortedTree_H_