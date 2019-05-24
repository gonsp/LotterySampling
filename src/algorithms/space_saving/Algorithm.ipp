#include "algorithms/space_saving/Algorithm.h"
#include <iostream>

namespace SpaceSaving {



template<class T>
Algorithm<T>::Algorithm(const InputParser& parameters) {
    m = (unsigned int) stoul(parameters.get_parameter("-m"));
    this->set_monitored_size(m);
    if(parameters.has_parameter("-threshold")) {
        threshold = stod(parameters.get_parameter("-threshold"));
    } else {
        threshold = 0;
    }
    int seed;
    if(parameters.has_parameter("-seed")) {
        seed = stoi(parameters.get_parameter("-seed"));
    } else {
        seed = -1;
    }
    ticket_generator = TicketUtils(seed);
}

template<class T>
FrequencyOrder<Element<T>>& Algorithm<T>::get_frequency_order() {
    return frequency_order;
}

template<class T>
bool Algorithm<T>::insert_element(Element<T>& element) {
    if(this->sample_size() < m) {
        frequency_order.insert_element(&element);
    } else { // Max number of monitored elements is reached. This new one will replace the one with less hits
        if(ticket_generator.generate_token() < threshold * TicketUtils::MAX_TICKET) {
            return false;
        }
        Element<T>* removed_element = frequency_order.replace_last(&element);
        this->remove_element(removed_element->id);

        frequency_order.increase_key(&element);
    }
    return true;
}

template<class T>
void Algorithm<T>::update_element(Element<T>& element) {
    frequency_order.increase_key(&element);
}

template<class T>
unordered_map<string, double> Algorithm<T>::get_custom_stats() const {
    unordered_map<string, double> stats;
    stats["threshold"] = threshold;
    return stats;
}


}