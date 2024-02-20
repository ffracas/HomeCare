#ifndef RELATEDREMOVAL_HPP
#define RELATEDREMOVAL_HPP

#include "noderemoval.hpp"

namespace homecare {

class RelatedRemoval : public NodeRemoval {
private:
    static const double m_related;                                             //parametro related removal
    static const double m_distanceWeight;                                      //parametro related removal
    static const double m_windowWeight;                                        //parametro related removal
    static const double m_serviceWeight;                                       //parametro related removal

    std::pair<int, int> getRandomNode(std::vector<std::pair<int, int>>) noexcept (false);
    static int calculateSharedCaregivers(std::string, std::string, int&, int&);
    
public:
    RelatedRemoval(ALNSOptimisation&);
    ~RelatedRemoval();
    void removeNodes(int) override;
    static double calculateSimilarity(int, int, int, std::string, std::string);
};

}

#endif