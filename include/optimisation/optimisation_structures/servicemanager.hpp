#ifndef SERVICEMANAGER_HPP
#define SERVICEMANAGER_HPP

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "infonode.hpp"

namespace homecare {

class ServiceManager {
private:
    std::map<std::string, InfoNode> m_services;
public:
    ServiceManager();
    ~ServiceManager();
    void insertService(std::string, InfoNode);
    InfoNode getInfoForService(std::string);
    std::vector<InfoNode> getAllService() const;
    int size();
    bool isPresent(std::string) const;
    void destroyAll();
    InfoNode getOtherServiceInfo(std::string) const noexcept (false); 
    InfoNode update(std::string, int, int, int);                 //nuovo infonode di other service
};

}

#endif