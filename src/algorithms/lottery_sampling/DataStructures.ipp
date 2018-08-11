#include "DataStructures.h"

namespace LotterySampling {


using namespace std;

template<class Element>
TicketOrder<Element>::TicketOrder(unsigned int m) {
    v.reserve(m+1);
}

template<class Element>
Element* TicketOrder<Element>::top() const {
    assert(!empty());
    return v.front();
}

template<class Element>
Element* TicketOrder<Element>::pop() {
    return pop(0);
}

template<class Element>
Element* TicketOrder<Element>::pop(iterator it) {
    assert(it >= 0 and it < v.size());
    Element* element = top();
    if(it == v.size() - 1) {
        v.pop_back();
    } else {
        v[it] = v.back();
        v.pop_back();
        ticket_updated(it); // To regain the heap property
    }
    return element;
}

template<class Element>
void TicketOrder<Element>::push(Element* element) {
    v.push_back(element);
    ticket_updated(v.size() - 1);
}

template<class Element>
Element* TicketOrder<Element>::pop_and_push(Element* element) {
    assert(!empty());
    v.push_back(element);
    return pop();
}

template<class Element>
void TicketOrder<Element>::ticket_updated(iterator it) {
    assert(it >= 0 and it < v.size());
    Element* element = v[it];
    iterator new_it = -1;
    iterator father = get_father_iterator(it);
    iterator min_child = get_min_child_iterator(it);
    if(father != -1 && v[it]->ticket < v[father]->ticket) {
        new_it = father;
    } else if(min_child != -1 && v[min_child]->ticket < v[it]->ticket) {
        new_it = min_child;
    }
    if(new_it != -1) {
        v[it] = v[new_it];
        v[new_it] = element;
        ticket_updated(new_it);
    }
    v[it]->ticket_iterator = it;
}

template<class Element>
bool TicketOrder<Element>::empty() const {
    return size() == 0;
}

template<class Element>
unsigned int TicketOrder<Element>::size() const {
    return v.size();
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
    if(child_left < v.size()) {
        iterator child_right = it * 2 + 2;
        if(child_right < v.size() && v[child_right]->ticket < v[child_left]->ticket) {
            return child_right;
        }
        return child_left;
    }
    return -1;
}


}

