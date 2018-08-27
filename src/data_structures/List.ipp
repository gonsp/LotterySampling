#include "List.h"


namespace List {


using namespace std;

template<class Element, ClassField<Element, Locator<Element>> locator_field>
void List<Element, locator_field>::insert_element(Element* element) {
    l.push_front(element);
    element->*locator_field = l.begin();
};

template<class Element, ClassField<Element, Locator<Element>> locator_field>
Element* List<Element, locator_field>::get_older() const {
    assert(!empty());
    return *prev(l.end());
};

template<class Element, ClassField<Element, Locator<Element>> locator_field>
Element* List<Element, locator_field>::pop_and_push(Element* element) {
    assert(!empty());
    Element* removed_element = get_older();
    *prev(l.end()) = element;
    move_to_front(removed_element);
    return removed_element;
};

template<class Element, ClassField<Element, Locator<Element>> locator_field>
void List<Element, locator_field>::move_to_front(Element* element) {
    assert(!empty());
    l.splice(l.begin(), l, element->*locator_field);
    element->*locator_field = l.begin();
}

template<class Element, ClassField<Element, Locator<Element>> locator_field>
Iterator<Element> List<Element, locator_field>::begin() {
    return l.begin();
};

template<class Element, ClassField<Element, Locator<Element>> locator_field>
Iterator<Element> List<Element, locator_field>::end() {
    return l.end();
};

template<class Element, ClassField<Element, Locator<Element>> locator_field>
unsigned long List<Element, locator_field>::size() const {
    return l.size();
};

template<class Element, ClassField<Element, Locator<Element>> locator_field>
bool List<Element, locator_field>::empty() const {
    return l.empty();
};


}