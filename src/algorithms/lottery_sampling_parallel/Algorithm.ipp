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
    element.mean_ticket = 0;

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
    element.observed_count++;

    Ticket new_mean_ticket = 0;
    Ticket estimated_ticket = TicketUtils::estimate_ticket(element.observed_count);

    for(int i = 0; i < instances.size(); ++i) {

        Ticket ticket = ticket_generator.generate_ticket();

        if(element.instances.count(i) == 0) { // element wasn't sampled in instance i

            bool is_inserted = instances[i].size() < m or instances[i].top()->ticket < ticket;
            if(is_inserted) {
                ElementInstance<T>& element_instance = element.instances.emplace(i, ElementInstance<T>(&element, ticket)).first->second;
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
            } else {
                // TODO consider max(estimated_ticket, ticket)
                ticket = estimated_ticket;
            }

        } else { // element was being sampled in instance i
            ElementInstance<T>& element_instance = element.instances.find(i)->second;
            if(element_instance.ticket < ticket) {
                element_instance.ticket = ticket;
                instances[i].key_updated(&element_instance);
            } else {
                ticket = element_instance.ticket;
            }
        }

        TicketUtils::incremental_averaging(new_mean_ticket, ticket, i + 1);
    }

    frequency_order.update_key(&element, &Element<T>::mean_ticket, new_mean_ticket);
}

template<class T>
float Algorithm<T>::get_threshold() const {
    return 0;
}

template<class T>
void Algorithm<T>::print_state() {
    for(auto it = frequency_order.begin(); it != frequency_order.end(); ++it) {
        Element<T>& element = *(*it);
        cout << element.id << ", " << element.get_count() << ", " << TicketUtils::normalize_ticket(element.mean_ticket) << ", " << element.observed_count << endl;

        Ticket mean_ticket = 0;
        Ticket estimated_ticket = TicketUtils::estimate_ticket(element.observed_count);
        for(int i = 0; i < instances.size(); ++i) {
            Ticket ticket;
            if(element.instances.count(i) == 0) {
                ticket = estimated_ticket;
            } else {
                ticket = element.instances.find(i)->second.ticket;
            }
            cout << TicketUtils::normalize_ticket(ticket) << " ";
            mean_ticket += ticket / instances.size();
        }
        cout << endl << TicketUtils::normalize_ticket(mean_ticket) << endl;
    }
}


}
