#include "SortedList.h"


template<class Element>
void SortedList<Element>::insert_element(Element* element) {
    if(bucket_list.empty() || std::prev(bucket_list.end())->freq != 1) {
        // There are no buckets or the smallest one has count greater than 1
        bucket_list.emplace_back(1);
    }
    std::prev(bucket_list.end())->elements.emplace_back(element);
    element->frequency_order_iterator = std::prev(end());
}

template<class Element>
Element* SortedList<Element>::pop_and_push(Element* element) {
    assert(!bucket_list.empty());
    Iterator& iterator = element->frequency_order_iterator;
    iterator.bucket_iterator = std::prev(bucket_list.end());
    iterator.element_iterator = iterator.bucket_iterator->elements.begin(); // We select the oldest element with less hits
    Element* removed_element = *iterator.element_iterator;

    // Replacing the old element
    *iterator.element_iterator = element;

    return removed_element;
}

template<class Element>
void SortedList<Element>::increment_frequency(Element* element) {
    Iterator& iterator = element->frequency_order_iterator;
    int new_freq = iterator.bucket_iterator->freq + 1;
    if(iterator.bucket_iterator == bucket_list.begin() || std::prev(iterator.bucket_iterator)->freq != new_freq) { // It is the highest bucket or the next bucket doesn't have the required count
        if(iterator.bucket_iterator->elements.size() == 1) { // We can reuse the current bucket
            iterator.bucket_iterator->freq++;
            return;
        } else { // A new bucket needs to be created since there are other elements in the current bucket
            bucket_list.emplace(iterator.bucket_iterator, new_freq);
        }
    }
    iterator.bucket_iterator->elements.erase(iterator.element_iterator);
    typename BucketList::iterator old_bucket = iterator.bucket_iterator;
    --iterator.bucket_iterator;
    if(old_bucket->elements.empty()) {
        bucket_list.erase(old_bucket);
    }
    iterator.bucket_iterator->elements.push_back(element);
    iterator.element_iterator = std::prev(iterator.bucket_iterator->elements.end());
}

template<class Element>
unsigned int SortedList<Element>::get_frequency(Element* element) const {
    return element->frequency_order_iterator.bucket_iterator->freq;
}

template<class Element>
typename SortedList<Element>::Iterator SortedList<Element>::begin() {
    Iterator iterator(this);
    iterator.bucket_iterator = bucket_list.begin();
    if(!bucket_list.empty()) {
        iterator.element_iterator = iterator.bucket_iterator->elements.begin();
    }
    return iterator;
}

template<class Element>
typename SortedList<Element>::Iterator SortedList<Element>::end() {
    Iterator iterator(this);
    iterator.bucket_iterator = bucket_list.end();
    return iterator;
}

template<class Element>
typename SortedList<Element>::Iterator SortedList<Element>::Iterator::operator++() {
    assert(sorted_list != nullptr);
    if(bucket_iterator != sorted_list->bucket_list.end()) {
        if(++element_iterator == bucket_iterator->elements.end()) {
            if(++bucket_iterator != sorted_list->bucket_list.end()) {
                element_iterator = bucket_iterator->elements.begin();
            }
        }
    }
    return *this;
}

template<class Element>
typename SortedList<Element>::Iterator SortedList<Element>::Iterator::operator--() {
    assert(sorted_list != nullptr);
    if(bucket_iterator == sorted_list->bucket_list.end()) {
        if(!sorted_list->bucket_list.empty()) {
            bucket_iterator = std::prev(sorted_list->bucket_list.end());
            element_iterator = std::prev(bucket_iterator->elements.end());
        }
    } else if(element_iterator != bucket_iterator->elements.begin()) {
        --element_iterator;
    } else {
        if(bucket_iterator != sorted_list->bucket_list.begin()) {
            --bucket_iterator;
            element_iterator = std::prev(bucket_iterator->elements.end());
        }
    }
    return *this;
}

template<class Element>
Element*& SortedList<Element>::Iterator::operator*() const {
    assert(sorted_list != nullptr);
    return *element_iterator;
}

template<class Element>
bool SortedList<Element>::Iterator::operator==(const Iterator& iterator) {
    assert(sorted_list != nullptr);
    if(this->bucket_iterator == this->sorted_list->bucket_list.end() && iterator.bucket_iterator == this->sorted_list->bucket_list.end()) {
        return true;
    } else {
        return this->bucket_iterator == iterator.bucket_iterator && this->element_iterator == iterator.element_iterator;
    }
}

template<class Element>
bool SortedList<Element>::Iterator::operator!=(const Iterator& iterator) {
    return !(*this == iterator);
}
