#ifndef WORSTREMOVAL_HPP
#define WORSTREMOVAL_HPP

#include "noderemoval.hpp"

namespace homecare {
    
class WorstRemoval : public NodeRemoval {
private:
    //const double p_worst;                                               //parametro worst removal
public:
    WorstRemoval(double);
    ~WorstRemoval();
    void removeNodes(int) override;
};


} 


#endif