#include "algorithms/count_sketch/Algorithm.h"
#include <iostream>

namespace CountSketch {


using namespace std;

template<class T>
Algorithm<T>::Algorithm(const InputParser& parameters, bool count_min) {
    m = (unsigned int) stoul(parameters.get_parameter("-m"));
    this->set_monitored_size(m);
    h = (unsigned int) stoul(parameters.get_parameter("-h"));
    if(h % 2 == 1) {
        cerr << "h needs to be even" << endl;
        exit(1);
    }
    this->count_min = count_min;
    counters = Counters(h, vector<int>(m, 0));
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
        unsigned int index = hash_hasher(id_hash + i) % m;
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
        std::nth_element(element_counters.begin(), element_counters.begin() + h/2, element_counters.end());
        return element_counters[h/2];
    } else {
        return *std::min_element(element_counters.begin(), element_counters.end());
    }
}

template<class T>
bool Algorithm<T>::insert_element(Element<T>& element) {
    element.count = max(1, update_count(element));

    if(this->sample_size() < m) {
        frequency_order.insert_element(&element);
    } else {
        Element<T>* removed_element = *prev(frequency_order.end());
        bool is_inserted = removed_element->get_count() < element.get_count();
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
    frequency_order.update_key(&element, &Element<T>::count, element.count + 1);
    update_count(element);
}

template<class T>
void Algorithm<T>::print_state() {
    for(auto it = frequency_order.begin(); it != frequency_order.end(); ++it) {
        Element<T>* element = *it;
        cout << element->id << ", " << element->get_count() << endl;
    }
    assert(frequency_order.size() == this->sample_size());
    for(int i = 0; i < h; ++i) {
        for(int j = 0; j < m; ++j) {
            cout << counters[i][j] << ", ";
        }
        cout << endl;
    }
}

template<class T>
float Algorithm<T>::get_threshold() const {
    return 0;
}


}