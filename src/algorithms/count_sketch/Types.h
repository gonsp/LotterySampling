#ifndef _CountSketch_Types_H_
#define _CountSketch_Types_H_

#include "data_structures/SortedTree.h"
#include <vector>

namespace CountSketch {
    
    
using namespace std;

using Counters = vector<vector<int>>;

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


// TODO improve this (use random hash functions, the hash<int> doesn't work for this)
template<class T>
struct Hasher {

    hash<string> hasher;

    size_t operator()(T t) const {
        return hasher(to_string(t));
    }
};

template<>
size_t Hasher<string>::operator()(string t) const {
    return hasher(t);
}


}

#endif //_CountSketch_Types_H_
