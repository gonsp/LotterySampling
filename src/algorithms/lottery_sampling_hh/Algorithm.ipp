#include "algorithms/lottery_sampling_hh/Algorithm.h"


namespace LotterySamplingHH {


template<class T>
Algorithm<T>::Algorithm(const InputParser& parameters) {
    m = (unsigned int) stoul(parameters.get_parameter("-m"));
    this->set_monitored_size(m);
    phi = stod(parameters.get_parameter("-phi"));
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
        element.freq = 1;
        frequency_order.insert_element(&element);
    } else {
        unsigned int freq_threshold = floor(phi * this->N);
        Token token = ticket_generator.generate_token();
        if(token < TicketUtils::estimate_ticket(freq_threshold)) {
            return false;
        }

        Element<T>* element_min_freq = *prev(frequency_order.end());
        frequency_order.remove_element(element_min_freq);
        this->remove_element(element_min_freq->id);

        element.freq = freq_threshold + 1;
        frequency_order.insert_element(&element);
    }
    return true;
}

template<class T>
void Algorithm<T>::update_element(Element<T>& element) {
    frequency_order.update_key(&element, &Element<T>::freq, element.freq + 1);
}

template<class T>
double Algorithm<T>::get_frequency_threshold(double f) const {
    return f;
}

template<class T>
unordered_map<string, double> Algorithm<T>::get_custom_stats() const {
    unordered_map<string, double> stats;
    stats["threshold"] = TicketUtils::normalize_ticket(TicketUtils::estimate_ticket(floor(phi * this->N)));
    return stats;
}


}