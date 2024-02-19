#ifndef HISTORICALREMOVAL_HPP
#define HISTORICALREMOVAL_HPP

#include "noderemoval.hpp"

namespace homecare {
    
class HistoricalRemoval : public NodeRemoval {
private:
    /* data */
public:
    HistoricalRemoval(/* args */);
    ~HistoricalRemoval();
    void removeNodes(int) override;
};

} 

#endif