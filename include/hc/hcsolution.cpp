#include "hcsolution.hpp"

using namespace homecare;
using namespace std;
using namespace Json;

const int HCSolution::NO_INDEX = -1;
const int HCSolution::MAX_INT = 2147483647;
const string HCSolution::I_SOL_FILE("./I_soluzione.json");

HCSolution::HCSolution(string t_dataPath) : m_data(t_dataPath) {
    for (const Caregiver& caregiver : m_data.getCaregivers()) {
        m_routes.push_back(Route(caregiver));
    }
}

HCSolution::~HCSolution() {}

int HCSolution::generateISolution() {
    //reset dictionary of served Sequential client and caregiver
    m_prevServCaregiver.clear();
    
    //initialise vector of patient to serve
    vector<Patient> patientsToServe(m_data.getPatients());
    if (patientsToServe.size() < 1 || m_routes.size() < 1) {
        throw std::runtime_error("Errore nei dati del problema o nella lettura");
        return -1;
    }
    
    //searching for starting solution
    while (patientsToServe.size() > 0) {
        int bestRoute = searchForRoute(patientsToServe[0]);
        int time = calculateArrivalTime(bestRoute, patientsToServe[0]);
        Node newNode(patientsToServe[0]);
        if (patientsToServe[0].getSync() == Simultaneous) {
            int bestSync = searchForRoute(patientsToServe[0], bestRoute);
            double syncTime = calculateArrivalTime(bestSync, patientsToServe[0]);
            time = time >= syncTime ? time : syncTime;
            m_routes[bestSync].addNode(newNode, 
                            m_data.getNodeDistances(m_routes[bestSync].getlastPatientDistanceIndex()), 
                            m_data.getNodeDistances(newNode.getDistancesIndex()), time);
        } else if (patientsToServe[0].getSync() == Sequential && patientsToServe[0].getServices().size() > 1) {
            patientsToServe.insert(patientsToServe.begin() + 0, patientsToServe[0].getPatientAndNextService());
            sort(patientsToServe.begin(), patientsToServe.end(), 
                [] (Patient p1, Patient p2) { return p1.getWindowEndTime() < p2.getWindowEndTime(); });
            m_prevServCaregiver[patientsToServe[0].getID()] = bestRoute;
        } 
        m_routes[bestRoute].addNode(newNode, 
                            m_data.getNodeDistances(m_routes[bestRoute].getlastPatientDistanceIndex()), 
                            m_data.getNodeDistances(newNode.getDistancesIndex()), time);

        patientsToServe.erase(patientsToServe.begin());
    }
    
    writeSolutionOnFile(I_SOL_FILE);

    return 0;
}

int HCSolution::searchForRoute(Patient patient, int sync_index)  {
    string request = patient.getServices()[0].getService();
    vector<string> invalidCaregivers(patient.getInvalidCaregivers());
    int bestRoute = NO_INDEX;
    int bestCost = MAX_INT;
    int i = 0;

    for (const Route& route: m_routes) {
        vector<string> services(route.getAvilableServices());
        if (i != sync_index 
                && find(services.begin(), services.end(), request) != services.end() 
                && find(invalidCaregivers.begin(), invalidCaregivers.end(), route.getCaregiver()) 
                    == invalidCaregivers.end()
                && (m_prevServCaregiver.find(patient.getID()) == m_prevServCaregiver.end()
                    || (m_prevServCaregiver.find(patient.getID()) != m_prevServCaregiver.end() 
                        && m_prevServCaregiver[patient.getID()] != i))
                ) {
            int cost = route.getFreeTime() + 
                        m_data.getDistance(route.getlastPatientDistanceIndex(), patient.getDistancesIndex());
            if (cost < bestCost) {
                bestCost = cost;
                bestRoute = i;
            }
        }
        ++i;
    }
    if (bestRoute == NO_INDEX) {
        throw std::runtime_error("Problema nella composizione della I soluzione");
    }
    return bestRoute;
}

int HCSolution::calculateArrivalTime(int route, Patient patient) {
    double serviceTimePrevision = m_routes[route].getFreeTime() + 
                m_data.getDistance(m_routes[route].getlastPatientDistanceIndex(), patient.getDistancesIndex());
    return patient.getWindowStartTime() > serviceTimePrevision ? patient.getWindowStartTime() : serviceTimePrevision;
}

int HCSolution::writeSolutionOnFile(string outputFilePath) {
    Json::Value routes;
    Json::StreamWriterBuilder builder;
    const std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    const string PATIENT_FIELD  ("global_ordering");
    const string ROUTE_FIELD    ("routes");
    const string CAREGIVER_FIELD("caregiver_id");
    const string PATIENTS_FIELD ("locations");

    vector<Patient> patients(m_data.getPatients());
    for (int i = 0; i < patients.size(); ++i) {
        routes[PATIENT_FIELD][i] = patients[i].getID();
    }

    int i = 0;
    for (const Route& route : m_routes) {
        routes[ROUTE_FIELD][i][CAREGIVER_FIELD] = route.getCaregiver();
        routes[ROUTE_FIELD][i][PATIENTS_FIELD]  = route.getJSONRoute();
        ++i;
    }

    std::ofstream outputFile(outputFilePath);
    writer -> write(routes, &outputFile);
    outputFile.close();

    return 0;
}