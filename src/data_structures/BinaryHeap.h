#ifndef _BinaryHeap_LotterySampling_H_
#define _BinaryHeap_LotterySampling_H_

#include <vector>
#include "utils/Misc.h"


namespace BinaryHeap {


typedef int Iterator;

template<class Element, ComparatorFunction<Element> comparator_func, ClassField<Element, Iterator> iterator_field>
class BinaryHeap {
// - Logarithmic insertions and deletions of arbitrary elements and ticket values.
// - Logarithmic ticket modification.
// - Traversal not allowed. Only access to the smallest ticket's element.

private:
    std::vector<Element*> v;
    PointerComparator<Element, comparator_func> comparator;

    Element* pop(Iterator it);

    void key_updated(Iterator it);

    Iterator get_father_iterator(Iterator it) const;

    Iterator get_min_child_iterator(Iterator it) const;

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


#include "BinaryHeap.ipp"

#endif //_BinaryHeap_LotterySampling_H_