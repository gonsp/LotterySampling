#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_

#include <ostream>
#include <unordered_map>
#include "Stats.h"

// This interface is needed so it's possible to create a pointer
// to a GenericAlgorithm, since GenericAlgorithm is a template class
class GenericAlgorithmInterface {
public:
    virtual void frequent_query(float f, std::ostream& stream) = 0;

    virtual void k_top_query(int k, std::ostream& stream) = 0;

    virtual void process_element(std::string& element_id) = 0;

    virtual void print_state() = 0; // For debugging purposes
};


template <class ElementLocator>
class GenericAlgorithm : public GenericAlgorithmInterface {

protected:

    typedef std::unordered_map<std::string, ElementLocator> MonitoredElements;
    MonitoredElements monitored_elements;

    int N = 0;

    virtual ElementLocator insert_element(std::string& element_id) = 0;

    virtual void update_element(ElementLocator& locator) = 0;

    int size() {
        return (int) (monitored_elements.size());
    }

    void remove_element(std::string& element_id) {
        monitored_elements.erase(element_id);
    }

public:

    void process_element(std::string& element_id) override {
        ++N;
        typename MonitoredElements::iterator it = monitored_elements.find(element_id);
        if(it == monitored_elements.end()) { // element wasn't being sampled
            ElementLocator locator = insert_element(element_id);
            if(locator.is_valid()) {
                monitored_elements[element_id] = locator;
            }
        } else { // element was being sampled
            update_element(it->second);
        }
    }
};

#endif //_ALGORITHM_H_
