#include "TicketOrder.h"


template<class Element>
TicketOrder<Element>::TicketOrder(unsigned int m) {
    this->reserve(m+1);
}

template<class Element>
Element* TicketOrder<Element>::top() const {
    assert(!empty());
    return this->front();
}

template<class Element>
Element* TicketOrder<Element>::pop() {
    return pop(0);
}

template<class Element>
void TicketOrder<Element>::remove_element(Element* element) {
    pop(element->ticket_order_iterator);
}

template<class Element>
Element* TicketOrder<Element>::pop(iterator it) {
    assert(it >= 0 and it < this->size());
    Element* element = top();
    if(it == this->size() - 1) {
        this->pop_back();
    } else {
        (*this)[it] = this->back();
        this->pop_back();
        ticket_updated(it); // To regain the heap property
    }
    return element;
}

template<class Element>
void TicketOrder<Element>::push(Element* element) {
    this->push_back(element);
    ticket_updated(this->size() - 1);
}

template<class Element>
Element* TicketOrder<Element>::pop_and_push(Element* element) {
    assert(!empty());
    this->push_back(element);
    return pop();
}

template<class Element>
void TicketOrder<Element>::ticket_updated(Element* element) {
    ticket_updated(element->ticket_order_iterator);
}

template<class Element>
void TicketOrder<Element>::ticket_updated(iterator it) {
    assert(it >= 0 and it < this->size());
    Element* element = (*this)[it];
    iterator new_it = -1;
    iterator father = get_father_iterator(it);
    iterator min_child = get_min_child_iterator(it);
    if(father != -1 && (*this)[it]->ticket < (*this)[father]->ticket) {
        new_it = father;
    } else if(min_child != -1 && (*this)[min_child]->ticket < (*this)[it]->ticket) {
        new_it = min_child;
    }
    if(new_it != -1) {
        (*this)[it] = (*this)[new_it];
        (*this)[new_it] = element;
        ticket_updated(new_it);
    }
    (*this)[it]->ticket_order_iterator = it;
}

template<class Element>
typename TicketOrder<Element>::iterator TicketOrder<Element>::get_father_iterator(iterator it) const {
    if(it == 0) {
        return -1;
    }
    return (it - 1)/2;
}

template<class Element>
typename TicketOrder<Element>::iterator TicketOrder<Element>::get_min_child_iterator(iterator it) const {
    iterator child_left = it * 2 + 1;
    if(child_left < this->size()) {
        iterator child_right = it * 2 + 2;
        if(child_right < this->size() && (*this)[child_right]->ticket < (*this)[child_left]->ticket) {
            return child_right;
        }
        return child_left;
    }
    return -1;
}

