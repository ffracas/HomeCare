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
    static bool m_orderedPatientsList;
    static std::vector<Patient> m_patients;
    static std::vector<std::vector<int>> m_distances;
    static std::vector<Service> m_services;
    static std::vector<Depot> m_depots;
    static std::vector<Caregiver> m_caregivers;

public:
    static const double TRAVEL_TIME_WEIGHT;
    static const double MAX_TARDINESS_WEIGHT;
    static const double TARDINESS_WEIGHT;
    static const double EXTRA_TIME_WEIGHT;
    static const double MAX_IDLE_TIME_WEIGHT;
    static const double TOT_WAITING_TIME_WEIGHT;

    ~HCData();
    static void setData(std::string) noexcept (false);
    static std::vector<Patient> getPatients();
    static std::vector<std::vector<int>> getDistances();
    static std::vector<int> getNodeDistances(int) noexcept (false);
    static int getDistance(int, int) noexcept(false);
    static std::vector<Service> getServices();
    static std::vector<Depot> getDepots();
    static std::vector<Caregiver> getCaregivers();
    static std::vector<std::string> getCaregiversForService(std::string);
    static Caregiver getCaregiver(std::string) noexcept (false);
    static Patient getPatient(std::string) noexcept (false);
    static Patient getPatient(int) noexcept (false);
    static int getPatientPosition(std::string) noexcept (false);
};

}

#endif