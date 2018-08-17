#include "Algorithm.h"
#include "utils/InputParser.h"
#include <iostream>
#include <stack>

namespace LotterySampling {


using namespace std;

template<class T>
Algorithm<T>::Algorithm(const InputParser& parameters) {
    m = (unsigned int) stoul(parameters.get_parameter("-m"));
    aging = parameters.has_parameter("-aging");
    multilevel = parameters.has_parameter("-multilevel");
    level_1 = TicketOrder<Element<T>>(m);
    if(!multilevel) {
        this->set_monitored_size(m);
    }
    int seed;
    if(parameters.has_parameter("-seed")) {
        seed = stoi(parameters.get_parameter("-seed"));
    } else {
        seed = random_device()();
    }
    random_state = mt19937_64(seed);
    if(aging) {
        // If aging is used, then the range of the tickets is decreased
        // in half to avoid overflows.
        MAX_TICKET = static_cast<unsigned long long int>(numeric_limits<int64_t>::max() - 1);
        dist = uniform_int_distribution<Ticket>(0, MAX_TICKET);
    } else {
        MAX_TICKET = numeric_limits<uint64_t>::max();
    }
}

template<class T>
void Algorithm<T>::frequent_query(float f, ostream& stream) {
    for(auto it = frequency_order.rbegin(); it != frequency_order.rend() && (*it)->freq >= f * this->N; ++it) {
        stream << (*it)->id << " " << (*it)->freq / (float) this->N << endl;
    }
}

template<class T>
void Algorithm<T>::k_top_query(int k, ostream& stream) {
    for(auto it = frequency_order.rbegin(); it != frequency_order.rend() && k-- > 0; ++it) {
        stream << (*it)->id << " " << (*it)->freq / (float) this->N << endl;
    }
}

template<class T>
void Algorithm<T>::insert_level_1(Element <T>& element) {
    element.level = 1;
    if(this->sample_size() < m) {
        level_1.push(&element);
    } else {
        Element<T>* replaced_element = level_1.pop_and_push(&element);
        if(multilevel) {
            // Smallest ticket's element is kicked out from level 1 to level 2
            level_2.push(replaced_element);
            replaced_element->level = 2;
        } else {
            // Smallest ticket's element is just removed since multilevel is not being used
            frequency_order.remove_element(replaced_element);
            this->remove_element(replaced_element->id);
        }
    }
}

template<class T>
void Algorithm<T>::insert_level_2(Element<T>& element) {
    // Smallest ticket's element is removed
    element.level = 2;
    Element<T>* removed_element = level_2.pop_and_push(&element);
    frequency_order.remove_element(removed_element);
    this->remove_element(removed_element->id);
}

template<class T>
bool Algorithm<T>::insert_element(Element<T>& element) {

    element.ticket = generate_ticket();

    if(this->sample_size() < m) {
        element.freq = 1;
        element.over_estimation = 0;
        insert_level_1(element);
    } else {
        if(element.ticket > level_1.top()->ticket) {
            element.freq = estimate_frequency(level_1.top()->ticket);
            insert_level_1(element);
        } else if(!level_2.empty() && element.ticket > level_2.top()->ticket) {
            element.freq = estimate_frequency(level_2.top()->ticket);
            insert_level_2(element);
        } else {
            // New element didn't get a good enough ticket to get sampled, so it's discarded
            return false;
        }
        element.over_estimation = element.freq - 1;
    }

    frequency_order.insert_element(&element);
    return true;
}

template<class T>
void Algorithm<T>::update_element(Element<T>& element) {
    // Updating frequency
    frequency_order.update_key(&element, &Element<T>::freq, element.freq + 1);

    // Updating ticket
    Ticket ticket = generate_ticket();
    if(ticket > element.ticket) { // The new ticket is better than the old one
        element.ticket = ticket; // Updating (the better) ticket
        if(element.level == 2 && level_1.top()->ticket < ticket) {
            // element is moving from level_2 to level_1, so we kick out the lowest ticket from level_1 to level_2
            level_2.remove_element(&element);
            insert_level_1(element);
        } else {
            (element.level == 1 ? level_1 : level_2).key_updated(&element);
        }
    }
}

template<class T>
Ticket Algorithm<T>::generate_ticket() {
    Ticket ticket = dist(random_state);
    if(aging) {
        ticket += this->N;
    }
    return ticket;
}

template<class T>
inline unsigned int Algorithm<T>::estimate_frequency(Ticket min_ticket) const {
    // TODO Protect from infinity
    // TODO take into account aging
    return static_cast<unsigned int>(1 / (1 - min_ticket / (double) MAX_TICKET));
}

template<class T>
void Algorithm<T>::print_level(TicketOrder<Element<T>>& level) {
    TicketOrder<Element<T>> aux = level;
    stack<Element<T>*> s;
    while(!aux.empty()) {
        Element<T>* element = aux.pop();
        s.push(element);
    }
    while(!s.empty()) {
        Element<T>* element = s.top();
        s.pop();
        cout << element->id << ", " << element->ticket << ", " << element->freq << ", " << element->over_estimation << endl;
    }
}

template<class T>
void Algorithm<T>::print_state() {
    cout << "-----------------------" << endl;
    cout << "%%%%%% level_1 %%%%%%" << endl;
    print_level(level_1);
    cout << "-----------------------" << endl;
    cout << "%%%%%% level_2 %%%%%%" << endl;
    print_level(level_2);
    cout << "-----------------------" << endl;
    cout << "%%%%%% frequency_order %%%%%%" << endl;
    for(auto it = frequency_order.rbegin(); it != frequency_order.rend(); ++it) {
        cout << (*it)->id << ", " << (*it)->ticket << ", " << (*it)->freq << ", " << (*it)->over_estimation << endl;
    }
    assert(level_1.size() + level_2.size() == this->sample_size());
    assert(frequency_order.size() == this->sample_size());
}


}
