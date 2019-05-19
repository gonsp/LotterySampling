#include "algorithms/basic_lottery_sampling/Algorithm.h"
#include "utils/InputParser.h"
#include "utils/TicketUtils.h"
#include <stack>

namespace BasicLotterySampling {


using namespace std;

template<class T>
Algorithm<T>::Algorithm(const InputParser& parameters) {
    m = (unsigned int) stoul(parameters.get_parameter("-m"));
    multilevel = parameters.has_parameter("-multilevel");
    level_1 = TicketOrder<Element<T>>(m);
    if(!multilevel) {
        this->set_monitored_size(m);
    }
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

    element.ticket = ticket_generator.generate_token();
    element.freq = 1;

    if(this->sample_size() < m) {
        insert_level_1(element);
    } else {
        if(element.ticket > level_1.top()->ticket) {
            insert_level_1(element);
        } else if(!level_2.empty() && element.ticket > level_2.top()->ticket) {
            insert_level_2(element);
        } else {
            // New element didn't get a good enough ticket to get sampled, so it's discarded
            return false;
        }
    }

    frequency_order.insert_element(&element);
    return true;
}

template<class T>
void Algorithm<T>::update_element(Element<T>& element) {
    // Updating frequency
    frequency_order.update_key(&element, &Element<T>::freq, element.freq + 1);

    // Updating ticket
    Token token = ticket_generator.generate_token();
    if(token > element.ticket) { // The new ticket is better than the old one
        element.ticket = token; // Updating (the better) ticket
        if(element.level == 2 && level_1.top()->ticket < token) {
            // element is moving from level_2 to level_1, so we kick out the lowest ticket from level_1 to level_2
            level_2.remove_element(&element);
            insert_level_1(element);
        } else {
            (element.level == 1 ? level_1 : level_2).key_updated(&element);
        }
    }
}

template<class T>
double Algorithm<T>::get_threshold() const {
    return TicketUtils::normalize_ticket(level_1.top()->ticket);
}


}
