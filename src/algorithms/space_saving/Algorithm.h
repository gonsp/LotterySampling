#ifndef _SPACESAVING_H_
#define _SPACESAVING_H_

#include "algorithms/GenericAlgorithm.h"
#include "utils/InputParser.h"
#include "Types.h"
#include <string>


namespace SpaceSaving {


class Algorithm : public GenericAlgorithm<ElementLocator> {

private:

    StreamSummary stream_summary;
    unsigned int m;

    void increment_counter(ElementLocator& locator);

    bool insert_element(std::string& element_id, ElementLocator& locator) override;

    void update_element(ElementLocator& locator) override;

public:
    Algorithm(const InputParser& parameters);

    void frequent_query(float f, std::ostream& stream) override;

    void k_top_query(int k, std::ostream& stream) override;

    void print_state() override;
};


}

#endif //_SPACESAVING_H_
