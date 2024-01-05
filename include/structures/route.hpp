#ifndef ROUTE_HPP
#define ROUTE_HPP

#include <iostream>
#include <vector>
#include <string>
#include <json/json.h>

#include "arc.hpp"
#include "node.hpp"
#include "../patient/patient.hpp"
#include "../caregiver/caregiver.hpp"

using namespace std;

namespace homecare {

class Route{
    private:
        vector<Arc> m_arcs;
        Caregiver m_caregiver;
        int m_currentTime;
        int m_lastPatientDistanceIndex;
        static const int BASE_ROUTE_LEN = 2;
        const Node m_depot;
        bool m_depot2depot;

    public:
        static const int EMPTY_ROUTE = -1;
        static constexpr int ZERO_TIME = 0;

        Route(Caregiver);
        ~Route();
        //getter
        int getFreeTime() const;
        int getlastPatientDistanceIndex() const;
        vector<string> getAvilableServices() const;
        string getCaregiver() const;
        vector<Arc> getArcs() const;
        //solver
        int addNode(Node, vector<int>, vector<int>, int); 
        //toFileFormat
        string getRouteToString() const;
        Json::Value getJSONRoute() const;
        //checker
        bool isAvailable() const;
        //reader
        int readNodesFromJson(Json::Value, vector<Patient>, vector<vector<int>>) noexcept (false);
};

}

#endif