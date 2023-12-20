#ifndef ROUTE_HPP
#define ROUTE_HPP

#include <iostream>
#include <vector>
#include <string>

#include "arc.hpp"
#include "../patient/patient.hpp"
#include "../depot/depot.hpp"
#include "../caregiver/caregiver.hpp"

using namespace std;

namespace homecare{

class Route{
    private:
        std::vector<Arc> m_arcs_in_route;
        Caregiver m_caregiver;
        int m_currentTime;
        int m_lastPatientDistanceIndex;
        static const int BASE_ROUTE_LEN = 2;

    public:
        static const int EMPTY_ROUTE = -1;
        static constexpr int ZERO_TIME = 0;

        Route(Caregiver);
        ~Route();
        //getter
        int getFreeTime();
        int getLastClient();
        int addNode(Patient, vector<vector<int>>, int = 0); 
        string getRouteToString();
};

}

#endif