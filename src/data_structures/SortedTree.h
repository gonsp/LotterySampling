#ifndef _SortedTree_LotterySampling_H_
#define _SortedTree_LotterySampling_H_

#include <set>
#include "utils/Misc.h"

template<class Element, ComparatorFunction<Element> comparator_func>
using SortedTreeIterator = typename std::multiset<Element*, PointerComparator<Element, comparator_func>>::iterator;

template<class Element, ComparatorFunction<Element> comparator_func, ClassField<Element, SortedTreeIterator<Element, comparator_func>> iterator_field>
class SortedTree : private std::multiset<Element*, PointerComparator<Element, comparator_func>> {
// - Logarithmic insertions of arbitrary keys.
// - Constant time deletion of arbitrary elements (through its iterator).
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

#include "SortedTree.ipp"

#endif //_SortedTree_LotterySampling_H_