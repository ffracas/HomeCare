#include "route.hpp"

using namespace std;
using namespace homecare;

const int Route::BASE_ROUTE_LEN = 2;
const int Route::DEPOT = 0;

Route::Route(Caregiver t_caregiver) : m_caregiver(t_caregiver)
{
    m_nodes.push_back(Node(t_caregiver));
    m_maxTardiness = 0;
    m_maxIdleTime = 0;
    m_totalTardiness = 0;
    m_totalWaitingTime = 0;
    m_travelTime = 0;
    m_lastNode2DepotDistance = 0;
}

Route::~Route() {}

int Route::getFreeTime() const { return m_nodes[m_nodes.size() - 1].getDeparturTime(); }

int Route::getLastPatientDistanceIndex() const { return m_nodes[m_nodes.size() - 1].getDistancesIndex(); }

int Route::addNode(Patient t_newPatient, int t_estimatedArrivalTime)
{
    return addNode(Node(t_newPatient, t_estimatedArrivalTime), t_estimatedArrivalTime);
}

/**
 * Adds a new node to the route and updates route statistics.
 *
 * @param t_newNode The new node to be added to the route.
 * @param t_new2DepotDistance The distance from the new node to the depot.
 * @param t_estimatedArrivalTime The estimated arrival time at the new node.
 * @param t_last2newDistance The distance from the last node to the new node.
 * @return The index of the newly added node in the route.
 */
int Route::addNode(Node t_newNode, int t_estimatedArrivalTime)
{
    // update cost data
    // sub distance of the last arc
    m_travelTime -= m_lastNode2DepotDistance;
    m_travelTime += HCData::getDistance(t_newNode.getDistancesIndex(), m_caregiver.getDepotDistanceIndex());
    // update waiting time
    if (t_estimatedArrivalTime < t_newNode.getTimeWindowOpen())
    {
        int earliness = t_newNode.getTimeWindowOpen() - (m_nodes[m_nodes.size() - 1].getDeparturTime() + HCData::getDistance(getLastPatientDistanceIndex(), t_newNode.getDistancesIndex()));
        m_totalWaitingTime += earliness;
        m_maxIdleTime = max(earliness, m_maxIdleTime);
        t_estimatedArrivalTime = t_newNode.getTimeWindowOpen();
    }
    // update tardiness
    if (t_estimatedArrivalTime > t_newNode.getTimeWindowClose())
    {
        int tardiness = t_estimatedArrivalTime - t_newNode.getTimeWindowClose();
        m_totalTardiness += tardiness;
        m_maxTardiness = max(tardiness, m_maxTardiness);
    }

    // add new node
    t_newNode.setArrivalTime(t_estimatedArrivalTime);
    m_nodes.push_back(t_newNode);
    m_lastNode2DepotDistance = HCData::getDistance(getLastPatientDistanceIndex(), t_newNode.getDistancesIndex());
    m_nodes[DEPOT].setArrivalTime(m_nodes[m_nodes.size() - 1].getDeparturTime() + m_lastNode2DepotDistance);

    // update cost data (travel time)
    m_travelTime += m_lastNode2DepotDistance;

    return m_nodes.size() - 1;
}

Route Route::deleteNode(int node)
{
    int nNodes = m_nodes.size();
    if (nNodes == 1 || node >= nNodes || node == DEPOT)
    {
        throw std::runtime_error("Constraint on node destruction.");
    }
    m_nodes.erase(m_nodes.begin() + node);
    recalculateRoute();
    return *this;
}

void Route::recalculateRoute(int initIndex)
{
    Route newRoute(m_caregiver); // Initialize new route
    // Copia i nodi dal primo fino a initIndex - 1 nel nuovo percorso
    for (int i = 1; i < initIndex; ++i)
    {
        newRoute.addNode(m_nodes[i],
                         HCData::getDistance(m_nodes[i].getDistancesIndex(), m_caregiver.getDepotDistanceIndex()),
                         m_nodes[i].getArrivalTime(),
                         HCData::getDistance(newRoute.getLastNode2DepotDistance(),
                                             m_nodes[i].getDistancesIndex()));
    }

    // set iterator for different services
    vector<Node>::iterator indepI = nextNode(m_nodes.begin() + initIndex, false);
    vector<Node>::iterator interdepI = nextNode(m_nodes.begin() + initIndex, true);

    while (indepI != m_nodes.end() || interdepI != m_nodes.end())
    {
        vector<Node>::iterator newNode;
        int last2newDistance;
        int hypotheticalRouteTime = 0;

        if (indepI == m_nodes.end())
        {
            hypotheticalRouteTime = interdepI->getArrivalTime();
            newNode = interdepI;
            interdepI = nextNode(++interdepI, true);
        }
        else
        {
            last2newDistance = HCData::getDistance(newRoute.getLastPatientDistanceIndex(),
                                                   indepI->getDistancesIndex());
            hypotheticalRouteTime = newRoute.getFreeTime() + last2newDistance;
            hypotheticalRouteTime = max(hypotheticalRouteTime, indepI->getTimeWindowOpen());
            indepI->setArrivalTime(hypotheticalRouteTime);
            if (interdepI == m_nodes.end())
            {
                newNode = indepI;
                indepI = nextNode(++indepI, false);
            }
            else
            {
                if (indepI->getDeparturTime() + HCData::getDistance(indepI->getDistancesIndex(),
                                                                    interdepI->getDistancesIndex()) <=
                    interdepI->getArrivalTime())
                {
                    newNode = indepI;
                    indepI = nextNode(++indepI, false);
                }
                else
                {
                    hypotheticalRouteTime = interdepI->getArrivalTime();
                    newNode = interdepI;
                    interdepI = nextNode(++interdepI, true);
                }
            }
        }
        last2newDistance = HCData::getDistance(newRoute.getLastPatientDistanceIndex(),
                                               newNode->getDistancesIndex());
        newRoute.addNode(*newNode,
                         HCData::getDistance(newNode->getDistancesIndex(),
                                             m_caregiver.getDepotDistanceIndex()),
                         hypotheticalRouteTime, last2newDistance);
    }

    // Update the current route with the new route's data
    m_maxTardiness = newRoute.getMaxTardiness();
    m_maxIdleTime = newRoute.getMaxIdleTime();
    m_totalTardiness = newRoute.getTotalTardiness();
    m_totalWaitingTime = newRoute.getTotalWaitingTime();
    m_travelTime = newRoute.getTravelTime();
    m_lastNode2DepotDistance = newRoute.getLastNode2DepotDistance();

    // Clear current nodes and copy new nodes
    m_nodes.clear();
    m_nodes.insert(m_nodes.begin(), newRoute.getNodes().begin(), newRoute.getNodes().end());
}

vector<Node>::iterator Route::nextNode(vector<Node>::iterator start, bool interdependent)
{
    while (start != m_nodes.end())
    {
        if ((interdependent && start->isInterdependent()) || (!interdependent && !start->isInterdependent()))
        {
            return start;
        }
        ++start;
    }
    return m_nodes.end(); // Non trovato
}

int Route::findSuitablePosition(Node newNode) const
{
    string service = newNode.getService();
    if (!hasService(service))
    {
        return -1;
    }
    int time;
    for (int i = 0; i < m_nodes.size(); ++i)
    {
        time = m_nodes[i].getDeparturTime() + HCData::getDistance(m_nodes[i].getDistancesIndex(), newNode.getDistancesIndex());
        if (time <= newNode.getTimeWindowClose() && time >= newNode.getTimeWindowOpen())
        {
            return i + 1;
        }
        if (time > newNode.getTimeWindowClose())
        {
            time = m_nodes[i - 1].getDeparturTime() + HCData::getDistance(m_nodes[i - 1].getDistancesIndex(), newNode.getDistancesIndex());
            return i;
        }
    }
    return m_nodes.size();
}

int Route::addNodeBeetween(Node newNode)
{
    int position = findSuitablePosition(newNode);
    m_nodes.insert(m_nodes.begin() + position, newNode);
    recalculateRoute();
    if (m_nodes[position].getId() == newNode.getId())
    {
        return position;
    }
    else
    {
        auto it = find_if(m_nodes.begin(), m_nodes.end(),
                          [newNode](const Node &n)
                          { return newNode.getId() == n.getId(); });
        // If element was found
        if (it != m_nodes.end())
        {
            // calculating the index
            return distance(m_nodes.begin(), it);
        }
        return -1;
    }
}

tuple<Node, vector<Node>, vector<Node>> Route::addNodeInRoute(Patient patient, ALNSOptimisation &blackops)
{
    vector<Node> first, second;
    first.push_back(*m_nodes.begin());
    Node newNode(patient, 0);
    vector<Node>::iterator indepI = nextNode(m_nodes.begin() + 1, false);
    vector<Node>::iterator interdepI = nextNode(m_nodes.begin() + 1, true);

    while (interdepI != m_nodes.end() && indepI != m_nodes.end())
    {
        Node last = first.back();
        int time = last.getDeparturTime();

        bool isNewN = false;
        // prevision time for new node
        int prevNew = time + HCData::getDistance(last.getDistancesIndex(), patient.getDistancesIndex());
        newNode.setArrivalTime(prevNew);
        // prevision time for independent
        int prevInd = time + HCData::getDistance(last.getDistancesIndex(), indepI->getDistancesIndex());
        Node indNode(*indepI);
        indNode.setArrivalTime(prevInd);
        // next Node is the new one or the old one
        if (prevNew <= prevInd) {
            isNewN = true;
        }
        // compare with interdependent node time
        int prevNewToSync = toIns -> getDeparturTime() 
                        + HCData::getDistance(toIns->getDistancesIndex(), interdepI -> getDistancesIndex());
        pair<int, int> syncWin = blackops.getSyncServiceWindow(interdepI->getId(), interdepI->getService());
        // è possibile infilare il servizio prima di quello sincrono senza scombinare l'altra route?
        if (prevNewToSync >= syncWin.first && prevNewToSync <= syncWin.second) {
            // trovato il posto del nuovo nodo nella lista 
            if (isNewN) {
                int n_el = first.size();
                std::copy(m_nodes.begin() + n_el, m_nodes.end(), second.begin());
                return make_tuple(newNode, first, second);
            }
            // trovato il posto del vecchio nodo nella lista 
            else {
                indepI = nextNode(++indepI, false);
                first.push_back(*toIns);
                // inserisce il nodo interdipendente
                interdepI -> setArrivalTime(prevNewToSync);
                first.push_back(*interdepI);
                blackops.updateSyncServiceTime(interdepI->getId(), interdepI->getService(), prevNewToSync);
                interdepI = nextNode(++interdepI, true);
            }
        } // altrimenti SE il tempo è oltre metti prima il sincrono
        else if (prevNewToSync > syncWin.second) {
            interdepI->setArrivalTime(syncWin.first);
            first.push_back(*interdepI);
            blackops.updateSyncServiceTime(interdepI->getId(), interdepI->getService(), syncWin.first);
            interdepI = nextNode(++interdepI, true);
            // trovato il posto del nuovo nodo nella lista 
            if (isNewN) {
                int n_el = first.size();
                std::copy(m_nodes.begin() + n_el, m_nodes.end(), second.begin());
                return make_tuple(newNode, first, second);
            }

        }

        /*if (prevNew >= patient.getWindowStartTime()) {
            // departure time from newNode
            int prevNewToSync = prevNew + patient.getCurrentService().getDuration()
                                // distance newNode - next interdep
                                + HCData::getDistance(patient.getDistancesIndex(), interdepI->getDistancesIndex());
            pair<int, int> syncWin = blackops.getSyncServiceWindow(interdepI->getId(), interdepI->getService());
            if (prevNewToSync >= syncWin.first && prevNewToSync <= syncWin.second) {
                first.push_back(Node(patient, prevNew));
                interdepI->setArrivalTime(prevNewToSync);
                first.push_back(*interdepI);
                blackops.updateSyncServiceTime(interdepI->getId(), interdepI->getService(), prevNewToSync);
            }
            else {
                int newTime = interdepI->getDeparturTime() + HCData::getDistance(interdepI->getDistancesIndex(), patient.getDistancesIndex());
                first.push_back(*interdepI);
                first.push_back(Node(patient, newTime));
            }
        }*/
    }
    while (interdepI == m_nodes.end() && indepI != m_nodes.end())
    {
    }
    while (interdepI != m_nodes.end() && indepI == m_nodes.end())
    {
    }
}

int Route::getLastNode2DepotDistance() const { return m_lastNode2DepotDistance; }

/**
 * Returns the list of available services for the route.
 * @return The list of available services for the route.
 */
vector<string> Route::getAvilableServices() const { return m_caregiver.getServicesList(); }

/**
 * Returns the ID of the caregiver associated with the route.
 * @return The ID of the caregiver associated with the route.
 */
string Route::getCaregiver() const { return m_caregiver.getID(); }

/**
 * Returns the number of nodes in the route.
 * @return The number of nodes in the route.
 */
int Route::getNumNodes() const { return m_nodes.size(); }

/**
 * Returns the node at the specified position in the route.
 * @param pos The position of the node.
 * @return The node at the specified position in the route.
 * @throws std::runtime_error if the route contains only one node.
 */
Node Route::getNodeToDestroy(int pos) const
{
    int nNodes = m_nodes.size();
    if (nNodes == 1 || pos >= nNodes || pos == DEPOT)
    {
        throw std::runtime_error("Constraint on route selection.");
    }
    return m_nodes[pos];
}

/**
 * Returns the maximum tardiness for the route.
 * @return The maximum tardiness for the route.
 */
int Route::getMaxTardiness() const { return m_maxTardiness; }

/**
 * Returns the maximum idle time for the route.
 * @return The maximum idle time for the route.
 */
int Route::getMaxIdleTime() const { return m_maxIdleTime; }

/**
 * Returns the total tardiness for the route.
 * @return The total tardiness for the route.
 */
int Route::getTotalTardiness() const { return m_totalTardiness; }

/**
 * Returns the total waiting time for the route.
 * @return The total waiting time for the route.
 */
int Route::getTotalWaitingTime() const { return m_totalWaitingTime; }

/**
 * Returns the travel time for the route.
 * @return The travel time for the route.
 */
int Route::getTravelTime() const { return m_travelTime; }

int Route::getExtraTime() const
{
    int endTime = m_nodes[DEPOT].getArrivalTime() - m_caregiver.getShiftEndTime();
    return endTime > 0 ? endTime : 0;
}

string Route::getHash() const
{
    stringstream ss;

    ss << m_caregiver.getID() << "--";
    for (const Node &node : m_nodes)
    {
        ss << node.getHash() << '-';
    }
    ss << "--";
    return ss.str();
}

bool Route::isAvailable() const { return m_caregiver.isWorking(this->getFreeTime()); }

bool Route::hasService(string request) const
{
    vector<string> availableServices = m_caregiver.getServicesList();
    return find(availableServices.begin(), availableServices.end(), request) != availableServices.end();
}

vector<Node> Route::getNodes() const { return m_nodes; }

int Route::readNodesFromJson(Json::Value t_patientsInJson, vector<Patient> t_patients, vector<int> t_depotDistances)
{

    const string PATIENT_FIELD("patient");
    const string SERVICE_FIELD("service");
    const string ARRIVAL_FIELD("arrival_time");
    const string DEPARTURE_FIELD("departure_time");

    for (int i = 0; i < t_patientsInJson.size(); ++i)
    {
        string patientToSearch = t_patientsInJson[i][PATIENT_FIELD].asString();
        vector<Patient>::iterator patient = find_if(t_patients.begin(), t_patients.end(),
                                                    [patientToSearch](const Patient p)
                                                    { return p.getID() == patientToSearch; });
        if (patient == t_patients.end())
        {
            throw runtime_error("Errore nel file soluzione. Paziente non trovato.");
        }
        if (patient->getCurrentService().getService() != t_patientsInJson[i][SERVICE_FIELD].asString())
        {
            *patient = patient->getPatientAndNextService();
            if (patient->getCurrentService().getService() != t_patientsInJson[i][SERVICE_FIELD].asString())
            {
                throw runtime_error("Errore nel file soluzione. Servizio non associato al paziente.");
            }
        }
        m_nodes.push_back(Node(*patient, t_patientsInJson[i][ARRIVAL_FIELD].asInt()));
        if (m_nodes[m_nodes.size() - 1].getDeparturTime() != t_patientsInJson[i][DEPARTURE_FIELD].asInt())
        {
            throw runtime_error("Errore nel file soluzione. Problema nei tempi.");
        }
    }
    // link to depot
    m_nodes[DEPOT].setArrivalTime(m_nodes[m_nodes.size() - 1].getDeparturTime() +
                                  t_depotDistances[m_nodes[m_nodes.size() - 1].getDistancesIndex()]);

    return m_nodes.size() - 1;
}

string Route::getRouteToString() const
{
    stringstream ss;
    ss << " ROUTE\n";
    for (const Node &node : m_nodes)
    {
        ss << node.toString() << '\n';
    }
    return ss.str();
}

Json::Value Route::getJSONRoute() const
{
    Json::Value route;
    const string ARRIVAL_FIELD("arrival_time");
    const string DEPARTURE_FIELD("departure_time");
    const string PATIENT_FIELD("patient");
    const string SERVICE_FIELD("service");

    // start from the first patient
    for (int i = 1, j = 0; i < m_nodes.size(); ++i, ++j)
    {
        route[j][ARRIVAL_FIELD] = m_nodes[i].getArrivalTime();
        route[j][DEPARTURE_FIELD] = m_nodes[i].getDeparturTime();
        route[j][PATIENT_FIELD] = m_nodes[i].getId();
        route[j][SERVICE_FIELD] = m_nodes[i].getService();
    }

    return route;
}