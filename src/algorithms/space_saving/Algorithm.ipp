#include "algorithms/space_saving/Algorithm.h"
#include <iostream>

namespace SpaceSaving {


using namespace std;

template<class T>
Algorithm<T>::Algorithm(const InputParser& parameters) {
    m = (unsigned int) stoul(parameters.get_parameter("-m"));
    this->set_monitored_size(m);
    if(parameters.has_parameter("-threshold")) {
        threshold = stof(parameters.get_parameter("-threshold"));
    } else {
        threshold = -1;
    }
    int seed;
    if(parameters.has_parameter("-seed")) {
        seed = stoi(parameters.get_parameter("-seed"));
    } else {
        seed = -1;
    }
    ticket_generator = TicketGenerator(seed);
}

template<class T>
FrequencyOrderIterator<Element<T>> Algorithm<T>::frequency_order_begin() {
    return frequency_order.begin();
}

template<class T>
FrequencyOrderIterator<Element<T>> Algorithm<T>::frequency_order_end() {
    return frequency_order.end();
}

template<class T>
bool Algorithm<T>::insert_element(Element<T>& element) {
    if(this->sample_size() < m) {
        frequency_order.insert_element(&element);
        element.over_estimation = 0;
    } else { // Max number of monitored elements is reached. This new one will replace the one with less hits
        bool is_inserted = threshold == -1 || ticket_generator.generate_ticket() >= threshold * ticket_generator.MAX_TICKET;
        if(!is_inserted) {
            return false;
        }
        Element<T>* removed_element = frequency_order.pop_and_push(&element);
        this->remove_element(removed_element->id);

        element.over_estimation = frequency_order.get_key(&element);
        frequency_order.increment_key(&element);
    }
    return true;
}

template<class T>
void Algorithm<T>::update_element(Element<T>& element) {
    frequency_order.increment_key(&element);
}

template<class T>
void Algorithm<T>::print_state() {
    for(auto it = frequency_order.begin(); it != frequency_order.end(); ++it) {
        Element<T>* element = *it;
        cout << element->id << ", " << element->get_count() << ", " << element->over_estimation << endl;
    }
    assert(frequency_order.size() == this->sample_size());
}

template<class T>
float Algorithm<T>::get_threshold() const {
    if(threshold == -1) {
        return 0;
    } else {
        return threshold;
    }
}


}