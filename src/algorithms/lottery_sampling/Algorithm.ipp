#include "algorithms/lottery_sampling/Algorithm.h"


namespace LotterySampling {


template<class T>
Algorithm<T>::Algorithm(const InputParser& parameters) {
    m = (unsigned int) stoul(parameters.get_parameter("-m"));
    if(parameters.has_parameter("-phi")) {
        phi = stod(parameters.get_parameter("-phi"));
    } else {
        phi = -1;
    }
    ticket_order = TicketOrder<Element<T>>(m);
    leading_ones = 0;
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
void Algorithm<T>::process_element(const T& element_id) {
    Ticket old_threshold = this->sample_size() > 0 ? ticket_order.top()->ticket : 0;

    GenericAlgorithm<Element, T, FrequencyOrder<Element<T>>>::process_element(element_id);

    Ticket new_threshold = ticket_order.top()->ticket;
    if(new_threshold > old_threshold && this->sample_size() == m && phi == -1) {
        int remaining_leading_ones = TicketUtils::get_leading_ones(new_threshold);
        if(remaining_leading_ones > 0) {
            for(auto it = frequency_order.begin(); it != frequency_order.end(); ++it) {
                TicketUtils::scale_ticket((*it)->ticket, remaining_leading_ones);
            }
            leading_ones += remaining_leading_ones;
        }
    }
}

template<class T>
bool Algorithm<T>::insert_element(Element<T>& element) {
    pair<bool, Token> scaled_token = ticket_generator.generate_token(leading_ones);
    element.ticket = scaled_token.second;

    if(this->sample_size() < m) {
        assert(scaled_token.first);
        frequency_order.insert_element(&element);
        ticket_order.push(&element);
    } else { // Max number of monitored elements is reached. This new one may replace the one with less hits
        Ticket threshold;
        if(phi == -1) {
            threshold = ticket_order.top()->ticket;
        } else {
            assert(scaled_token.first);
            threshold = TicketUtils::estimate_ticket(floor(phi * this->N));
        }

        if(!scaled_token.first || element.ticket < threshold) {
            return false;
        }

        Element<T>* removed_element = frequency_order.replace_back(&element);
        ticket_order.pop_and_push(removed_element, &element);
        this->remove_element(removed_element->id);

        frequency_order.increase_key(&element);
    }
    return true;
}

template<class T>
void Algorithm<T>::update_element(Element<T>& element) {
    frequency_order.increase_key(&element);
    pair<bool, Token> scaled_token = ticket_generator.generate_token(leading_ones);
    if(scaled_token.first && scaled_token.second > element.ticket) {
        element.ticket = scaled_token.second;
        ticket_order.key_updated(&element);
    }
}

template<class T>
double Algorithm<T>::get_frequency_threshold(double f) const {
    if(phi != -1) {
        assert(f == phi);
    }
    return f;
}

template<class T>
unordered_map<string, double> Algorithm<T>::get_custom_stats() const {
    unordered_map<string, double> stats;
    if(phi == -1) {
        stats["threshold"] = this->sample_size() > 0 ? TicketUtils::normalize_ticket(ticket_order.top()->ticket) : 0;
    } else {
        stats["threshold"] = TicketUtils::normalize_ticket(TicketUtils::estimate_ticket(floor(phi * this->N)));
    }
    return stats;
}


}