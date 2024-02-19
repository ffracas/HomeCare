#ifndef CLASTERREMOVAL_HPP
#define CLASTERREMOVAL_HPP

#include "noderemoval.hpp"

namespace homecare {

class ClasterRemoval : public NodeRemoval {
private:
    
public:
    ClasterRemoval(ALNSOptimisation&);
    ~ClasterRemoval();
    void removeNodes(int) override;
};

}

#endif