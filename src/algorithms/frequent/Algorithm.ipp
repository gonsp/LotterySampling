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
FrequencyOrder<Element<T>>& Algorithm<T>::get_frequency_order() {
    return frequency_order;
}

template<class T>
bool Algorithm<T>::insert_element(Element<T>& element) {
    if(this->sample_size() < m) {
        frequency_order.insert_element(&element);
    } else {
        bool is_inserted = (*prev(frequency_order.end()))->get_count() == offset;
        if(!is_inserted) {
            ++offset;
            return false;
        }
        Element<T>* removed_element = frequency_order.replace_last(&element);
        this->remove_element(removed_element->id);

        frequency_order.increase_key(&element);
    }
    return true;
}

template<class T>
void Algorithm<T>::update_element(Element<T>& element) {
    frequency_order.increase_key(&element);
}

template<class T>
void Algorithm<T>::print_state() {
    for(auto it = frequency_order.begin(); it != frequency_order.end(); ++it) {
        Element<T>* element = *it;
        cout << element->id << ", " << element->get_count() << endl;
    }
    assert(frequency_order.size() == this->sample_size());
}

}