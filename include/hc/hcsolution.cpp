#include "hcsolution.hpp"

using namespace homecare;
using namespace std;

HCSolution::HCSolution(string t_dataPath) : m_data(t_dataPath) {
    for (const Caregiver& caregiver : m_data.getCaregivers()) {
        m_routes.push_back(Route(caregiver));
    }
}

HCSolution::~HCSolution() {}

int HCSolution::generateISolution() {
    vector<Patient> patientsToServe(m_data.getPatients());
    if (patientsToServe.size() < 1 || m_routes.size() < 1) {
        throw std::runtime_error("Errore nei dati del problema o nella lettura");
        return -1;
    }

    sort(patientsToServe.begin(), patientsToServe.end(), 
        [] (Patient p1, Patient p2) { return p1.getWindowEndTime() > p2.getWindowEndTime(); });
    
    while (patientsToServe.size() > 0) {
        vector<Route> availableRoute;
        copy_if(m_routes.begin(), m_routes.end(), back_inserter(availableRoute), 
            [patientsToServe] (Route r) { 
                vector<string> services(r.getAvilableServices());
                return find(services.begin(), services.end(), patientsToServe[0].getServices()[0].getService()) 
                        != services.end(); 
            });
    }

    return 0;
}