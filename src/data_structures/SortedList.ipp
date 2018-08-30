#include "data_structures/SortedList.h"


namespace SortedList {


using namespace std;

template<class Element, ClassField<Element, Locator<Element>> locator_field>
void SortedList<Element, locator_field>::insert_element(Element* element) {
    if(bucket_list.empty() || prev(bucket_list.end())->key != 1) {
        // There are no buckets or the smallest one has key greater than 1
        bucket_list.emplace_back(1);
    }
    Iterator<Element> iterator = Iterator<Element>(&bucket_list);
    iterator.bucket_iterator = prev(bucket_list.end());
    iterator.bucket_iterator->elements.emplace_back(element);
    iterator.element_iterator = prev(iterator.bucket_iterator->elements.end());
    element->*locator_field = iterator;
    ++element_count;
}

template<class Element, ClassField<Element, Locator<Element>> locator_field>
void SortedList<Element, locator_field>::remove_element(Element* element) {
    Iterator<Element> iterator = element->*locator_field;
    iterator.bucket_iterator->elements.erase(iterator.element_iterator);
    if(iterator.bucket_iterator->elements.empty()) {
        bucket_list.erase(iterator.bucket_iterator);
    }
    --element_count;
};

template<class Element, ClassField<Element, Locator<Element>> locator_field>
void SortedList<Element, locator_field>::pop_back() {
    assert(!bucket_list.empty());
    remove_element(prev(end()));
};

template<class Element, ClassField<Element, Locator<Element>> locator_field>
Element* SortedList<Element, locator_field>::pop_and_push(Element* element) {
    assert(!bucket_list.empty());
    Iterator<Element>& iterator = element->*locator_field;
    iterator.bucket_iterator = prev(bucket_list.end());
    iterator.element_iterator = iterator.bucket_iterator->elements.begin(); // We select the oldest element with less hits
    Element* removed_element = *iterator.element_iterator;

    // Replacing the old element
    *iterator.element_iterator = element;

    return removed_element;
}

template<class Element, ClassField<Element, Locator<Element>> locator_field>
void SortedList<Element, locator_field>::increment_key(Element* element) {
    Iterator<Element>& iterator = element->*locator_field;
    int new_key = iterator.bucket_iterator->key + 1;
    if(iterator.bucket_iterator == bucket_list.begin() || prev(iterator.bucket_iterator)->key != new_key) { // It is the highest bucket or the next bucket doesn't have the required key
        if(iterator.bucket_iterator->elements.size() == 1) { // We can reuse the current bucket
            iterator.bucket_iterator->key = new_key;
            return;
        } else { // A new bucket needs to be created since there are other elements in the current bucket
            bucket_list.emplace(iterator.bucket_iterator, new_key);
        }
    }
    iterator.bucket_iterator->elements.erase(iterator.element_iterator);
    typename BucketList<Element>::iterator old_bucket = iterator.bucket_iterator;
    --iterator.bucket_iterator;
    if(old_bucket->elements.empty()) {
        bucket_list.erase(old_bucket);
    }
    iterator.bucket_iterator->elements.push_back(element);
    iterator.element_iterator = prev(iterator.bucket_iterator->elements.end());
}

template<class Element, ClassField<Element, Locator<Element>> locator_field>
KeyType SortedList<Element, locator_field>::get_key(Element* element) const {
    return (element->*locator_field).bucket_iterator->key;
}

template<class Element, ClassField<Element, Locator<Element>> locator_field>
Iterator<Element> SortedList<Element, locator_field>::begin() {
    Iterator<Element> iterator(&bucket_list);
    iterator.bucket_iterator = bucket_list.begin();
    if(!bucket_list.empty()) {
        iterator.element_iterator = iterator.bucket_iterator->elements.begin();
    }
    return iterator;
}

template<class Element, ClassField<Element, Locator<Element>> locator_field>
Iterator<Element> SortedList<Element, locator_field>::end() {
    Iterator<Element> iterator(&bucket_list);
    iterator.bucket_iterator = bucket_list.end();
    return iterator;
}

template<class Element, ClassField<Element, Locator<Element>> locator_field>
unsigned long SortedList<Element, locator_field>::size() const {
    return element_count;
};

template<class Element>
Iterator<Element> Iterator<Element>::Iterator::operator++() {
    assert(bucket_list != nullptr);
    if(bucket_iterator != bucket_list->end()) {
        if(++element_iterator == bucket_iterator->elements.end()) {
            if(++bucket_iterator != bucket_list->end()) {
                element_iterator = bucket_iterator->elements.begin();
            }
        }
    }
    return *this;
}

template<class Element>
Iterator<Element> Iterator<Element>::operator--() {
    assert(bucket_list != nullptr);
    if(bucket_iterator == bucket_list->end()) {
        if(!bucket_list->empty()) {
            bucket_iterator = prev(bucket_list->end());
            element_iterator = prev(bucket_iterator->elements.end());
        }
    } else if(element_iterator != bucket_iterator->elements.begin()) {
        --element_iterator;
    } else {
        if(bucket_iterator != bucket_list->begin()) {
            --bucket_iterator;
            element_iterator = prev(bucket_iterator->elements.end());
        }
    }
    return *this;
}

template<class Element>
Element*& Iterator<Element>::operator*() const {
    assert(bucket_list != nullptr);
    return *element_iterator;
}

template<class Element>
bool Iterator<Element>::operator==(const Iterator<Element>& iterator) {
    assert(bucket_list != nullptr);
    if(this->bucket_iterator == bucket_list->end() && iterator.bucket_iterator == bucket_list->end()) {
        return true;
    } else {
        return this->bucket_iterator == iterator.bucket_iterator && this->element_iterator == iterator.element_iterator;
    }
}

template<class Element>
bool Iterator<Element>::operator!=(const Iterator<Element>& iterator) {
    return !(*this == iterator);
}


}
