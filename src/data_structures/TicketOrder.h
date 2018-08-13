#ifndef _TicketOrder_LotterySampling_H_
#define _TicketOrder_LotterySampling_H_

#include <vector>


template<class Element>
class TicketOrder : private std::vector<Element*> { // Implementation of a min binary heap to represent the order by ticket value of sampled elements, allowing the update of the ticktet
// - Logarithmic insertions and deletions of arbitrary elements and ticket values.
// - Logarithmic ticket modification.
// - Traversal not allowed. Only access to the smallest ticket's element.

public:
    typedef int iterator;

private:
    Element* pop(iterator it);

    void ticket_updated(iterator it);

    iterator get_father_iterator(iterator it) const;

    iterator get_min_child_iterator(iterator it) const;

public:
    TicketOrder() {}

    TicketOrder(unsigned int m);

    Element* top() const;

    Element* pop();

    void push(Element* element);

    Element* pop_and_push(Element* element);

    void remove_element(Element* element);

    void ticket_updated(Element* element);

    using std::vector<Element*>::empty;

    using std::vector<Element*>::size;
};


#include "TicketOrder.ipp"

#endif //_TicketOrder_LotterySampling_H_