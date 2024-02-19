#ifndef CLASTERREMOVAL_HPP
#define CLASTERREMOVAL_HPP

#include "noderemoval.hpp"

namespace homecare {

class ClasterRemoval : public NodeRemoval {
private:
    /* data */
public:
    ClasterRemoval(/* args */);
    ~ClasterRemoval();
    void removeNodes(int) override;
};

}

#endif