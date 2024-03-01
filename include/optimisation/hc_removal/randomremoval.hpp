#ifndef RANDOMREMOVAL_HPP
#define RANDOMREMOVAL_HPP

#include "noderemoval.hpp"

namespace homecare {
    
class RandomRemoval : public NodeRemoval {
public:
    RandomRemoval();
    ~RandomRemoval();
    void removeNodes(int) override; 
};

} 

#endif