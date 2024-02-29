#ifndef ROUTE_HPP
#define ROUTE_HPP

#include <iostream>
#include <vector>
#include <string>
#include <json/json.h>

#include "node.hpp"
#include "../readjson/hcdata.hpp"
#include "../patient/patient.hpp"
#include "../caregiver/caregiver.hpp"

namespace homecare {

class RoutesOpt;

class Route{
private:
    std::vector<Node> m_nodes;
    Caregiver m_caregiver;

    int m_maxTardiness;
    int m_maxIdleTime;
    int m_totalTardiness;
    int m_totalWaitingTime;
    int m_travelTime;

    int m_lastNode2DepotDistance;

    static const int BASE_ROUTE_LEN;
    static const int DEPOT;

    void makeTimeRecalculation(int);
    int addNode(Node, int, int, int);
    //void recalculateRoute(const int = 1);
    static std::vector<Node>::iterator nextNode(std::vector<Node>&, std::vector<Node>::iterator, bool);
    std::vector<Node> difference(const std::vector<Node>&, const std::vector<Node>&);
    bool validityControl(int) const;

public:
    static const int EMPTY_ROUTE = -1;
    static constexpr int ZERO_TIME = 0;

    Route(Caregiver);
    ~Route();
    // getter
    int getFreeTime() const;
    int getLastPatientDistanceIndex() const;
    std::vector<std::string> getAvilableServices() const;
    std::string getCaregiver() const;
    std::vector<Node> getNodes() const;
    Node getNodeToDestroy(int) const noexcept (false);
    int getLastNode2DepotDistance() const;
    int getNumNodes() const;
    int getMaxTardiness() const;
    int getMaxIdleTime() const;
    int getTotalTardiness() const;
    int getTotalWaitingTime() const;
    int getTravelTime() const;
    int getExtraTime() const;
    int getDeltaTime() const;
    std::string getHash() const;

    int addNode(Patient, int);
    int addNode(Node, int); 
    //solver
    /* 
    int addNodeBeetween(Node);
    int addNodeBeetween(Node, int, int); ////////////////////////////////////////////////////
    int findSuitablePosition(Node) const;
     */
    Route deleteNode(int, RoutesOpt&);
    std::tuple<Node, std::vector<Node>, std::vector<Node>> addNodeInRoute(Patient, RoutesOpt&);
    // toFileFormat
    std::string getRouteToString() const;
    Json::Value getJSONRoute() const;
    // checker
    bool isAvailable() const;
    bool hasService(std::string) const;
    //reader
    int readNodesFromJson(Json::Value, std::vector<Patient>, std::vector<int>) noexcept (false);
    //
    void updateRoute(std::vector<Node>&);
    static std::vector<Node> mergeLists(std::vector<Node>, std::vector<Node>, RoutesOpt&);

    int getNoChangeWindowCloseTime(int) const noexcept (false);
    int getNoChangeWindowOpenTime(int) const noexcept (false);
    void updateNodeTime(int, int);
};

}

#endif