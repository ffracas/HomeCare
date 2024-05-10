#ifndef ROUTE_HPP
#define ROUTE_HPP

#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <string>
#include <json/json.h>

#include "routeops.hpp"
#include "../node.hpp"
#include "../syncwindows/syncwindows.hpp"
#include "../../readjson/hcdata.hpp"
#include "../../patient/patient.hpp"
#include "../../caregiver/caregiver.hpp"

namespace homecare {

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

    static const int DEPOT = 0;

    //void makeTimeRecalculation(int);
    //int addNode(Node, int, int, int);
    //void recalculateRoute(const int = 1);
    //static std::vector<Node>::iterator nextNode(std::vector<Node>&, std::vector<Node>::iterator, bool);
    bool validityControl(int) const;
    void exploreData();

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
    Node getPatientNode(int) const noexcept (false);
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
    double getCost() const;
    int getNoChangeWindowCloseTime(int) const noexcept (false);
    int getNoChangeWindowOpenTime(int) const noexcept (false);

    //add node sequntial
    int appendNode(Node, int); 
    //solver
    void deleteNode(int, const SyncWindows&);
    void addNodeInRoute(Node, const SyncWindows&);
    // checker
    bool isAvailable() const;
    bool hasService(std::string) const;
    bool isIndexNodeValid(int) const;
    // updater
    void updateNodeTime(int, int);
    // output for File
    std::string getRouteToString() const;
    Json::Value getJSONRoute() const;
    // input from File
    int readNodesFromJson(Json::Value, std::vector<Patient>, std::vector<int>) noexcept (false);
};

}

#endif