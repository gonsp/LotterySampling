#ifndef _List_H_
#define _List_H_

#include "utils/Misc.h"
#include <list>

namespace List {


template<class Element>
using Iterator = typename std::list<Element*>::iterator;

template<class Element>
using Locator = Iterator<Element>;


template<class Element, ClassField<Element, Locator<Element>> locator_field>
class List {

private:

    std::list<Element*> l;

public:

    void insert_element(Element* element);

    Element* get_older() const;

    Element* pop_and_push(Element* element);

    void move_to_front(Element* element);

    Iterator<Element> begin();

    Iterator<Element> end();

    unsigned long size() const;

    bool empty() const;
};


}

#include "data_structures/List.ipp"

#endif //_List_H_
