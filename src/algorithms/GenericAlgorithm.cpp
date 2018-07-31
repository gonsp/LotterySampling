template<class ElementLocator>
void GenericAlgorithm<ElementLocator>::remove_element(std::string& element_id) {
    monitored_elements.erase(element_id);
}

template<class ElementLocator>
ElementLocator&
GenericAlgorithm<ElementLocator>::get_locator(std::string& element_id) {
    return monitored_elements[element_id];
}

template<class ElementLocator>
void GenericAlgorithm<ElementLocator>::set_monitored_size(unsigned int m) {
    // TODO experiment with different max_load_factor
    monitored_elements.max_load_factor(1);
    monitored_elements.reserve(m);
}

template<class ElementLocator>
void GenericAlgorithm<ElementLocator>::process_element(std::string& element_id) {
    ++N;
    typename MonitoredElements::iterator it = monitored_elements.find(element_id);
    if(it == monitored_elements.end()) { // element wasn't being sampled
        ElementLocator locator = insert_element(element_id);
        if(locator.is_valid()) {
            monitored_elements[element_id] = locator;
        }
    } else { // element was being sampled
        update_element(it->second);
    }
}

template<class ElementLocator>
unsigned int GenericAlgorithm<ElementLocator>::sample_size() {
    return (int) (monitored_elements.size());
}
