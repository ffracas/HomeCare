#ifndef HCSOLVER_HPP
#define HCSOLVER_HPP

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

class hcsolver {
private:
    vector<Patient> m_patients;
    vector<vector<int>> m_distances;
    vector<Service> m_services;
    vector<Depot> m_depots;
    vector<Caregiver> m_caregivers;

public:
    hcsolver(string) noexcept (false);
    ~hcsolver();
};

}

#endif