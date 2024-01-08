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

namespace homecare {

class Route{
    private:
        std::vector<Node> m_nodes;
        Caregiver m_caregiver;
        static const int BASE_ROUTE_LEN;
        static const int DEPOT;

    public:
        static const int EMPTY_ROUTE = -1;
        static constexpr int ZERO_TIME = 0;

        Route(Caregiver);
        ~Route();
        //getter
        int getFreeTime() const;
        int getlastPatientDistanceIndex() const;
        std::vector<std::string> getAvilableServices() const;
        std::string getCaregiver() const;
        std::vector<Node> getNodes() const;
        //solver
        int addNode(Patient, std::vector<int>, int); 
        //toFileFormat
        std::string getRouteToString() const;
        Json::Value getJSONRoute() const;
        //checker
        bool isAvailable() const;
        //reader
        int readNodesFromJson(Json::Value, std::vector<Patient>,std::vector<int>) noexcept (false);
};

}

#endif