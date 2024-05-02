#include "servicemanager.hpp"

using namespace std;
using namespace homecare;

ServiceManager::ServiceManager() : m_services () {}

ServiceManager::~ServiceManager() {}

void ServiceManager::insertService(string key, InfoNode value) { 
    m_services.push_back(make_pair(key, value)); 
}

vector<InfoNode>  ServiceManager::getAllService() const { 
    vector<InfoNode> services;
    for (const auto& service : m_services) {
        services.push_back(service.second);
    }
    return services; 
}

int ServiceManager::size() { return m_services.size(); }

bool ServiceManager::isPresent(string service) const { 
    for (auto it = m_services.begin(); it != m_services.end(); ++it) {
        if (it->first == service) {
            return true;
        }
    }
    return false;
}

void ServiceManager::destroyAll() {
    for (auto& service : m_services) {
        service.second.destroy();
    }
}

int ServiceManager::relocateNode(string service, int route, int arrival, int routePos) {
    for (auto it = m_services.begin(); it != m_services.end(); ++it) {
        if (it->first == service && !it->second.isAssigned()) {
            it->second.setInRoute(route, routePos, arrival);
            return 1;
        }
    }
    return 0;
}

pair<string, InfoNode> ServiceManager::getOtherServiceInfo(string service, int currentRoute) const {
    for (auto it = m_services.begin(); it != m_services.end(); ++it ) {
        if (it->first != service || it->second.getRoute() != currentRoute) {
            return *it;
        }
    }
    throw runtime_error("[service manager] Service not found");
}

pair<string, InfoNode> ServiceManager::update(string service, int currentRoute, int openWin, int closeWin) {
    for (auto it = m_services.begin(); it != m_services.end(); ++it) {
        if (it->first == service && it->second.getRoute() == currentRoute) {
            if (it->second.getTime() > closeWin) {
                it->second.setTime(closeWin);
            } else if (it->second.getTime() < openWin) {
                it->second.setTime(openWin);
            }
            return make_pair(it->first, it->second);
        }
    }
    throw runtime_error("[service manager] Service not found");
}
