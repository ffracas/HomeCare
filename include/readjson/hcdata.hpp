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

namespace homecare {

class HCData {
private:
    bool m_orderedPatientsList;
    std::vector<Patient> m_patients;
    std::vector<std::vector<int>> m_distances;
    std::vector<Service> m_services;
    std::vector<Depot> m_depots;
    std::vector<Caregiver> m_caregivers;

public:
    static const double TRAVEL_TIME_WEIGHT;
    static const double MAX_TARDINESS_WEIGHT;
    static const double TARDINESS_WEIGHT;
    static const double EXTRA_TIME_WEIGHT;
    static const double MAX_IDLE_TIME_WEIGHT;
    static const double TOT_WAITING_TIME_WEIGHT;

    HCData(std::string) noexcept (false);
    ~HCData();
    std::vector<Patient> getPatients();
    std::vector<std::vector<int>> getDistances() const;
    std::vector<int> getNodeDistances(int) noexcept (false);
    int getDistance(int, int) noexcept(false);
    std::vector<Service> getServices() const;
    std::vector<Depot> getDepots() const;
    std::vector<Caregiver> getCaregivers() const;
    Caregiver getCaregiver(std::string) const noexcept (false);
    Patient getPatient(std::string) const noexcept (false);
    int getPatientPosition(std::string) const noexcept (false);
};

}

#endif