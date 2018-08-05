#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_

#include <ostream>
#include <unordered_map>

// This interface is needed so it's possible to create a pointer
// to a GenericAlgorithm, since GenericAlgorithm is a template class
// and the type of ElementLocator is not known in compile time
template <class T>
class GenericAlgorithmInterface {
public:
    virtual void frequent_query(float f, std::ostream& stream) = 0;

    virtual void k_top_query(int k, std::ostream& stream) = 0;

    virtual void process_element(const T& element_id) = 0;

    virtual unsigned int sample_size() = 0;

    virtual void print_state() = 0; // For debugging purposes

    virtual ~GenericAlgorithmInterface() {};
};


template <class T, class ElementLocator>
class GenericAlgorithm : public GenericAlgorithmInterface<T> {

private:
    typedef std::unordered_map<T, ElementLocator> MonitoredElements;
    MonitoredElements monitored_elements;

protected:

    int N = 0;

    virtual bool insert_element(const T& element_id, ElementLocator& locator) = 0;

    virtual void update_element(ElementLocator& locator) = 0;

    void remove_element(const T& element_id);

    ElementLocator& get_locator(const T& element_id);

    void set_monitored_size(unsigned int m);

public:

    void process_element(const T& element_id) override;

    unsigned int sample_size() override;
};


#include "GenericAlgorithm.cpp"


#endif //_ALGORITHM_H_
