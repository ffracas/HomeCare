#ifndef ROUTESOPT_HPP
#define ROUTESOPT_HPP

#include <map>
#include <vector>
#include <sstream>

#include "../../readjson/hcdata.hpp"
#include "../../structures/route.hpp"
#include "infonode.hpp"
#include "servicemanager.hpp"

namespace homecare {

class RoutesOpt {
private:
    std::vector<Route> m_routes;
    std::map<std::string, ServiceManager> m_mapOfPatient;
    
public:
    RoutesOpt();
    RoutesOpt(std::vector<Route>&);
    ~RoutesOpt();

    RoutesOpt replaceRoute(Route&, int);
    
    std::vector<Route> getRoutes() const;
    Route getRoute(int) const;
    Node getNodeInRoute(int, int) noexcept (false);
    std::string getRouteCaregiver(int) const noexcept (false);
    int getNumberOfRoutes() const;
    int getNumberOfNodesInRoute(int) const noexcept (false);

    std::vector<InfoNode> getPatientServices(std::string) const;
    int getNPatientServices(std::string) const;
    
    bool isServiceAvailableInRoute(std::string, int) const;
    bool isEmpty() const;
    
    void destroyReferencesForPatient(std::string);
    std::pair<int, int> getSyncServiceWindow(std::string, std::string, int) noexcept(false);              
    void updateSyncServiceTime(std::string, std::string, int, int);          
    std::pair<std::string, InfoNode> getInterdependetInfo(std::string, std::string, int); 
};

}

#endif
