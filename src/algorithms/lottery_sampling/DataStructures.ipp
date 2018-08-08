#include "DataStructures.h"

namespace LotterySampling {


using namespace std;

template<class Element>
TicketOrder<Element>::TicketOrder(unsigned int m) {
    v.reserve(m);
}

template<class Element>
Element* TicketOrder<Element>::top() const {

}

template<class Element>
Element* TicketOrder<Element>::pop() {

}

template<class Element>
void TicketOrder<Element>::push(Element* element) {

}

template<class Element>
Element* TicketOrder<Element>::pop_and_push(Element* element) {

}

template<class Element>
void TicketOrder<Element>::ticket_updated(iterator it) {

}

template<class Element>
void TicketOrder<Element>::remove_element(iterator it) {

}

template<class Element>
bool TicketOrder<Element>::empty() const {
    return size() == 0;
}

template<class Element>
unsigned int TicketOrder<Element>::size() const {

}


}

