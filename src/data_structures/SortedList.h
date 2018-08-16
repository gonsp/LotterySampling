#ifndef _SortedList_H_
#define _SortedList_H_

#include <list>

template<class Element>
class SortedList {

private:

    typedef std::list<Element*> ElementList;

    struct Bucket {
        unsigned int freq;
        ElementList elements;

        Bucket(unsigned int freq) {
            this->freq = freq;
        }
    };

    typedef std::list<Bucket> BucketList;
    BucketList bucket_list;

public:

    struct Iterator {

    private:
        SortedList<Element>* sorted_list;

    public:
        typename BucketList::iterator bucket_iterator;
        typename ElementList::iterator element_iterator;

        Iterator() {
            this->sorted_list = nullptr;
        }

        Iterator(SortedList<Element>* sorted_list) {
            this->sorted_list = sorted_list;
        }

        Element*& operator*() const;

        Iterator operator++();

        Iterator operator--();

        bool operator==(const Iterator& iterator);

        bool operator!=(const Iterator& iterator);
    };

    void insert_element(Element* element);

    Element* pop_and_push(Element* element);

    void increment_frequency(Element* element);

    unsigned int get_frequency(Element* element) const;

    Iterator begin();

    Iterator end();

};


#include "SortedList.ipp"

#endif //_SortedList_H_
