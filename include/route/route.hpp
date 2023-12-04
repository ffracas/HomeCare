#ifndef ROUTE_HPP
#define ROUTE_HPP

#include <iostream>
#include <vector>
#include "../arc/arc.hpp"
#include "../params/params.hpp"
#include "../utils/utils.hpp"

using namespace std;

namespace homecare{

class Route{
    private:
        std::vector<Arc> m_arcs_in_route;
        Node m_depot;
        int m_quantity = 0;
        int m_distance = 0;
        double m_currentTime = 0;
        static const int BASE_ROUTE_LEN = 2;

    public:
        static const int EMPTY_ROUTE = -1;

        Route(Node);
        ~Route();
        int addNextNode(Node, double**);
        void setSeed(Node, double);
        bool contains(int);
        int searchForNextNode(vector<Node>, double**, Params);
        bool addNodeBetween(int, Node, double**);
        double getCurrentTime();
};

}

#endif