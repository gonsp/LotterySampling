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
        int over_estimation;

        Element(int over_estimation) {
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

    int m;

public:
    SpaceSaving(const InputParser& parameters);

    void frequent_query(std::ostream& stream) override;

    void k_top_query(std::ostream& stream) override;

    void process_element(std::string& element_id) override;
};


#endif //_SPACESAVING_H_
