#include "algorithms/frequent/Algorithm.h"
#include <iostream>

namespace Frequent {


using namespace std;

template<class T>
Algorithm<T>::Algorithm(const InputParser& parameters) {
    m = (unsigned int) stoul(parameters.get_parameter("-m"));
    this->set_monitored_size(m);
    offset = 0;
}

template<class T>
FrequencyOrderIterator<Element<T>> Algorithm<T>::frequency_order_begin() {
    return frequency_order.begin();
}

template<class T>
FrequencyOrderIterator<Element<T>> Algorithm<T>::frequency_order_end() {
    return frequency_order.end();
}

template<class T>
bool Algorithm<T>::insert_element(Element<T>& element) {
    if(this->sample_size() < m) {
        frequency_order.insert_element(&element);
    } else { // Max number of monitored elements is reached. This new one will replace the one with less hits
        bool is_inserted = (*prev(frequency_order.end()))->get_count() == offset;
        if(!is_inserted) {
            ++offset;
            return false;
        }
        Element<T>* removed_element = frequency_order.pop_and_push(&element);
        this->remove_element(removed_element->id);

        frequency_order.increment_key(&element);
    }
    return true;
}

template<class T>
void Algorithm<T>::update_element(Element<T>& element) {
    frequency_order.increment_key(&element);
}

template<class T>
void Algorithm<T>::print_state() {
    for(auto it = frequency_order.begin(); it != frequency_order.end(); ++it) {
        Element<T>* element = *it;
        cout << element->id << ", " << element->get_count() << endl;
    }
    assert(frequency_order.size() == this->sample_size());
}

template<class T>
float Algorithm<T>::get_threshold() const {
    return 0;
}


}