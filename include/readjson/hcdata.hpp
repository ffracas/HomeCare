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
    bool m_orderedPatientsList;
    vector<Patient> m_patients;
    vector<vector<int>> m_distances;
    vector<Service> m_services;
    vector<Depot> m_depots;
    vector<Caregiver> m_caregivers;

public:
    HCData(string) noexcept (false);
    ~HCData();
    vector<Patient> getPatients();
    vector<vector<int>> getDistances() const;
    vector<int> getNodeDistances(int) noexcept(false);
    int getDistance(int, int) noexcept(false);
    vector<Service> getServices() const;
    vector<Depot> getDepots() const;
    vector<Caregiver> getCaregivers() const;
    Caregiver getCaregiver(string) const;
};

}

#endif