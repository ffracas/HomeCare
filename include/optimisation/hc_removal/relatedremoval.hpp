#ifndef RELATEDREMOVAL_HPP
#define RELATEDREMOVAL_HPP

#include "noderemoval.hpp"

namespace homecare {

class RelatedRemoval : public NodeRemoval {
private:
    const double m_related;                                             //parametro related removal
    const double m_distanceWeight;                                      //parametro related removal
    const double m_windowWeight;                                        //parametro related removal
    const double m_serviceWeight;                                       //parametro related removal

    double calculateSimilarity(int, int, int, bool, int, int, int) const;
    void findMinMaxRelatetion(int&, int&, int&, int&, int&, int, int) const;
    std::pair<int, int> getRandomNode(std::vector<std::pair<int, int>>) noexcept (false);
    
public:
    RelatedRemoval(ALNSOptimisation&, double, double, double, double);
    ~RelatedRemoval();
    void removeNodes(int) override;
};

}

#endif