#include "algorithms/lottery_space_saving/Algorithm.h"
#include <iostream>

namespace LotterySpaceSaving {


using namespace std;

template<class T>
Algorithm<T>::Algorithm(const InputParser& parameters) {
    m = (unsigned int) stoul(parameters.get_parameter("-m"));
    this->set_monitored_size(m);
    ticket_update = !parameters.has_parameter("-no_ticket_update");
    int seed;
    if(parameters.has_parameter("-seed")) {
        seed = stoi(parameters.get_parameter("-seed"));
    } else {
        seed = -1;
    }
    if(parameters.has_parameter("-threshold")) {
        threshold = stof(parameters.get_parameter("-threshold"));
    } else {
        threshold = -1;
    }
    ticket_generator = TicketGenerator(seed);
    mean_ticket = 0;
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
    element.ticket = ticket_generator.generate_ticket(this->N);

    if(this->sample_size() < m) {
        frequency_order.insert_element(&element);
        element.over_estimation = 0;
    } else { // Max number of monitored elements is reached. This new one may replace the one with less hits
        Element<T>* removed_element = *prev(frequency_order.end());
        bool is_inserted;
        if(threshold == -1) { // Not using fixed user defined threshold (default case), so the threshold will be increasing through the stream
            is_inserted = element.ticket >= removed_element->ticket || element.ticket >= mean_ticket;
        } else {
            is_inserted = element.ticket >= threshold * ticket_generator.MAX_TICKET;
        }
        if(is_inserted) {
            frequency_order.pop_and_push(&element);
            ticket_generator.decremental_averaging(mean_ticket, removed_element->ticket, this->sample_size());
            this->remove_element(removed_element->id);

            element.over_estimation = element.get_count();
            frequency_order.increment_key(&element);
        } else {
            return false;
        }
    }
    ticket_generator.incremental_averaging(mean_ticket, element.ticket, this->sample_size() + 1);
    return true;
}

template<class T>
void Algorithm<T>::update_element(Element<T>& element) {
    frequency_order.increment_key(&element);
    if(threshold == -1 and ticket_update) {
        Ticket ticket = ticket_generator.generate_ticket(this->N);
        if(ticket > element.ticket) {
            ticket_generator.decremental_averaging(mean_ticket, element.ticket, this->sample_size());
            ticket_generator.incremental_averaging(mean_ticket, ticket, this->sample_size());
            element.ticket = ticket;
        }
    }
}

template<class T>
float Algorithm<T>::get_threshold() const {
    return ticket_generator.normalize_ticket(mean_ticket, this->N);
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