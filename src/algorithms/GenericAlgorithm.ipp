template <class T, class ElementLocator>
void GenericAlgorithm<T, ElementLocator>::remove_element(const T& element_id) {
    monitored_elements.erase(element_id);
}

template <class T, class ElementLocator>
ElementLocator& GenericAlgorithm<T, ElementLocator>::get_locator(const T& element_id) {
    return monitored_elements[element_id];
}

template <class T, class ElementLocator>
void GenericAlgorithm<T, ElementLocator>::set_monitored_size(unsigned int m) {
    // TODO experiment with different max_load_factor
    monitored_elements.max_load_factor(1);
    monitored_elements.reserve(m);
}

template <class T, class ElementLocator>
void GenericAlgorithm<T, ElementLocator>::process_element(const T& element_id) {
    ++N;
    typename MonitoredElements::iterator it = monitored_elements.find(element_id);
    if(it == monitored_elements.end()) { // element wasn't being sampled
        ElementLocator locator;
        if(insert_element(element_id, locator)) {
            monitored_elements[element_id] = locator;
        }
    } else { // element was being sampled
        update_element(it->second);
    }
}

template <class T, class ElementLocator>
unsigned int GenericAlgorithm<T, ElementLocator>::sample_size() {
    return (int) (monitored_elements.size());
}
