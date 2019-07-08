#include "algorithms/lossy_counting/Algorithm.h"


namespace LossyCounting {


template<class T>
Algorithm<T>::Algorithm(const InputParser& parameters) {
    error = stod(parameters.get_parameter("-error"));
}

template<class T>
FrequencyOrder<Element<T>>& Algorithm<T>::get_frequency_order() {
    return frequency_order;
}

template<class T>
bool Algorithm<T>::insert_element(Element<T>& element) {

    unsigned int window = (unsigned int) ceil(this->N / ceil(1 / error));

    if(this->sample_size() > 0) {
        Element<T>* element_min_freq = *prev(frequency_order.end());
        if(element_min_freq->freq < window) {
            frequency_order.remove_element(element_min_freq);
            this->remove_element(element_min_freq->id);
        }
    }

    element.freq = window;
    frequency_order.insert_element(&element);
    return true;
}

template<class T>
void Algorithm<T>::update_element(Element<T>& element) {
    frequency_order.update_key(&element, &Element<T>::freq, element.freq + 1);
}


}