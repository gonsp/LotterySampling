#include "FrequencyOrder.h"


using namespace std;

template<class Element, class FrequencyComparator>
void FrequencyOrder<Element, FrequencyComparator>::insert_element(Element* element) {
    element->frequency_order_iterator = this->insert(element);
}

template<class Element, class FrequencyComparator>
void FrequencyOrder<Element, FrequencyComparator>::remove_element(Element* element) {
    this->erase(element->frequency_order_iterator);
    element->frequency_order_iterator = this->end();
}

template<class Element, class FrequencyComparator>
void FrequencyOrder<Element, FrequencyComparator>::increment_frequency(Element* element) {
    iterator hint = next(element->frequency_order_iterator);
    this->erase(element->frequency_order_iterator); // It's needed to remove and reinsert an element since there isn't an "update" method in multiset
    element->freq++;
    element->frequency_order_iterator = this->emplace_hint(hint, element);

}

template<class Element, class FrequencyComparator>
void FrequencyOrder<Element, FrequencyComparator>::modify_frequency(Element* element, unsigned int freq) {
    this->erase(element->frequency_order_iterator);
    element->freq = freq;
    element->frequency_order_iterator = this->emplace(element);
}

