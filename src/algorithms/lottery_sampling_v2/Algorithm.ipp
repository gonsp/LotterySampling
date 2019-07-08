#include "algorithms/lottery_sampling_v2/Algorithm.h"

namespace LotterySamplingV2 {


template<class T>
Algorithm<T>::Algorithm(const InputParser& parameters) {
    k = (unsigned int) stoul(parameters.get_parameter("-k"));
    m = (unsigned int) stoul(parameters.get_parameter("-m"));
    max_k_th_f_obs = 0;
    assert(k <= m);
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

    if(this->sample_size() < m) {
        element.freq = 1;
    } else { // Max number of monitored elements is reached. This new one may replace the one with less f'
        max_k_th_f_obs = max(get_k_th_obs_freq(k), max_k_th_f_obs);
        Ticket threshold = TicketUtils::estimate_ticket(max_k_th_f_obs);

        if(ticket_generator.generate_token() < threshold) {
            return false;
        }

        element.freq = max_k_th_f_obs + 1;

        Element<T>* removed_element = frequency_order.back();
        frequency_order.remove_element(removed_element);
        obs_frequency_order.remove_element(removed_element);
        this->remove_element(removed_element->id);
    }

    frequency_order.insert_element(&element);
    obs_frequency_order.insert_element(&element);
    return true;
}

template<class T>
unsigned int Algorithm<T>::get_k_th_obs_freq(int k) {
    unsigned int obs_freq = 0;
    int i = 0;
    for(auto it = obs_frequency_order.begin(); it != obs_frequency_order.end() && i < k; ++it, ++i) {
        obs_freq = (*it)->get_obs_freq();
    }
    return obs_freq;
}

template<class T>
void Algorithm<T>::update_element(Element<T>& element) {
    frequency_order.update_key(&element, &Element<T>::freq, element.freq + 1);
    obs_frequency_order.increase_key(&element);
    max_k_th_f_obs = max(get_k_th_obs_freq(k), max_k_th_f_obs);
}

template<class T>
unordered_map<string, double> Algorithm<T>::get_custom_stats() {
    unordered_map<string, double> stats;
    if(this->sample_size() < m) {
        stats["threshold"] = 0;
    } else {
        stats["threshold"] = TicketUtils::normalize_ticket(TicketUtils::estimate_ticket(max_k_th_f_obs));
    }
    return stats;
}


}