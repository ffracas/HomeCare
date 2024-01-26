#include "hcsolution.hpp"

using namespace homecare;
using namespace std;
using namespace Json;

const int HCSolution::NO_INDEX = -1;
const int HCSolution::MAX_INT = 2147483647;

const double HCSolution::m_travelTimeWeight = 0.165;
const double HCSolution::m_maxTardinessWeight = 0.165;
const double HCSolution::m_tardinessWeight = 0.165;
const double HCSolution::m_extraTimeWeight = 0.165;
const double HCSolution::m_maxIdleTimeWeight = 0.165;
const double HCSolution::m_totWaitingTimeWeight = 0.165;

const string HCSolution::I_SOL_FILE("./I_soluzione.json");

HCSolution::HCSolution(string t_dataPath) : m_data(t_dataPath), m_maxTardiness (0), m_maxIdleTime (0), 
        m_totalExtraTime (0), m_totalTardiness (0), m_totalWaitingTime (0), m_travelTime (0), m_solCost (MAX_INT) {
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
        
        if (currentPatient.getSync() != NoSync) {
            Patient syncPS(currentPatient.getPatientAndNextService());
            int bestSync = searchForRoute(syncPS, bestRoute);
            int syncTime = calculateArrivalTime(bestSync, PatientDistanceIndex);
            if (currentPatient.getSync() == Simultaneous) {
                time = time >= syncTime ? time : syncTime;
                syncTime = time;
            } 
            else if (currentPatient.getSync() == Sequential && currentPatient.hasNext()) {
                if (syncTime > time + syncPS.getMaxWait()) {
                    time = syncTime - syncPS.getMaxWait();
                }
                if (syncTime < time + syncPS.getMinWait()) {
                    syncTime = time + syncPS.getMinWait();
                }
            }
            m_routes[bestSync].addNode(syncPS, m_data.getNodeDistances(PatientDistanceIndex), syncTime, 
                    m_data.getDistance(m_routes[bestSync].getlastPatientDistanceIndex(), syncPS.getDistancesIndex()));
        } 
        m_routes[bestRoute].addNode(currentPatient, m_data.getNodeDistances(PatientDistanceIndex), time, 
            m_data.getDistance(m_routes[bestRoute].getlastPatientDistanceIndex(), currentPatient.getDistancesIndex()));
    }

    for (Route route : m_routes) {
        m_maxIdleTime = route.getMaxIdleTime() > m_maxIdleTime ? route.getMaxIdleTime() : m_maxIdleTime;
        m_maxTardiness = route.getMaxTardiness() > m_maxTardiness ? route.getMaxTardiness() : m_maxTardiness;
        m_totalTardiness += route.getTotalTardiness();
        m_totalWaitingTime += route.getTotalWaitingTime();
        m_travelTime += route.getTravelTime();
        m_totalExtraTime += route.getExtraTime();
    }
    //solution validation 
    HCValidation val(m_data, m_routes);
    cout << val.checkSolution() << "\n";

    //calculate cost
    m_solCost = calculateCost();
    cout << m_solCost << "\n";

    //write json
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

double HCSolution::calculateCost() {
    return m_maxIdleTimeWeight * m_maxIdleTime + m_maxTardinessWeight * m_maxTardiness 
        + m_tardinessWeight * m_totalTardiness + m_totWaitingTimeWeight * m_totalWaitingTime  
        + m_extraTimeWeight * m_totalExtraTime + m_travelTimeWeight * m_travelTime;
}

int HCSolution::writeSolutionOnFile(string outputFilePath) {
    Json::Value routes;
    Json::StreamWriterBuilder builder;
    const std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    const string PATIENT_FIELD  ("global_ordering");

    const string ROUTE_FIELD    ("routes");
    const string CAREGIVER_FIELD("caregiver_id");
    const string PATIENTS_FIELD ("locations");

    const string COST_VAL_FIELD ("cost_components");
    const string MAX_TARD_FIELD ("EHHC_HighestTardiness");
    const string MAX_WAIT_FIELD ("EHHC_MaxIdleTime");
    const string TOT_TARD_FIELD ("EHHC_TotalTardiness");
    const string TOT_WAIT_FIELD ("EHHC_TotalWaitingTime");
    const string T_TRAVEL_FIELD ("EHHC_TravelTime");
    const string EXTRA_T_FIELD  ("EHHC_TotalExtraTime");

    routes[COST_VAL_FIELD][MAX_TARD_FIELD] = m_maxTardiness;
    routes[COST_VAL_FIELD][MAX_WAIT_FIELD] = m_maxIdleTime;
    routes[COST_VAL_FIELD][TOT_TARD_FIELD] = m_totalTardiness;
    routes[COST_VAL_FIELD][TOT_WAIT_FIELD] = m_totalWaitingTime;
    routes[COST_VAL_FIELD][T_TRAVEL_FIELD] = m_travelTime;
    routes[COST_VAL_FIELD][EXTRA_T_FIELD ] = m_totalExtraTime;

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