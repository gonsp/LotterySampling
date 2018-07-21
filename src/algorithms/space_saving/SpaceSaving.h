#ifndef _SPACESAVING_H_
#define _SPACESAVING_H_

#include "algorithms/Algorithm.h"
#include "InputParser.h"

class SpaceSaving : public Algorithm{

public:
    SpaceSaving(const InputParser& parameters);

    void frequent_query(std::ostream& stream) override;

    void k_top_query(std::ostream& stream) override;

    void process_element(std::string element) override;
};


#endif //_SPACESAVING_H_
