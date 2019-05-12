#include "algorithms/lottery_sampling/Algorithm.h"
#include "utils/TicketUtils.h"
#include <iostream>

namespace LotterySampling {


using namespace std;

template<class T>
Algorithm<T>::Algorithm(const InputParser& parameters) {
    m = (unsigned int) stoul(parameters.get_parameter("-m"));
    ticket_order = TicketOrder<Element<T>>(m);
    this->set_monitored_size(m);
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
    element.ticket = ticket_generator.generate_token();

    if(this->sample_size() < m) {
        frequency_order.insert_element(&element);
        ticket_order.push(&element);
//        element.over_estimation = 0;
    } else { // Max number of monitored elements is reached. This new one may replace the one with less hits
        if(element.ticket < ticket_order.top()->ticket) {
            return false;
        }

        Element<T>* removed_element = frequency_order.pop_and_push(&element);
        ticket_order.pop_and_push(removed_element, &element);
        this->remove_element(removed_element->id);

//        element.over_estimation = element.get_count();
        frequency_order.increment_key(&element);
    }
    return true;
}

template<class T>
void Algorithm<T>::update_element(Element<T>& element) {
    frequency_order.increment_key(&element);
    Token token = ticket_generator.generate_token();
    if(token > element.ticket) {
        element.ticket = token;
        ticket_order.key_updated(&element);
    }
}

template<class T>
double Algorithm<T>::get_threshold() const {
    return TicketUtils::normalize_ticket(ticket_order.top()->ticket);
}

template<class T>
void Algorithm<T>::print_state() {
    for(auto it = frequency_order.begin(); it != frequency_order.end(); ++it) {
        Element<T>* element = *it;
        cout << element->id << ", " << element->get_count() << ", " << TicketUtils::normalize_ticket(element->ticket);
        if(ticket_order.is_inside(element)) {
            cout << "*" << endl;
        } else {
            cout << endl;
        }
    }
    assert(frequency_order.size() == this->sample_size());
}


}