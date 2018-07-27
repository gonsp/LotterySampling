#ifndef _Types_H_
#define _Types_H_

#include <list>
#include <string>

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


}

#endif //_Types_H_
