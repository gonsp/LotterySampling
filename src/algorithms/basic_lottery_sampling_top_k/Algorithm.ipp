#include "algorithms/basic_lottery_sampling_top_k/Algorithm.h"
#include <math.h>
#include <iterator>


namespace BasicLotterySamplingTopK {


template<class T>
Algorithm<T>::Algorithm(const InputParser& parameters) {
    k = (unsigned int) stoul(parameters.get_parameter("-k"));
    r = stod(parameters.get_parameter("-r"));
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
    element.freq = 1;
    element.ticket = ticket_generator.generate_token();

    if(this->sample_size() < k) {
        frequency_order.insert_element(&element);
    } else {
        Ticket threshold = Ticket(get_threshold() * TicketUtils::MAX_TICKET);
        if(element.ticket < threshold) {
            return false;
        }

        for(int i = 0; !ticket_order.empty() && ticket_order.top()->ticket < threshold && i < 2; ++i) {
            this->remove_element(ticket_order.pop()->id);
        }

        ticket_order.push(&element);
    }

    return true;
}

template<class T>
void Algorithm<T>::update_element(Element<T>& element) {
    Token token = ticket_generator.generate_token();
    element.freq++;

    if(ticket_order.is_inside(&element)) {
        if(element.freq > (*prev(frequency_order.end()))->freq) {
            Element<T>* replaced_element = frequency_order.replace_back(&element);
            ticket_order.pop_and_push(&element, replaced_element);
        } else if(token > element.ticket) {
            element.ticket = token;
            ticket_order.key_updated(&element);
        }
    }

    if(!ticket_order.is_inside(&element)) {
        frequency_order.increase_key(&element);
        if(token > element.ticket) {
            element.ticket = token;
        }
    }
}

template<class T>
double Algorithm<T>::get_threshold() const {
    double threshold = 1 - (1 / ((*prev(frequency_order.end()))->get_freq() / r));
    assert(threshold < 1); // This could happen in very long streams due to precision errors
    return max(threshold, 0.0);
}

template<class T>
unordered_map<string, double> Algorithm<T>::get_custom_stats() const {
    unordered_map<string, double> stats;
    if(this->sample_size() < k) {
        stats["threshold"] = 0;
    } else {
        stats["threshold"] = get_threshold();
    }
    return stats;
}


}
