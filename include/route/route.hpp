#ifndef ROUTE_HPP
#define ROUTE_HPP

#include <iostream>
#include <vector>
#include "../arc/arc.hpp"

using namespace std;

namespace homecare{

class Route{
    private:
        std::vector<Arc> m_arcs_in_route;
        int m_len;

    public:
        Route();
        ~Route();
};

}

#endif