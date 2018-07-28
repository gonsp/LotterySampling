#ifndef _LOTTERYSAMPLING_H_
#define _LOTTERYSAMPLING_H_

#include "algorithms/GenericAlgorithm.h"
#include "InputParser.h"
#include "Types.h"
#include <random>

namespace LotterySampling {


class Algorithm : public GenericAlgorithm<ElementLocator> {

private:

    StreamSummary level_1;
    StreamSummary level_2;

    unsigned int m;
    bool aging;

    int seed;
    std::mt19937_64 rng;
    std::uniform_int_distribution<Ticket> dist;

    ElementLocator insert_element(std::string& element_id) override;

    void update_element(ElementLocator& locator) override;

    Ticket generate_ticket();

    void free_up_level_1();

    void free_up_level_2();

public:
    Algorithm(const InputParser& parameters);

    void frequent_query(float f, std::ostream& stream) override;

    void k_top_query(int k, std::ostream& stream) override;

    void print_state() override;
};


}

#endif //_LOTTERYSAMPLING_H_
