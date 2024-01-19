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
        
    }
    
    //searching for starting solution
    while (patientsToServe.size() > 0) {
        Patient currentPatient(patientsToServe[0]);
        patientsToServe.erase(patientsToServe.begin());
        int bestRoute = searchForRoute(currentPatient);
        int PatientDistanceIndex = currentPatient.getDistancesIndex();
        int time = calculateArrivalTime(bestRoute, PatientDistanceIndex);
        
        if (currentPatient.getSync() == Simultaneous) {
            Patient syncPS(currentPatient.getPatientAndNextService());
            int bestSync = searchForRoute(syncPS, bestRoute);
            int syncTime = calculateArrivalTime(bestSync, PatientDistanceIndex);
            time = time >= syncTime ? time : syncTime;
            m_routes[bestSync].addNode(syncPS, m_data.getNodeDistances(PatientDistanceIndex), time);
        } 
        else if (currentPatient.getSync() == Sequential && currentPatient.hasNext()) {
            patientsToServe.insert(patientsToServe.begin() + 0, currentPatient.getPatientAndNextService(time));
            sort(patientsToServe.begin(), patientsToServe.end(), 
                [] (Patient p1, Patient p2) { return p1.getWindowEndTime() < p2.getWindowEndTime(); });
            m_prevServCaregiver[currentPatient.getID()] = bestRoute;
        } 
        m_routes[bestRoute].addNode(currentPatient, m_data.getNodeDistances(PatientDistanceIndex), time);
    }

    /*for (Route route : m_routes) {
        cout << route.getRouteToString() << '\n';
    }*/
    HCValidation val(m_data, m_routes);
    cout<<val.checkSolution();

    writeSolutionOnFile(I_SOL_FILE);

    return 0;
}

int HCSolution::searchForRoute(Patient patient, int sync_index)  {
    string request = patient.getCurrentService().getService();
    vector<string> invalidCaregivers(patient.getInvalidCaregivers());
    int bestRoute = NO_INDEX;
    int bestCost = MAX_INT;

    for (int i = 0; i < m_routes.size(); ++i) {
        if (i != sync_index 
                && m_routes[i].hasService(request)
                && find(invalidCaregivers.begin(), invalidCaregivers.end(), m_routes[i].getCaregiver()) 
                    == invalidCaregivers.end()
                && (m_prevServCaregiver.find(patient.getID()) == m_prevServCaregiver.end()
                    || (m_prevServCaregiver.find(patient.getID()) != m_prevServCaregiver.end() 
                        && m_prevServCaregiver[patient.getID()] != i))
                ) {
            int cost = m_routes[i].getFreeTime() + 
                        m_data.getDistance(m_routes[i].getlastPatientDistanceIndex(), patient.getDistancesIndex());
            if (cost < bestCost) {
                bestCost = cost;
                bestRoute = i;
            }
        }
    }
    return bestRoute;
}

int HCSolution::calculateArrivalTime(int route, int patient) {
    return m_routes[route].getFreeTime() + 
                m_data.getDistance(m_routes[route].getlastPatientDistanceIndex(), patient);
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