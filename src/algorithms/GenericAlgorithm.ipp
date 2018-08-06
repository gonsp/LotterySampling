#include <algorithms/GenericAlgorithm.h>

template <template<typename> class Element, class T>
void GenericAlgorithm<Element, T>::set_monitored_size(unsigned int m) {
    // TODO experiment with different max_load_factor
    monitored_elements.max_load_factor(1);
    monitored_elements.reserve(m);
}

template <template<typename> class Element, class T>
void GenericAlgorithm<Element, T>::process_element(const T& element_id) {
    ++N;
    typename MonitoredElements::iterator it = monitored_elements.find(element_id);
    if(it == monitored_elements.end()) { // element wasn't being sampled
        has_extra_element = true;
        it = monitored_elements.emplace(element_id, element_id).first; // Create instance of element
        if(!insert_element((*it).second)) {
            monitored_elements.erase(it); // Since the algorithm has chosen no to keep it in the sample, we remove it
        }
        has_extra_element = false;
    } else { // element was being sampled
        update_element(it->second);
    }
}

template <template<typename> class Element, class T>
Element<T>& GenericAlgorithm<Element, T>::get_element_reference(const T& element_id) {
    return monitored_elements.find(element_id)->second;
}

template <template<typename> class Element, class T>
void GenericAlgorithm<Element, T>::remove_element(const T& element_id) {
    monitored_elements.erase(element_id);
}

template <template<typename> class Element, class T>
unsigned int GenericAlgorithm<Element, T>::sample_size() {
    return monitored_elements.size() - (has_extra_element ? 1 : 0);
}
