#include "algorithms/lottery_space_saving/Algorithm.h"
#include "utils/TicketUtils.h"
#include <iostream>

namespace LotterySpaceSaving {


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
    ticket_generator = TicketUtils(seed);
    mean_ticket = 0;
}

template<class T>
FrequencyOrder<Element<T>>& Algorithm<T>::get_frequency_order() {
    return frequency_order;
}

template<class T>
bool Algorithm<T>::insert_element(Element<T>& element) {
    element.ticket = ticket_generator.generate_ticket();

    if(this->sample_size() < m) {
        frequency_order.insert_element(&element);
//        element.over_estimation = 0;
    } else { // Max number of monitored elements is reached. This new one may replace the one with less hits
        Element<T>* removed_element = *prev(frequency_order.end());
        bool is_inserted = element.ticket >= removed_element->ticket || element.ticket >= mean_ticket;
        if(is_inserted) {
            frequency_order.pop_and_push(&element);
            TicketUtils::decremental_averaging(mean_ticket, removed_element->ticket, this->sample_size());
            this->remove_element(removed_element->id);

//            element.over_estimation = element.get_count();
            frequency_order.increment_key(&element);
        } else {
            return false;
        }
    }
    TicketUtils::incremental_averaging(mean_ticket, element.ticket, this->sample_size() + 1);
    return true;
}

template<class T>
void Algorithm<T>::update_element(Element<T>& element) {
    frequency_order.increment_key(&element);
    Ticket ticket = ticket_generator.generate_ticket();
    if(ticket > element.ticket) {
        TicketUtils::decremental_averaging(mean_ticket, element.ticket, this->sample_size());
        TicketUtils::incremental_averaging(mean_ticket, ticket, this->sample_size());
        element.ticket = ticket;
    }
}

template<class T>
float Algorithm<T>::get_threshold() const {
    return TicketUtils::normalize_ticket(mean_ticket);
}

template<class T>
void Algorithm<T>::print_state() {
    for(auto it = frequency_order.begin(); it != frequency_order.end(); ++it) {
        Element<T>* element = *it;
        cout << element->id << ", " << element->get_count() << ", " << endl; // << element->over_estimation << endl;
    }
    assert(frequency_order.size() == this->sample_size());
}


}