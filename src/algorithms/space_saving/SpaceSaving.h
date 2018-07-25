#ifndef _SPACESAVING_H_
#define _SPACESAVING_H_

#include "algorithms/Algorithm.h"
#include "InputParser.h"
#include <string>
#include <unordered_map>
#include <list>

class SpaceSaving : public Algorithm{

private:

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

    typedef std::unordered_map<std::string, ElementLocator> MonitoredElements;

    StreamSummary stream_summary;
    MonitoredElements monitored_elements;
    unsigned int m;

    void increment_counter(ElementLocator& locator);

public:
    SpaceSaving(const InputParser& parameters);

    void frequent_query(std::ostream& stream) override;

    void k_top_query(std::ostream& stream) override;

    void process_element(std::string& element_id) override;

    void print_state() override;
};


#endif //_SPACESAVING_H_
