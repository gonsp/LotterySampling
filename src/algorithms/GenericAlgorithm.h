#ifndef _GenericAlgorithm_H_
#define _GenericAlgorithm_H_

#include <ostream>
#include <unordered_map>
#include <string>
#include <list>

template<class T>
using QueryResults = std::list<std::pair<T, unsigned int>>;

// This interface is needed so it's possible to create a pointer
// to a GenericAlgorithm, since GenericAlgorithm is a template class
// and the type of Element is not known in compile time
template<class T>
class GenericAlgorithmInterface {

public:

    virtual QueryResults<T> frequent_query(double f, std::ostream& stream) = 0;

    virtual QueryResults<T> top_k_query(int k, std::ostream& stream) = 0;

    virtual void process_element(const T& element_id) = 0;

    virtual unsigned int sample_size() const = 0;

    virtual std::unordered_map<std::string, double> get_custom_stats() {
        return std::unordered_map<std::string, double>();
    }

    virtual ~GenericAlgorithmInterface() {}

};


template<template<typename> class Element, class T, class FrequencyOrder>
class GenericAlgorithm : public GenericAlgorithmInterface<T> {

private:

    typedef std::unordered_map<T, Element<T>> MonitoredElements;
    MonitoredElements monitored_elements;
    unsigned int m = 0;

protected:

    virtual bool insert_element(Element<T>& element) = 0;

    virtual void update_element(Element<T>& element) = 0;

    virtual FrequencyOrder& get_frequency_order() = 0;

    virtual double get_frequency_threshold(double f) const { return f; }

    void remove_element(const T& element_id);

    void set_monitored_size(unsigned int m);

public:

    int N = 0;

    void process_element(const T& element_id) override;

    unsigned int sample_size() const override;

    QueryResults<T> frequent_query(double f, std::ostream& stream) override;

    QueryResults<T> top_k_query(int k, std::ostream& stream) override;

};


#include "GenericAlgorithm.ipp"


#endif //_GenericAlgorithm_H_
