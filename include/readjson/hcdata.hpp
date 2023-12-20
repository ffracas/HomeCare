#ifndef HCDATA_HPP
#define HCDATA_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <json/json.h> 

#include "../patient/patient.hpp"
#include "../service/service.hpp"
#include "../depot/depot.hpp"
#include "../caregiver/caregiver.hpp"

using namespace std;
namespace homecare {

class HCData {
private:
    vector<Patient> m_patients;
    vector<vector<int>> m_distances;
    vector<Service> m_services;
    vector<Depot> m_depots;
    vector<Caregiver> m_caregivers;

public:
    HCData(string) noexcept (false);
    ~HCData();
    const vector<Patient> getPatients() const;
    const vector<std::vector<int>> getDistances() const;
    const int getDistance(int, int) noexcept(false);
    const vector<Service> getServices() const;
    const vector<Depot> getDepots() const;
    const vector<Caregiver> getCaregivers() const;
};

}

#endif