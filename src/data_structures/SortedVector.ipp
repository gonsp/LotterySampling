#include "data_structures/SortedVector.h"
#include <assert.h>

namespace SortedVector {


template<class Element, ClassField<Element, Locator> locator_field>
SortedVector<Element, locator_field>::SortedVector(unsigned int m) {
    v.reserve(m);
}
template<class Element, ClassField<Element, Locator> locator_field>
void SortedVector<Element, locator_field>::insert_element(Element* element) {
    v.push_back(element);
    Locator& locator = element->*locator_field;
    locator.pos = v.size() - 1;
    if(bucket_list.empty() || prev(bucket_list.end())->key != 1) {
        // There are no buckets or the smallest one has key greater than 1
        bucket_list.emplace_back(1, locator.pos);
    }
    locator.bucket_iterator = prev(bucket_list.end());
}

template<class Element, ClassField<Element, Locator> locator_field>
Element* SortedVector<Element, locator_field>::pop_back() {
    Element* removed_element = v.back();
    BucketListIterator bucket = (removed_element->*locator_field).bucket_iterator;
    if(is_bucket_size_one(*bucket)) {
        bucket_list.erase(bucket);
    }
    v.pop_back();
    return removed_element;
}

template<class Element, ClassField<Element, Locator> locator_field>
Element* SortedVector<Element, locator_field>::replace_back(Element* element) {
    assert(!bucket_list.empty());
    Element* removed_element = v[v.size() - 1];
    replace_element(removed_element, element);
    return removed_element;
}

template<class Element, ClassField<Element, Locator> locator_field>
void SortedVector<Element, locator_field>::replace_element(Element* replaced_element, Element* element) {
    assert(!bucket_list.empty());
    element->*locator_field = replaced_element->*locator_field;
    v[(element->*locator_field).pos] = element;
}

template<class Element, ClassField<Element, Locator> locator_field>
Element* SortedVector<Element, locator_field>::back() {
    return *prev(end());
}

template<class Element, ClassField<Element, Locator> locator_field>
void SortedVector<Element, locator_field>::increase_key(Element* element) {
    Locator& locator = element->*locator_field;
    bool bucket_has_size_one = is_bucket_size_one(*locator.bucket_iterator);
    int new_key = locator.bucket_iterator->key + 1;

    if(locator.pos != locator.bucket_iterator->boundary) {
        // We move the element at the beginning of the bucket
        swap(v[locator.pos], v[locator.bucket_iterator->boundary]);
        swap(locator.pos, (v[locator.pos]->*locator_field).pos);
    }

    if(locator.bucket_iterator == bucket_list.begin() || prev(locator.bucket_iterator)->key != new_key) { // It is the highest bucket or the next bucket doesn't have the required key
        if(bucket_has_size_one) { // We can reuse the current bucket
            locator.bucket_iterator->key = new_key;
            return;
        } else { // A new bucket needs to be created since there are other elements in the current bucket
            bucket_list.emplace(locator.bucket_iterator, new_key, locator.bucket_iterator->boundary);
        }
    }

    BucketListIterator old_bucket = locator.bucket_iterator;
    locator.bucket_iterator = prev(locator.bucket_iterator);
    if(bucket_has_size_one) {
        bucket_list.erase(old_bucket);
    } else {
        old_bucket->boundary++;
    }
}

template<class Element, ClassField<Element, Locator> locator_field>
KeyType SortedVector<Element, locator_field>::get_key(Element* element) const {
    return (element->*locator_field).bucket_iterator->key;
}

template<class Element, ClassField<Element, Locator> locator_field>
bool SortedVector<Element, locator_field>::is_bucket_size_one(const Bucket& bucket) const {
    return bucket.boundary == v.size() - 1 || (v[bucket.boundary + 1]->*locator_field).bucket_iterator->boundary != bucket.boundary;
}

template<class Element, ClassField<Element, Locator> locator_field>
unsigned int SortedVector<Element, locator_field>::size() const {
    return v.size();
}

template<class Element, ClassField<Element, Locator> locator_field>
IteratorConst<Element> SortedVector<Element, locator_field>::begin() const {
    return v.begin();
}

template<class Element, ClassField<Element, Locator> locator_field>
IteratorConst<Element> SortedVector<Element, locator_field>::end() const {
    return v.end();
}

template<class Element, ClassField<Element, Locator> locator_field>
Iterator<Element> SortedVector<Element, locator_field>::begin() {
    return v.begin();
}

template<class Element, ClassField<Element, Locator> locator_field>
Iterator<Element> SortedVector<Element, locator_field>::end() {
    return v.end();
}


}
