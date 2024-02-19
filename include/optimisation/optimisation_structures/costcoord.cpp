#include "costcoord.hpp"

using namespace homecare;

CostCoord::CostCoord(double t_cost, int t_n_route, int t_n_node)
        : m_cost(t_cost), m_n_route(t_n_route), m_n_node(t_n_node) {}

double CostCoord::getCost() const {
    return m_cost;
}

int CostCoord::getRouteNumber() const {
    return m_n_route;
}

int CostCoord::getNodePosition() const {
    return m_n_node;
}