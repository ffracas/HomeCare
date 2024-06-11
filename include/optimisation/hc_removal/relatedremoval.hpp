#ifndef RELATEDREMOVAL_HPP
#define RELATEDREMOVAL_HPP

#include "noderemoval.hpp"

namespace homecare {

class RelatedRemoval : public NodeRemoval {
private:
    static const double m_related_exp_coef;                                             //parametro related removal
    static const double m_distanceWeight;                                      //parametro related removal
    static const double m_windowWeight;                                        //parametro related removal
    static const double m_serviceWeight;                                       //parametro related removal

    Node getRandomNode(const std::vector<Node>&) noexcept (false);
    static int calculateSharedCaregivers(std::string, std::string, int&, int&);
    
public:
    RelatedRemoval();
    ~RelatedRemoval();
    void removeNodes(int) override;
    static double calculateSimilarity(int, int, int, std::string, std::string);
};

}

#endif