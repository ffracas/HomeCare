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

bool ServiceManager::isPresent(string service) { return m_services.find(service) != m_services.end(); }

void ServiceManager::destroyAll() {
    for (auto& service : m_services) {
        service.second.destroy();
    }
}
