#include "algorithms/sticky_sampling/Algorithm.h"
#include <math.h>
#include <list>


namespace StickySampling {


template<class T>
Algorithm<T>::Algorithm(const InputParser& parameters) {
    phi = stod(parameters.get_parameter("-phi"));
    delta = stod(parameters.get_parameter("-delta"));
    error = stod(parameters.get_parameter("-error"));
    r = 1;
    t = (unsigned int) ceil((1 / error) * log(1 / (phi * delta)));
    next_resampling = 2 * t;
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
    if(this->N == next_resampling) {
        next_resampling *= 2;
        r *= 2;
        list<Element<T>*> removed_elements;
        for(auto it = frequency_order.begin(); it != frequency_order.end(); ++it) {
            Element<T>* element = *it;
            unsigned int new_freq = element->get_freq();
            bool first = true;
            // Naive implementation. Not optimized.
            // Following procedure described for CountingSamples because I believe the one explained in StickySampling is wrong.
            while(new_freq > 0 and !(first ? toss_coin(0.5) : toss_coin(1 / double(r)))) {
                new_freq--;
                first = false;
            }
            if(new_freq == 0) {
                removed_elements.push_back(element);
            } else {
                frequency_order.update_key(element, &Element<T>::freq, new_freq);
            }
        }
        for(auto it = removed_elements.begin(); it != removed_elements.end(); ++it) {
            frequency_order.remove_element(*it);
            this->remove_element((*it)->id);
        }
    }
    GenericAlgorithm<Element, T, FrequencyOrder<Element<T>>>::process_element(element_id);
}

template<class T>
bool Algorithm<T>::insert_element(Element<T>& element) {
    if(toss_coin(1 / double(r))) {
        element.freq = 1;
        frequency_order.insert_element(&element);
        return true;
    } else {
        return false;
    }
}

template<class T>
void Algorithm<T>::update_element(Element<T>& element) {
    frequency_order.update_key(&element, &Element<T>::freq, element.freq + 1);
}

template<class T>
double Algorithm<T>::get_frequency_threshold(double f) const {
    assert(f == phi);
    return phi - error;
}

template<class T>
bool Algorithm<T>::toss_coin(double p) {
    return TicketUtils::normalize_ticket(ticket_generator.generate_token()) <= p;
}


}
