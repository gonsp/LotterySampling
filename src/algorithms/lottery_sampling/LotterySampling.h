#ifndef _LOTTERYSAMPLING_H_
#define _LOTTERYSAMPLING_H_

#include "algorithms/GenericAlgorithm.h"
#include "InputParser.h"
#include "Types.h"

namespace LotterySampling {


class Algorithm : public GenericAlgorithm<int> {

private:

    StreamSummary s1;
    StreamSummary s2;

    int insert_element(std::string& element_id) override;

    void update_element(int& locator) override;

public:
    Algorithm(const InputParser& parameters);

    void frequent_query(float f, std::ostream& stream) override;

    void k_top_query(int k, std::ostream& stream) override;

    void print_state() override;
};


}

#endif //_LOTTERYSAMPLING_H_
