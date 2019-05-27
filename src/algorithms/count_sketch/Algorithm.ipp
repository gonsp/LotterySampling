#include "algorithms/count_sketch/Algorithm.h"
#include <algorithm>


namespace CountSketch {


template<class T>
Algorithm<T>::Algorithm(const InputParser& parameters, bool count_min) {
    m = (unsigned int) stoul(parameters.get_parameter("-m"));
    this->set_monitored_size(m);
    h = (unsigned int) stoul(parameters.get_parameter("-h"));
    q = (unsigned int) stoul(parameters.get_parameter("-q"));
    assert(h % 2 == 1);
    this->count_min = count_min;
    counters = Counters(h, vector<int>(q, 0));
}

template<class T>
FrequencyOrder<Element<T>>& Algorithm<T>::get_frequency_order() {
    return frequency_order;
}

template<class T>
int Algorithm<T>::update_count(Element<T>& element) {
    vector<int> element_counters = vector<int>(h);
    size_t id_hash = element_hasher(element.id);
    for(int i = 0; i < h; ++i) {
        unsigned int index = hash_hasher(id_hash + i) % q;
        int increment;
        if(count_min) {
            increment = 1;
        } else {
            increment = hash_hasher(id_hash + h + i) % 2 == 0 ? -1 : 1;
        }
        counters[i][index] += increment;
        element_counters[i] = counters[i][index];
    }
    if(count_min) {
        return *min_element(element_counters.begin(), element_counters.end());
    } else {
        nth_element(element_counters.begin(), element_counters.begin() + h/2, element_counters.end());
        return element_counters[h/2];
    }
}

template<class T>
bool Algorithm<T>::insert_element(Element<T>& element) {
    element.freq = max(1, update_count(element));

    if(this->sample_size() < m) {
        frequency_order.insert_element(&element);
    } else {
        Element<T>* removed_element = *prev(frequency_order.end());
        bool is_inserted = removed_element->get_freq() < element.get_freq();
        if(!is_inserted) {
            return false;
        }
        frequency_order.remove_element(removed_element);
        frequency_order.insert_element(&element);
        this->remove_element(removed_element->id);
    }
    return true;
}

template<class T>
void Algorithm<T>::update_element(Element<T>& element) {
    frequency_order.update_key(&element, &Element<T>::freq, element.freq + 1);
    update_count(element);
}


}