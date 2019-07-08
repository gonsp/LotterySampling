#ifndef _SortedTree_H_
#define _SortedTree_H_

#include "utils/Misc.h"
#include <set>

namespace SortedTree {
    
    
using namespace std;

template<class Element, ComparatorFunction<Element> comparator_func>
using Iterator = typename multiset<Element*, PointerComparator<Element, comparator_func>>::reverse_iterator;

template<class Element, ComparatorFunction<Element> comparator_func>
using Locator = typename multiset<Element*, PointerComparator<Element, comparator_func>>::iterator;

template<class Element, ComparatorFunction<Element> comparator_func, ClassField<Element, Locator<Element, comparator_func>> locator_field>
class SortedTree {
// - Logarithmic insertions of arbitrary keys.
// - Constant time deletion of arbitrary elements (through its locator).
// - Constant time key increment (1 unit).
// - Logarithmic time arbitrary key modification.
// - Linear time ordered traversal through iterators.

private:

    multiset<Element*, PointerComparator<Element, comparator_func>> s;

public:

    void insert_element(Element* element);

    void remove_element(Element* element);

    template<class KeyType, class Field>
    void update_key(Element* element, Field key_field, KeyType new_key);

    Element* back();

    unsigned long size() const;

    Iterator<Element, comparator_func> begin();

    Iterator<Element, comparator_func> end();
};


}

#include "data_structures/SortedTree.ipp"

#endif //_SortedTree_H_