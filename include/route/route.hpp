#ifndef ROUTE_HPP
#define ROUTE_HPP

#include <iostream>
#include <vector>
#include <string>
#include "../arc/arc.hpp"
#include "../params/params.hpp"
#include "../utils/utils.hpp"

using namespace std;

namespace homecare{

class Route{
    private:
        std::vector<Arc> m_arcs_in_route;
        Node m_depot;
        int m_quantity; 
        //int m_distance;
        double m_currentTime;
        static const int BASE_ROUTE_LEN = 2;

    public:
        static const int EMPTY_ROUTE = -1;

        Route(Node);
        ~Route();
        int addNextNode(Node, double**);
        bool contains(int);
        int searchForNextNode(vector<Node>, double**, Params);
        int addNodeBetween(int, Node, double**);
        double getCurrentTime();
        string getRouteToString();
        double valutateAlternativeRoute(int, Node, double**);
};

}

#endif