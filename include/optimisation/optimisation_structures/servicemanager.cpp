#include "servicemanager.hpp"

using namespace std;
using namespace homecare;

ServiceManager::ServiceManager() : m_services () {}

ServiceManager::~ServiceManager() {}

void ServiceManager::insertService(string key, InfoNode value) { m_services.insert(make_pair(key, value)); }

InfoNode ServiceManager::getInfoForService(string key) { return m_services[key]; }

vector<InfoNode>  ServiceManager::getAllService() const { 
    vector<InfoNode> services;
    for (const auto& service : m_services) {
        services.push_back(service.second);
    }
    return services; 
}

int ServiceManager::size() { return m_services.size(); }

bool ServiceManager::isPresent(string service) const { return m_services.find(service) != m_services.end(); }

void ServiceManager::destroyAll() {
    for (auto& service : m_services) {
        service.second.destroy();
    }
}

InfoNode ServiceManager::getOtherServiceInfo(string service) const {
    for (auto it = m_services.begin(); it != m_services.end(); ++it ) {
        if (it->first != service) {
            return it -> second;
        }
    }
}

InfoNode ServiceManager::update(string service, int time, int openWin, int closeWin) {
    if (m_services[service].getTime() > closeWin) {
        m_services[service].setTime(closeWin);
    }
    else if (m_services[service].getTime() < openWin) {
        m_services[service].setTime(openWin);
    }
    return m_services[service];
} 
