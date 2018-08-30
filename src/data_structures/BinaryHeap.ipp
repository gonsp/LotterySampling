#include "data_structures/BinaryHeap.h"


namespace BinaryHeap {


using namespace std;

template<class Element, ComparatorFunction<Element> comparator_func, ClassField<Element, Locator> locator_field>
BinaryHeap<Element, comparator_func, locator_field>::BinaryHeap(unsigned int m) {
    v.reserve(m+1);
}

template<class Element, ComparatorFunction<Element> comparator_func, ClassField<Element, Locator> locator_field>
Element* BinaryHeap<Element, comparator_func, locator_field>::top() const {
    assert(!empty());
    return v.front();
}

template<class Element, ComparatorFunction<Element> comparator_func, ClassField<Element, Locator> locator_field>
Element* BinaryHeap<Element, comparator_func, locator_field>::pop() {
    return pop(0);
}

template<class Element, ComparatorFunction<Element> comparator_func, ClassField<Element, Locator> locator_field>
void BinaryHeap<Element, comparator_func, locator_field>::remove_element(Element* element) {
    pop(element->*locator_field);
}

template<class Element, ComparatorFunction<Element> comparator_func, ClassField<Element, Locator> locator_field>
Element* BinaryHeap<Element, comparator_func, locator_field>::pop(Locator locator) {
    assert(locator >= 0 and locator < this->size());
    Element* element = top();
    if(locator == this->size() - 1) {
        v.pop_back();
    } else {
        v[locator] = v.back();
        v.pop_back();
        key_updated(locator); // To regain the heap property
    }
    return element;
}

template<class Element, ComparatorFunction<Element> comparator_func, ClassField<Element, Locator> locator_field>
void BinaryHeap<Element, comparator_func, locator_field>::push(Element* element) {
    v.push_back(element);
    key_updated(this->size() - 1);
}

template<class Element, ComparatorFunction<Element> comparator_func, ClassField<Element, Locator> locator_field>
Element* BinaryHeap<Element, comparator_func, locator_field>::pop_and_push(Element* element) {
    assert(!empty());
    v.push_back(element);
    return pop();
}

template<class Element, ComparatorFunction<Element> comparator_func, ClassField<Element, Locator> locator_field>
void BinaryHeap<Element, comparator_func, locator_field>::key_updated(Element* element) {
    key_updated(element->*locator_field);
}

template<class Element, ComparatorFunction<Element> comparator_func, ClassField<Element, Locator> locator_field>
void BinaryHeap<Element, comparator_func, locator_field>::key_updated(Locator locator) {
    assert(locator >= 0 and locator < this->size());
    Element* element = v[locator];
    Locator new_locator = -1;
    Locator father = get_father_locator(locator);
    Locator min_child = get_min_child_locator(locator);
    if(father != -1 && comparator(v[locator], v[father])) {
        new_locator = father;
    } else if(min_child != -1 && comparator(v[min_child], v[locator])) {
        new_locator = min_child;
    }
    if(new_locator != -1) {
        v[locator] = v[new_locator];
        v[new_locator] = element;
        key_updated(new_locator);
    }
    v[locator]->*locator_field = locator;
}

template<class Element, ComparatorFunction<Element> comparator_func, ClassField<Element, Locator> locator_field>
Locator BinaryHeap<Element, comparator_func, locator_field>::get_father_locator(Locator locator) const {
    if(locator == 0) {
        return -1;
    }
    return (locator - 1)/2;
}

template<class Element, ComparatorFunction<Element> comparator_func, ClassField<Element, Locator> locator_field>
Locator BinaryHeap<Element, comparator_func, locator_field>::get_min_child_locator(Locator locator) const {
    Locator child_left = locator * 2 + 1;
    if(child_left < this->size()) {
        Locator child_right = child_left + 1;
        if(child_right < this->size() && comparator(v[child_right], v[child_left])) {
            return child_right;
        }
        return child_left;
    }
    return -1;
}

template<class Element, ComparatorFunction<Element> comparator_func, ClassField<Element, Locator> locator_field>
unsigned long BinaryHeap<Element, comparator_func, locator_field>::size() const {
    return v.size();
}

template<class Element, ComparatorFunction<Element> comparator_func, ClassField<Element, Locator> locator_field>
bool BinaryHeap<Element, comparator_func, locator_field>::empty() const {
    return v.empty();
}


}

