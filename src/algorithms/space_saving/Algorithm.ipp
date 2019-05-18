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
//        element.over_estimation = 0;
    } else { // Max number of monitored elements is reached. This new one will replace the one with less hits
        if(ticket_generator.generate_token() < threshold * TicketUtils::MAX_TICKET) {
            return false;
        }
        Element<T>* removed_element = frequency_order.replace_last(&element);
        this->remove_element(removed_element->id);

//        element.over_estimation = frequency_order.get_key(&element);
        frequency_order.increase_key(&element);
    }
    return true;
}

template<class T>
void Algorithm<T>::update_element(Element<T>& element) {
    frequency_order.increase_key(&element);
}

template<class T>
void Algorithm<T>::print_state() {
    for(auto it = frequency_order.begin(); it != frequency_order.end(); ++it) {
        Element<T>* element = *it;
        cout << element->id << ", " << element->get_count() << endl; //<< ", " << element->over_estimation << endl;
    }
    assert(frequency_order.size() == this->sample_size());
}

template<class T>
double Algorithm<T>::get_threshold() const {
    return threshold;
}

template<class T>
float Algorithm<T>::get_frequency_threshold(float f) const {
    float error = 1 / m;
    assert(error < f / 2);
    return f - error;
}


}