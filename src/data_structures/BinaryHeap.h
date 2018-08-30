#ifndef _BinaryHeap_H_
#define _BinaryHeap_H_

#include "utils/Misc.h"
#include <vector>


namespace BinaryHeap {


typedef int Locator;

template<class Element, ComparatorFunction<Element> comparator_func, ClassField<Element, Locator> locator_field>
class BinaryHeap {
// - Logarithmic insertions and deletions of arbitrary elements and key values.
// - Logarithmic key modification.
// - Traversal not allowed. Only access to the smallest key's element.

private:

    std::vector<Element*> v;
    PointerComparator<Element, comparator_func> comparator;

    Element* pop(Locator locator);

    void key_updated(Locator locator);

    Locator get_father_locator(Locator locator) const;

    Locator get_min_child_locator(Locator locator) const;

public:

    BinaryHeap() {}

    BinaryHeap(unsigned int m);

    Element* top() const;

    Element* pop();

    void push(Element* element);

    Element* pop_and_push(Element* element);

    void remove_element(Element* element);

    void key_updated(Element* element);

    unsigned long size() const;

    bool empty() const;
};


}

#include "data_structures/BinaryHeap.ipp"

#endif //_BinaryHeap_H_