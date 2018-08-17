#include "BinaryHeap.h"


namespace BinaryHeap {


using namespace std;

template<class Element, ComparatorFunction<Element> comparator_func, ClassField<Element, Iterator> iterator_field>
BinaryHeap<Element, comparator_func, iterator_field>::BinaryHeap(unsigned int m) {
    v.reserve(m+1);
}

template<class Element, ComparatorFunction<Element> comparator_func, ClassField<Element, Iterator> iterator_field>
Element* BinaryHeap<Element, comparator_func, iterator_field>::top() const {
    assert(!empty());
    return v.front();
}

template<class Element, ComparatorFunction<Element> comparator_func, ClassField<Element, Iterator> iterator_field>
Element* BinaryHeap<Element, comparator_func, iterator_field>::pop() {
    return pop(0);
}

template<class Element, ComparatorFunction<Element> comparator_func, ClassField<Element, Iterator> iterator_field>
void BinaryHeap<Element, comparator_func, iterator_field>::remove_element(Element* element) {
    pop(element->*iterator_field);
}

template<class Element, ComparatorFunction<Element> comparator_func, ClassField<Element, Iterator> iterator_field>
Element* BinaryHeap<Element, comparator_func, iterator_field>::pop(Iterator it) {
    assert(it >= 0 and it < this->size());
    Element* element = top();
    if(it == this->size() - 1) {
        v.pop_back();
    } else {
        v[it] = v.back();
        v.pop_back();
        key_updated(it); // To regain the heap property
    }
    return element;
}

template<class Element, ComparatorFunction<Element> comparator_func, ClassField<Element, Iterator> iterator_field>
void BinaryHeap<Element, comparator_func, iterator_field>::push(Element* element) {
    v.push_back(element);
    key_updated(this->size() - 1);
}

template<class Element, ComparatorFunction<Element> comparator_func, ClassField<Element, Iterator> iterator_field>
Element* BinaryHeap<Element, comparator_func, iterator_field>::pop_and_push(Element* element) {
    assert(!empty());
    v.push_back(element);
    return pop();
}

template<class Element, ComparatorFunction<Element> comparator_func, ClassField<Element, Iterator> iterator_field>
void BinaryHeap<Element, comparator_func, iterator_field>::key_updated(Element* element) {
    key_updated(element->*iterator_field);
}

template<class Element, ComparatorFunction<Element> comparator_func, ClassField<Element, Iterator> iterator_field>
void BinaryHeap<Element, comparator_func, iterator_field>::key_updated(Iterator it) {
    assert(it >= 0 and it < this->size());
    Element* element = v[it];
    Iterator new_it = -1;
    Iterator father = get_father_iterator(it);
    Iterator min_child = get_min_child_iterator(it);
    if(father != -1 && comparator(v[it], v[father])) {
        new_it = father;
    } else if(min_child != -1 && comparator(v[min_child], v[it])) {
        new_it = min_child;
    }
    if(new_it != -1) {
        v[it] = v[new_it];
        v[new_it] = element;
        key_updated(new_it);
    }
    v[it]->*iterator_field = it;
}

template<class Element, ComparatorFunction<Element> comparator_func, ClassField<Element, Iterator> iterator_field>
Iterator BinaryHeap<Element, comparator_func, iterator_field>::get_father_iterator(Iterator it) const {
    if(it == 0) {
        return -1;
    }
    return (it - 1)/2;
}

template<class Element, ComparatorFunction<Element> comparator_func, ClassField<Element, Iterator> iterator_field>
Iterator BinaryHeap<Element, comparator_func, iterator_field>::get_min_child_iterator(Iterator it) const {
    Iterator child_left = it * 2 + 1;
    if(child_left < this->size()) {
        Iterator child_right = it * 2 + 2;
        if(child_right < this->size() && comparator(v[child_right], v[child_left])) {
            return child_right;
        }
        return child_left;
    }
    return -1;
}

template<class Element, ComparatorFunction<Element> comparator_func, ClassField<Element, Iterator> iterator_field>
unsigned long BinaryHeap<Element, comparator_func, iterator_field>::size() const {
    return v.size();
}

template<class Element, ComparatorFunction<Element> comparator_func, ClassField<Element, Iterator> iterator_field>
bool BinaryHeap<Element, comparator_func, iterator_field>::empty() const {
    return v.empty();
}


}

