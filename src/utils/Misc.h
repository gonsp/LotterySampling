#ifndef _Misc_H_
#define _Misc_H_

#include <cstdint>

template<class ClassName>
using ComparatorFunction = bool (ClassName::*)(const ClassName& element) const;

template<class ClassName, class T>
using ClassField = T ClassName::*;

template<class ClassName, ComparatorFunction<ClassName> compare>
struct PointerComparator {
    bool operator()(const ClassName* a, const ClassName* b) const {
        return (a->*compare)(*b);
    }
};

#endif //_Misc_H_
