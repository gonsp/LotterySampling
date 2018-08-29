#include <algorithms/GenericAlgorithm.h>

template<template<typename> class Element, class T, class FrequencyOrderIterator>
void GenericAlgorithm<Element, T, FrequencyOrderIterator>::set_monitored_size(unsigned int m) {
    // TODO experiment with different max_load_factor
    monitored_elements.max_load_factor(1);
    monitored_elements.reserve(m);
}

template<template<typename> class Element, class T, class FrequencyOrderIterator>
void GenericAlgorithm<Element, T, FrequencyOrderIterator>::process_element(const T& element_id) {
    ++N;
    typename MonitoredElements::iterator it = monitored_elements.find(element_id);
    if(it == monitored_elements.end()) { // element wasn't being sampled
        has_extra_element = true;
        it = monitored_elements.emplace(element_id, element_id).first; // Create instance of element
        if(!insert_element(it->second)) {
            monitored_elements.erase(it); // Since the algorithm has chosen no to keep it in the sample, we remove it
        }
        has_extra_element = false;
    } else { // element was being sampled
        update_element(it->second);
    }
}

template<template<typename> class Element, class T, class FrequencyOrderIterator>
void GenericAlgorithm<Element, T, FrequencyOrderIterator>::frequent_query(float f, std::ostream& stream) {
    for(FrequencyOrderIterator it = frequency_order_begin(); it != frequency_order_end() && (*it)->get_freq() >= f * N; ++it) {
        Element<T>* element = *it;
        stream << element->id << " " << element->get_freq() / float(N) << std::endl;
    }
};

template<template<typename> class Element, class T, class FrequencyOrderIterator>
void GenericAlgorithm<Element, T, FrequencyOrderIterator>::k_top_query(int k, std::ostream& stream) {
    for(FrequencyOrderIterator it = frequency_order_begin(); it != frequency_order_end() && k-- > 0; ++it) {
        Element<T>* element = *it;
        stream << element->id << " " << element->get_freq() / float(N) << std::endl;
    }
};

template<template<typename> class Element, class T, class FrequencyOrderIterator>
void GenericAlgorithm<Element, T, FrequencyOrderIterator>::remove_element(const T& element_id) {
    monitored_elements.erase(element_id);
}

template<template<typename> class Element, class T, class FrequencyOrderIterator>
unsigned int GenericAlgorithm<Element, T, FrequencyOrderIterator>::sample_size() const {
    return monitored_elements.size() - (has_extra_element ? 1 : 0);
}
