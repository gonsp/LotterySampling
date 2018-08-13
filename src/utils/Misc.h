#ifndef _Misc_H_
#define _Misc_H_

struct PointerComparator {
    template<class T>
    inline bool operator()(const T* a, const T* b) const {
        return *a < *b;
    }
};

#endif //_Misc_H_
