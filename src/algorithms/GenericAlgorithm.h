#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_

#include <ostream>
#include <unordered_map>

// This interface is needed so it's possible to create a pointer
// to a GenericAlgorithm, since GenericAlgorithm is a template class
// and the type of Element is not known in compile time
template<class T>
class GenericAlgorithmInterface {
public:
    virtual void frequent_query(float f, std::ostream& stream) = 0;

    virtual void k_top_query(int k, std::ostream& stream) = 0;

    virtual void process_element(const T& element_id) = 0;

    virtual unsigned int sample_size() const = 0;

    virtual void print_state() = 0; // For debugging purposes

    virtual ~GenericAlgorithmInterface() {};
};


template<template<typename> class Element, class T>
class GenericAlgorithm : public GenericAlgorithmInterface<T> {

private:
    typedef std::unordered_map<T, Element<T>> MonitoredElements;
    MonitoredElements monitored_elements;
    bool has_extra_element = false;

protected:

    int N = 0;

    virtual bool insert_element(Element<T>& element) = 0;

    virtual void update_element(Element<T>& element) = 0;

    void remove_element(const T& element_id);

    void set_monitored_size(unsigned int m);

public:

    void process_element(const T& element_id) override;

    unsigned int sample_size() const override;
};


#include "GenericAlgorithm.ipp"


#endif //_ALGORITHM_H_
