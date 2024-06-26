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
    void exploreData();

public:
    static const int EMPTY_ROUTE = -1;
    static constexpr int ZERO_TIME = 0;

    Route(Caregiver);
    ~Route();
    
    // GETTERS
    // last
    int getFreeTime() const;
    int getLastPatientDistanceIndex() const;
    int getLastNode2DepotDistance() const;
    // services and caregivers
    std::vector<std::string> getAvilableServices() const;
    std::string getCaregiver() const;
    // nodes
    std::vector<Node> getNodes() const;
    int getNumNodes() const;
    Node getPatientNode(int) const noexcept (false);
    int getNodeArrivalTime(std::string) const noexcept (false);
    // costs
    int getMaxTardiness() const;
    int getMaxIdleTime() const;
    int getTotalTardiness() const;
    int getTotalWaitingTime() const;
    int getTravelTime() const;
    int getExtraTime() const;
    double getCost() const;
    // hash
    std::string getHash() const;
    //todo: metodo che ritorna il tempo di arrivo ad un determinato nodo
    // time window
    int getNoChangeWindowCloseTime(int) const noexcept (false);
    int getNoChangeWindowOpenTime(int) const noexcept (false);

    //add node sequntial
    int appendNode(Node, int); 
    //solver
    void deleteNode(int, const SyncWindows&);
    int addNodeInRoute(Node, const SyncWindows&);
    // checker
    bool hasService(std::string) const;
    bool isIndexNodeValid(int) const;
    // updater
    int reschedule(const SyncWindows&);
    void updateNodeTime(int, int);
    // output for File
    std::string getRouteToString() const;
    Json::Value getJSONRoute() const;
    // input from File
    int readNodesFromJson(Json::Value, std::vector<Patient>, std::vector<int>) noexcept (false);
};

}

#endif