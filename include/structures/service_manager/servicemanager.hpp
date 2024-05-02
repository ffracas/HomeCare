#ifndef SERVICEMANAGER_HPP
#define SERVICEMANAGER_HPP

#include <map>
#include <string>
#include <iostream>
#include <utility>
#include <vector>

#include "infonode.hpp"

namespace homecare {

class ServiceManager {
private:
    std::vector<std::pair<std::string, InfoNode>> m_services;
public:
    ServiceManager();
    ~ServiceManager();
    void insertService(std::string, InfoNode);
    //InfoNode getInfoForService(std::string);
    std::vector<InfoNode> getAllService() const;
    int size();
    bool isPresent(std::string) const;
    void destroyAll();
    int relocateNode(std::string, int, int, int);
    std::pair<std::string, InfoNode> getOtherServiceInfo(std::string, int) const noexcept (false); 
    std::pair<std::string, InfoNode> update(std::string, int, int, int) noexcept(false);               
};

}

#endif