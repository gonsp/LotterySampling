#include "algorithms/lottery_sampling/Algorithm.h"
#include <iostream>

namespace LotterySampling {


using namespace std;

template<class T>
Algorithm<T>::Algorithm(const InputParser& parameters) {
    m = (unsigned int) stoul(parameters.get_parameter("-m"));
    this->set_monitored_size(m);
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
    element.ticket = ticket_generator.generate_ticket();

    if(this->sample_size() < m) {
        frequency_order.insert_element(&element);
        ticket_order.push(&element);
        element.over_estimation = 0;
    } else { // Max number of monitored elements is reached. This new one may replace the one with less hits
        if(element.ticket < ticket_order.top()->ticket) {
            return false;
        }

        Element<T>* removed_element = *prev(frequency_order.end());
        frequency_order.pop_and_push(&element);
        ticket_order.replace_element(removed_element, &element);
        this->remove_element(removed_element->id);

        element.over_estimation = element.get_count();
        frequency_order.increment_key(&element);
    }
    return true;
}

template<class T>
void Algorithm<T>::update_element(Element<T>& element) {
    frequency_order.increment_key(&element);
    Ticket ticket = ticket_generator.generate_ticket();
    if(ticket > element.ticket) {
        element.ticket = ticket;
        ticket_order.key_updated(&element);
    }
}

template<class T>
float Algorithm<T>::get_threshold() const {
    return ticket_generator.normalize_ticket(ticket_order.top()->ticket);
}

template<class T>
void Algorithm<T>::print_state() {
    for(auto it = frequency_order.begin(); it != frequency_order.end(); ++it) {
        Element<T>* element = *it;
        cout << element->id << ", " << element->get_count() << ", " << element->over_estimation << endl;
    }
    assert(frequency_order.size() == this->sample_size());
}


}