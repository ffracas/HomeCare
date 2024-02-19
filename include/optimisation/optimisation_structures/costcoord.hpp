#ifndef COSTCOORD_HPP
#define COSTCOORD_HPP

namespace homecare {

class CostCoord {
private:
    double m_cost;
    int m_n_route;
    int m_n_node;

public:
    CostCoord(double t_cost, int t_n_route, int t_n_node);
    double  getCost() const;
    int     getRouteNumber() const;
    int     getNodePosition() const;
};

}

#endif