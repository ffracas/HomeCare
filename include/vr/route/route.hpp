#ifndef ROUTE_HPP
#define ROUTE_HPP

#include <iostream>
#include <vector>
#include <string>
#include "../arc/arc.hpp"
#include "../params/params.hpp"
#include "../utils/utils.hpp"

using namespace std;

namespace vr{

class Route{
    private:
        std::vector<Arc> m_arcs_in_route;
        Node m_depot;
        int m_maxQuantity;
        int m_quantity; 
        double m_currentTime;
        int m_lastClientID;
        static const int BASE_ROUTE_LEN = 2;

    public:
        static const int EMPTY_ROUTE = -1;
        static constexpr double ZERO_TIME = 0.0;

        Route(Node, double);
        ~Route();
        //getter
        double getFreeTime();
        int getLastClient();
        int addNode(Node, vector<vector<double>>, int = 0);
        string getRouteToString();
        //cheker
        bool isSuitableFor(Node);
};

}

#endif