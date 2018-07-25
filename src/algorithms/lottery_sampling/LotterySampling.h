#ifndef _LOTTERYSAMPLING_H_
#define _LOTTERYSAMPLING_H_

#include "algorithms/Algorithm.h"
#include "InputParser.h"

class LotterySampling : public Algorithm {

private:
    void process_element(std::string& element_id) override;

public:
    LotterySampling(const InputParser& parameters);

    void frequent_query(float f, std::ostream& stream) override;

    void k_top_query(int k, std::ostream& stream) override;

    void print_state() override;
};


#endif //_LOTTERYSAMPLING_H_
