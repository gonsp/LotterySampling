#include "algorithms/GenericAlgorithm.h"

template<template<typename> class Element, class T, class FrequencyOrder>
void GenericAlgorithm<Element, T, FrequencyOrder>::set_monitored_size(unsigned int m) {
    // TODO experiment with different max_load_factor
    monitored_elements.max_load_factor(1);
    monitored_elements.reserve(m);
}

template<template<typename> class Element, class T, class FrequencyOrder>
void GenericAlgorithm<Element, T, FrequencyOrder>::process_element(const T& element_id) {
    ++N;
    typename MonitoredElements::iterator it = monitored_elements.find(element_id);
    if(it == monitored_elements.end()) { // element wasn't being sampled
        it = monitored_elements.emplace(element_id, element_id).first; // Create instance of element
        if(!insert_element(it->second)) {
            monitored_elements.erase(it); // Since the algorithm has chosen no to keep it in the sample, we remove it
        } else {
            ++m;
        }
    } else { // element was being sampled
        update_element(it->second);
    }
}

template<template<typename> class Element, class T, class FrequencyOrder>
void GenericAlgorithm<Element, T, FrequencyOrder>::frequent_query(float f, std::ostream& stream) {
    for(auto it = get_frequency_order().begin(); it != get_frequency_order().end() && (*it)->get_count() >= get_frequency_threshold(f) * N; ++it) {
        Element<T>* element = *it;
        stream << element->id << " " << element->get_count() / float(N) << std::endl;
    }
};

template<template<typename> class Element, class T, class FrequencyOrder>
void GenericAlgorithm<Element, T, FrequencyOrder>::top_k_query(int k, std::ostream& stream) {
    for(auto it = get_frequency_order().begin(); it != get_frequency_order().end() && k-- > 0; ++it) {
        Element<T>* element = *it;
        stream << element->id << " " << element->get_count() / float(N) << std::endl;
    }
};

template<template<typename> class Element, class T, class FrequencyOrder>
void GenericAlgorithm<Element, T, FrequencyOrder>::remove_element(const T& element_id) {
    monitored_elements.erase(element_id);
    --m;
}

template<template<typename> class Element, class T, class FrequencyOrder>
unsigned int GenericAlgorithm<Element, T, FrequencyOrder>::sample_size() const {
    return m;
}
