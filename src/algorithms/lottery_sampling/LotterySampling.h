#ifndef _LOTTERYSAMPLING_H_
#define _LOTTERYSAMPLING_H_

#include "algorithms/Algorithm.h"
#include "InputParser.h"

class LotterySampling : public Algorithm {

public:
    LotterySampling(const InputParser& parameters);

    void frequent_query(std::ostream& stream) override;

    void k_top_query(std::ostream& stream) override;

    void process_element(std::string element) override;
};


#endif //_LOTTERYSAMPLING_H_
