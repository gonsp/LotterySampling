#include "Algorithm.h"
#include <iostream>

namespace LotteryCacheSampling {


using namespace std;

template<class T>
Algorithm<T>::Algorithm(const InputParser& parameters) {
    m = (unsigned int) stoul(parameters.get_parameter("-m"));
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
void Algorithm<T>::frequent_query(float f, ostream& stream) {
    for(auto it = frequency_order.begin(); it != frequency_order.end() && (*it)->freq >= f * this->N; ++it) {
        stream << (*it)->id << " " << (*it)->freq / (float) this->N << endl;
    }
}

template<class T>
void Algorithm<T>::k_top_query(int k, ostream& stream) {
    for(auto it = frequency_order.begin(); it != frequency_order.end() && k-- > 0; ++it) {
        stream << (*it)->id << " " << (*it)->freq / (float) this->N << endl;
    }
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
        element.initial_estimated_freq = 0;
    } else {
        Element<T>* older_hit = cache_order.get_older();
        if(element.ticket >= older_hit->ticket || element.ticket >= mean_ticket) {
            cache_order.pop_and_push(&element);
            ticket_generator.decremental_averaging(mean_ticket, older_hit->ticket, this->sample_size());
            frequency_order.remove_element(older_hit);
            // TODO fill this
            element.initial_estimated_freq = 0;
            this->remove_element(older_hit->id);
        } else {
            return false;
        }
    }
    ticket_generator.incremental_averaging(mean_ticket, element.ticket, this->sample_size() + 1);
    frequency_order.insert_element(&element);
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
        cout << element->id << ", " << frequency_order.get_key(element) << ", " << element->initial_estimated_freq << endl;
    }
    assert(cache_order.size() == this->sample_size());
    assert(frequency_order.size() == this->sample_size());
    cout << "Mean ticket " << mean_ticket << endl;
    cout << "Real mean ticket " << real_mean_ticket << endl;
}


}