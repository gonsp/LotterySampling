#ifndef _SPACESAVING_H_
#define _SPACESAVING_H_

#include "algorithms/GenericAlgorithm.h"
#include "InputParser.h"
#include <string>
#include <list>

namespace SpaceSaving {


struct Element {
    std::string id;
    int over_estimation;

    Element(std::string id, int over_estimation) {
        this->id = id;
        this->over_estimation = over_estimation;
    }
};

struct Bucket {
    int count;
    std::list<Element> elements;

    Bucket(int count) {
        this->count = count;
    }
};

typedef std::list<Bucket> StreamSummary;

struct ElementLocator {
    StreamSummary::iterator bucket_iterator;
    std::list<Element>::iterator element_iterator;
};


class Algorithm : public GenericAlgorithm<ElementLocator> {

private:

    StreamSummary stream_summary;
    unsigned int m;

    void increment_counter(ElementLocator& locator);

    ElementLocator insert_element(std::string& element_id) override;

    void update_element(ElementLocator& locator) override;

public:
    Algorithm(const InputParser& parameters);

    void frequent_query(float f, std::ostream& stream) override;

    void k_top_query(int k, std::ostream& stream) override;

    void print_state() override;
};


}

#endif //_SPACESAVING_H_
