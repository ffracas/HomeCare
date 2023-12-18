#ifndef HCREADER_HPP
#define HCREADER_HPP

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

class hcreader {
private:
    vector<Patient> m_patients;
    vector<vector<int>> m_distances;
    vector<Service> m_services;
    vector<Depot> m_depots;
    vector<Caregiver> m_caregivers;

public:
    hcreader(string) noexcept (false);
    ~hcreader();
};

}

#endif