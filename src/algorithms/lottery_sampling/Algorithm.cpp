#include "Algorithm.h"

using namespace LotterySampling;

Algorithm::Algorithm(const InputParser& parameters) {
    m = (unsigned  int) stoul(parameters.get_parameter("-m"));
    aging = parameters.has_parameter("-aging");
}

void Algorithm::frequent_query(float f, std::ostream& stream) {

}

void Algorithm::k_top_query(int k, std::ostream& stream) {

}

ElementLocator Algorithm::insert_element(std::string& element_id) {
    ElementLocator locator;
    Ticket ticket;
    if(size() < m) {
        locator.element_iterator = level_1.emplace(element_id, ticket, 1);
        locator.is_level_1 = true;
    } else {
        if(ticket > level_1.begin()->ticket) {
            locator.element_iterator = level_1.emplace(element_id, ticket, 1);
            locator.is_level_1 = true;
            // continue
        } else if(ticket > level_2.begin()->ticket) {
            locator.is_level_1 = false;
            // continue
        } else {
            // continue
        }
    }
    return locator;
}

void Algorithm::update_element(ElementLocator& locator) {

}

void Algorithm::print_state() {
}
