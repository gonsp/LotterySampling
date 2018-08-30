#include "data_structures/SortedTree.h"


namespace SortedTree {


using namespace std;

template<class Element, ComparatorFunction<Element> comparator_func, ClassField<Element, Locator<Element, comparator_func>> locator_field>
void SortedTree<Element, comparator_func, locator_field>::insert_element(Element* element) {
    element->*locator_field = s.insert(element);
}

template<class Element, ComparatorFunction<Element> comparator_func, ClassField<Element, Locator<Element, comparator_func>> locator_field>
void SortedTree<Element, comparator_func, locator_field>::remove_element(Element* element) {
    s.erase(element->*locator_field);
}

template<class Element, ComparatorFunction<Element> comparator_func, ClassField<Element, Locator<Element, comparator_func>> locator_field>
template<class KeyType, class Field>
void SortedTree<Element, comparator_func, locator_field>::update_key(Element* element, Field key_field, KeyType new_key) {
    // It's needed to remove and reinsert an element since there isn't an "update" method in multiset.
    Locator<Element, comparator_func> hint = next(element->*locator_field);
    s.erase(element->*locator_field);
    element->*key_field = new_key;
    element->*locator_field = s.emplace_hint(hint, element);
}

template<class Element, ComparatorFunction<Element> comparator_func, ClassField<Element, Locator<Element, comparator_func>> locator_field>
Iterator<Element, comparator_func> SortedTree<Element, comparator_func, locator_field>::begin() {
    return s.rbegin();
};

template<class Element, ComparatorFunction<Element> comparator_func, ClassField<Element, Locator<Element, comparator_func>> locator_field>
Iterator<Element, comparator_func> SortedTree<Element, comparator_func, locator_field>::end() {
    return s.rend();
};

template<class Element, ComparatorFunction<Element> comparator_func, ClassField<Element, Locator<Element, comparator_func>> locator_field>
unsigned long SortedTree<Element, comparator_func, locator_field>::size() const {
    return s.size();
};


}