#include "algorithms/lottery_sampling_parallel/Algorithm.h"
#include "utils/InputParser.h"
#include "utils/TicketUtils.h"
#include <iostream>

namespace LotterySamplingParallel {


using namespace std;

template<class T>
Algorithm<T>::Algorithm(const InputParser& parameters) {
    m = (unsigned int) stoul(parameters.get_parameter("-m"));
    unsigned int h = (unsigned int) stoul(parameters.get_parameter("-h"));
    instances = vector<TicketOrder<ElementInstance<T>>>(h);
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
    element.observed_count = 0;

    frequency_order.insert_element(&element);

    update_element(element);

    bool some_instance_inserted = !element.instances.empty();
    if(!some_instance_inserted) {
        frequency_order.remove_element(&element);
    }
    return some_instance_inserted;
}

template<class T>
void Algorithm<T>::update_element(Element<T>& element) {
    frequency_order.update_key(&element, &Element<T>::observed_count, element.observed_count + 1);

    for(int i = 0; i < instances.size(); ++i) {

        Token token = ticket_generator.generate_token();

        if(element.instances.count(i) == 0) { // element wasn't sampled in instance i

            bool is_inserted = instances[i].size() < m or instances[i].top()->ticket < token;
            if(is_inserted) {
                ElementInstance<T>& element_instance = element.instances.emplace(i, ElementInstance<T>(&element, token)).first->second;
                if(instances[i].size() < m) {
                    instances[i].push(&element_instance);
                } else {
                    ElementInstance<T>* replaced_element_instance = instances[i].pop_and_push(&element_instance);
                    Element<T>& replaced_element = *replaced_element_instance->element;
                    // We keep the mean ticket of the replaced_element as it is intentionally
                    replaced_element.instances.erase(i);
                    if(replaced_element.instances.empty()) {
                        frequency_order.remove_element(&replaced_element);
                        this->remove_element(replaced_element.id);
                    }
                }
            }

        } else { // element was being sampled in instance i
            ElementInstance<T>& element_instance = element.instances.find(i)->second;
            if(element_instance.ticket < token) {
                element_instance.ticket = token;
                instances[i].key_updated(&element_instance);
            }
        }
    }
}

template<class T>
void Algorithm<T>::print_state() {
    for(auto it = frequency_order.begin(); it != frequency_order.end(); ++it) {
        Element<T>& element = *(*it);
        cout << element.id << ", " << element.get_count() << endl;

        for(int i = 0; i < instances.size(); ++i) {
            Ticket ticket;
            if(element.instances.count(i) == 0) {
                ticket = 0;
            } else {
                ticket = element.instances.find(i)->second.ticket;
            }
            cout << TicketUtils::normalize_ticket(ticket) << " ";
        }
    }
}


}
