#include "SortedTree.h"


namespace SortedTree {


using namespace std;

template<class Element, ComparatorFunction<Element> comparator_func, ClassField<Element, Iterator<Element, comparator_func>> iterator_field>
void SortedTree<Element, comparator_func, iterator_field>::insert_element(Element* element) {
    element->*iterator_field = this->insert(element);
}

template<class Element, ComparatorFunction<Element> comparator_func, ClassField<Element, Iterator<Element, comparator_func>> iterator_field>
void SortedTree<Element, comparator_func, iterator_field>::remove_element(Element* element) {
    this->erase(element->*iterator_field);
    element->*iterator_field = this->end();
}

template<class Element, ComparatorFunction<Element> comparator_func, ClassField<Element, Iterator<Element, comparator_func>> iterator_field>
template<class KeyType, class Field>
void SortedTree<Element, comparator_func, iterator_field>::update_key(Element* element, Field key_field, KeyType new_key) {
    // It's needed to remove and reinsert an element since there isn't an "update" method in multiset.
    SortedTreeIterator<Element, comparator_func> hint = next(element->*iterator_field);
    this->erase(element->*iterator_field);
    element->*key_field = new_key;
    element->*iterator_field = this->emplace_hint(hint, element);
}


}