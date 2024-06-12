#ifndef WORSTREMOVAL_HPP
#define WORSTREMOVAL_HPP

#include "noderemoval.hpp"

namespace homecare {
    
class WorstRemoval : public NodeRemoval {
private:
    const double p_worst = 5.0;                                               //parametro worst removal

public:
    WorstRemoval();
    ~WorstRemoval();
    void removeNodes(int) override;
};


} 


#endif