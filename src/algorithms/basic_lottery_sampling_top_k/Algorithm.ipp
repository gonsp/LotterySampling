#include "algorithms/basic_lottery_sampling_top_k/Algorithm.h"
#include <math.h>
#include <iterator>


namespace BasicLotterySamplingTopK {


template<class T>
Algorithm<T>::Algorithm(const InputParser& parameters) {
    k = (unsigned int) stoul(parameters.get_parameter("-k"));
    delta = stod(parameters.get_parameter("-delta"));
    error = stod(parameters.get_parameter("-error"));
    r = log(k/delta);
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
    element.freq = 1;

    if(this->sample_size() < k) {
        frequency_order.insert_element(&element);
        if(this->sample_size() == k - 1) {
            k_th_freq_obs = 1;
            prev_count = k - element.frequency_order_locator.bucket_iterator->elements.size();
        }
    } else {
        Ticket threshold = Ticket(get_threshold() * TicketUtils::MAX_TICKET);
        if(element.ticket < threshold) {
            return false;
        }

        purge_sample(false);

        ticket_order.push(&element);
        frequency_order.insert_element(&element);
    }

    return true;
}

template<class T>
void Algorithm<T>::update_element(Element<T>& element) {

    Token token = ticket_generator.generate_token();

    if(ticket_order.is_inside(&element)) {
        if(element.freq == k_th_freq_obs) {
            Element<T>* old_heavy_hitter = element.frequency_order_locator.bucket_iterator->elements.back();
            // To place it next to the rest of non-heavy hitters inside the bucket:
            frequency_order.remove_element(old_heavy_hitter);
            frequency_order.insert_element(old_heavy_hitter, element.frequency_order_locator.bucket_iterator);
            assert(old_heavy_hitter->id != element.id);
            assert(!ticket_order.is_inside(old_heavy_hitter));
            ticket_order.pop_and_push(&element, old_heavy_hitter);
        } else if(token > element.ticket) {
            element.ticket = token;
            ticket_order.key_updated(&element);
        }
    }

    ++element.freq;
    frequency_order.increase_key(&element);

    if(!ticket_order.is_inside(&element)) {
        if(this->sample_size() >= k && element.freq - 1 == k_th_freq_obs && ++prev_count == k) {
            prev_count = k - element.frequency_order_locator.bucket_iterator->elements.size();
            k_th_freq_obs++;
        }
        if(token > element.ticket) {
            element.ticket = token;
        }
    }

    purge_sample(false);
}

template<class T>
void Algorithm<T>::purge_sample(bool lazy_removal) {
    Ticket threshold = Ticket(get_threshold() * TicketUtils::MAX_TICKET);
    for(int i = 0; !ticket_order.empty() && ticket_order.top()->ticket < threshold && (!lazy_removal || i < 2); ++i) {
        this->remove_element(ticket_order.pop()->id);
    }
}

template<class T>
double Algorithm<T>::get_threshold() const {
    double threshold = 1 - r / (sqrt(1 - error) * (1 - sqrt(1 - error)) * k_th_freq_obs);
    assert(threshold < 1); // This could happen in very long streams due to precision errors
    return max(threshold, 0.0);
}

template<class T>
unordered_map<string, double> Algorithm<T>::get_custom_stats() {
    unordered_map<string, double> stats;
    if(this->sample_size() < k) {
        stats["threshold"] = 0;
    } else {
        stats["threshold"] = get_threshold();
    }
    return stats;
}

template<class T>
QueryResults<T> Algorithm<T>::top_k_query(int k) {
    double support = sqrt(1 - error) * k_th_freq_obs / this->N;
    return this->frequent_query(support);
}


}
