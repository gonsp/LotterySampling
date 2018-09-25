#include "algorithms/lottery_cache_sampling/Algorithm.h"
#include <iostream>

namespace LotteryCacheSampling {


using namespace std;

template<class T>
Algorithm<T>::Algorithm(const InputParser& parameters) {
    m = (unsigned int) stoul(parameters.get_parameter("-m"));
    this->set_monitored_size(m);
    bool aging = parameters.has_parameter("-aging");
    int seed;
    if(parameters.has_parameter("-seed")) {
        seed = stoi(parameters.get_parameter("-seed"));
    } else {
        seed = -1;
    }
    ticket_generator = TicketGenerator(aging, seed);
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

    // An element is inserted if:
    // - There are less than the maximum sampled elements or
    // - Its ticket is higher than the sampled element with older hit or
    // - Its ticket is higher than the mean tickets of the sampled elements.

    if(this->sample_size() < m) {
        cache_order.insert_element(&element);
        frequency_order.insert_element(&element);
        element.over_estimation = 0;
    } else {
        Element<T>* older_hit = cache_order.get_older();
        if(element.ticket >= older_hit->ticket || element.ticket >= mean_ticket) {
            ticket_generator.decremental_averaging(mean_ticket, older_hit->ticket, this->sample_size());
            cache_order.pop_and_push(&element);
            frequency_order.replace(older_hit, &element);
            element.over_estimation = older_hit->get_count();
            frequency_order.increment_key(&element);
            this->remove_element(older_hit->id);
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
    cache_order.move_to_front(&element);

    Ticket ticket = ticket_generator.generate_ticket(this->N);
    if(ticket > element.ticket) {
        ticket_generator.decremental_averaging(mean_ticket, element.ticket, this->sample_size());
        ticket_generator.incremental_averaging(mean_ticket, ticket, this->sample_size());
        element.ticket = ticket;
    }
}

template<class T>
float Algorithm<T>::get_threshold() const {
    return mean_ticket / float(ticket_generator.MAX_TICKET);
}

template<class T>
void Algorithm<T>::print_state() {
    cout << "Cache order:" << endl;
    Ticket real_mean_ticket = 0;
    unsigned int i = 0;
    for(auto it = cache_order.begin(); it != cache_order.end(); ++it) {
        Element<T>* element = *it;
        cout << element->id << ", " << element->ticket << endl;
        ticket_generator.incremental_averaging(real_mean_ticket, element->ticket, ++i);
    }
    cout << "Frequency order: " << endl;
    for(auto it = frequency_order.begin(); it != frequency_order.end(); ++it) {
        Element<T>* element = *it;
        cout << element->id << ", " << (*it)->get_count() << ", " << element->over_estimation << endl;
    }
    assert(cache_order.size() == this->sample_size());
    assert(frequency_order.size() == this->sample_size());
    cout << "Mean ticket " << mean_ticket << endl;
    cout << "Real mean ticket " << real_mean_ticket << endl;
}


}