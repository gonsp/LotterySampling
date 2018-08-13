#ifndef _Element_SpaceSaving_H_
#define _Element_SpaceSaving_H_

#include <list>
#include <string>

namespace SpaceSaving {


template<class T>
struct Element {

public:
    struct Bucket {
    private:
        typedef std::list<Element<T>*> ElementList;

    public:
        typedef typename ElementList::iterator iterator;

        int count;
        ElementList elements;

        Bucket(int count) {
            this->count = count;
        }
    };

    typedef std::list<Bucket> StreamSummary;

    T id;
    int over_estimation;

    typename StreamSummary::iterator bucket_iterator;
    typename Bucket::iterator element_iterator;

    Element(const T& id) {
        this->id = id;
    }
};


}

#endif //_Element_SpaceSaving_H_
