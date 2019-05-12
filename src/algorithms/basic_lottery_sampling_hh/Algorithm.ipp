#include "algorithms/basic_lottery_sampling_hh/Algorithm.h"
#include "utils/InputParser.h"
#include "utils/TicketUtils.h"
#include "math.h"
#include <stack>

namespace BasicLotterySamplingHH {


using namespace std;

template<class T>
Algorithm<T>::Algorithm(const InputParser& parameters) {
    phi = stof(parameters.get_parameter("-phi"));
    delta = stof(parameters.get_parameter("-delta"));
    error = stof(parameters.get_parameter("-error"));
    r = phi/error * log(1/(phi * delta));
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

    element.ticket = ticket_generator.generate_token();
    element.count = 1;

    Ticket threshold = (Ticket) get_threshold() * TicketUtils::MAX_TICKET;
    if(element.ticket < threshold) {
        return false;
    }

    frequency_order.insert_element(&element);

    Element<T>* element_min_ticket = ticket_order.top();
    if(element_min_ticket->ticket < threshold) {
        frequency_order.remove_element(element_min_ticket);
        ticket_order.pop();
        this->remove_element(element_min_ticket->id);
    }
    return true;
}

template<class T>
void Algorithm<T>::update_element(Element<T>& element) {
    // Updating frequency
    frequency_order.update_key(&element, &Element<T>::count, element.count + 1);

    // Updating ticket
    Token token = ticket_generator.generate_token();
    if(token > element.ticket) {
        element.ticket = token;
    }
}

template<class T>
double Algorithm<T>::get_threshold() const {
    return 1 - (1 / ((phi / r) * this->N));
}

template<class T>
float Algorithm<T>::get_frequency_threshold(float f) const {
    assert(f == phi);
    return phi - error;
}


}
