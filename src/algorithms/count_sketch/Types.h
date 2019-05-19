#ifndef _CountSketch_Types_H_
#define _CountSketch_Types_H_

#include "data_structures/SortedTree.h"
#include <vector>

namespace CountSketch {


using Counters = std::vector<std::vector<int>>;

template<class Element>
using FrequencyOrder = SortedTree::SortedTree<Element, &Element::compare_freq, &Element::frequency_order_locator>;

template<class Element>
using FrequencyOrderLocator = SortedTree::Locator<Element, &Element::compare_freq>;


template<class T>
struct Element {

    T id;
    int freq;

    Element(const T& id) {
        this->id = id;
    }

    unsigned int get_freq() const {
        return freq;
    }

    bool compare_freq(const Element<T>& element) const {
        return this->freq < element.freq;
    }

    FrequencyOrderLocator<Element<T>> frequency_order_locator;
};


// TODO improve this (use random hash functions, the std::hash<int> doesn't work for this)
template<class T>
struct Hasher {

    std::hash<std::string> hasher;

    size_t operator()(T t) const {
        return hasher(std::to_string(t));
    }
};

template<>
size_t Hasher<std::string>::operator()(std::string t) const {
    return hasher(t);
}


}

#endif //_CountSketch_Types_H_
