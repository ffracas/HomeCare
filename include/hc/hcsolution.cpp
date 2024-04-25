#include "hcsolution.hpp"

using namespace homecare;
using namespace std;
using namespace Json;

const string HCSolution::I_SOL_FILE("./I_soluzione.json");

HCSolution::HCSolution() : m_maxTardiness (0), m_maxIdleTime (0), 
        m_totalExtraTime (0), m_totalTardiness (0), m_totalWaitingTime (0), m_travelTime (0), 
        m_solCost (HCData::MAX_COST), m_schedule(HCData::getCaregivers()) {}

HCSolution::~HCSolution() {}

int HCSolution::generateISolution() {
    //initialise vector of patient to serve
    vector<Patient> patientsToServe(HCData::getPatients());
    if (patientsToServe.size() < 1 || m_schedule.getNumberOfRoutes() < 1) {
        throw std::runtime_error("Errore nei dati del problema o nella lettura");
    }
    
    //searching for starting solution 
    while (patientsToServe.size() > 0) {
        m_schedule.greedyAppend(patientsToServe[0]);
        patientsToServe.erase(patientsToServe.begin());
    }

    updateCostData();

    //solution validation 
    HCValidation val(m_schedule.getSchedule());
    cout << "\n"<<val.checkSolution() << "\n";

    //calculate cost
    m_solCost = calculateCost();
    cout << m_solCost << "\n";

    //write json
    writeSolutionOnFile(I_SOL_FILE);
    
    return 0;
}

void homecare::HCSolution::updateCostData()
{
    m_maxIdleTime      = 0;
    m_maxTardiness     = 0;
    m_totalTardiness   = 0;
    m_totalWaitingTime = 0; 
    m_travelTime       = 0;   
    m_totalExtraTime   = 0;

    for (Route route : m_schedule.getSchedule()) {
        m_maxIdleTime = route.getMaxIdleTime() > m_maxIdleTime ? route.getMaxIdleTime() : m_maxIdleTime;
        m_maxTardiness = route.getMaxTardiness() > m_maxTardiness ? route.getMaxTardiness() : m_maxTardiness;
        m_totalTardiness += route.getTotalTardiness();
        m_totalWaitingTime += route.getTotalWaitingTime();
        m_travelTime += route.getTravelTime();
        m_totalExtraTime += route.getExtraTime();
    }
}

// FIXME: Aggiornare HCOptimisation 
int HCSolution::optimizeSolution() {
    m_schedule = HCOptimisation(m_schedule, calculateCost()).optimise();
    updateCostData();
    //solution validation 
    HCValidation val(m_schedule.getSchedule());
    cout << val.checkSolution() << "\n";

    //calculate cost
    m_solCost = calculateCost();
    cout << m_solCost << "\n";
    //write json
    writeSolutionOnFile("salcazzi.json");
    return m_solCost;
}

double HCSolution::calculateCost() {
    return HCData::MAX_WAIT_TIME_WEIGHT * m_maxIdleTime + HCData::MAX_TARDINESS_WEIGHT * m_maxTardiness 
        + HCData::TARDINESS_WEIGHT * m_totalTardiness + HCData::TOT_WAITING_TIME_WEIGHT * m_totalWaitingTime  
        + HCData::EXTRA_TIME_WEIGHT * m_totalExtraTime + HCData::TRAVEL_TIME_WEIGHT * m_travelTime;
}

//TODO: static da spostare da qualche parte forse HCData
double HCSolution::calculateCost(std::vector<Route>& routes) {
    int maxIdleTime      = 0;
    int maxTardiness     = 0;
    int totalTardiness   = 0;
    int totalWaitingTime = 0; 
    int travelTime       = 0;   
    int totalExtraTime   = 0;
    for (Route route : routes) {
        maxIdleTime = route.getMaxIdleTime() > maxIdleTime ? route.getMaxIdleTime() : maxIdleTime;
        maxTardiness = route.getMaxTardiness() > maxTardiness ? route.getMaxTardiness() : maxTardiness;
        totalTardiness += route.getTotalTardiness();
        totalWaitingTime += route.getTotalWaitingTime();
        travelTime += route.getTravelTime();
        totalExtraTime += route.getExtraTime();
    }
    return HCData::MAX_WAIT_TIME_WEIGHT * maxIdleTime + HCData::MAX_TARDINESS_WEIGHT * maxTardiness +  
        HCData::TARDINESS_WEIGHT * totalTardiness + HCData::TOT_WAITING_TIME_WEIGHT * totalWaitingTime +    
        HCData::EXTRA_TIME_WEIGHT * totalExtraTime + HCData::TRAVEL_TIME_WEIGHT * travelTime;
}

/////////////////////////////////////////////////////////////////////////////////////////     OUTPUT

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

    vector<Patient> patients(HCData::getPatients());
    for (int i = 0; i < patients.size(); ++i) {
        routes[PATIENT_FIELD][i] = patients[i].getID();
    }

    int i = 0;
    for (const Route& route : m_schedule.getSchedule()) {
        routes[ROUTE_FIELD][i][CAREGIVER_FIELD] = route.getCaregiver();
        routes[ROUTE_FIELD][i][PATIENTS_FIELD]  = route.getJSONRoute();
        ++i;
    }

    std::ofstream outputFile(outputFilePath);
    writer -> write(routes, &outputFile);
    outputFile.close();

    return 0;
}