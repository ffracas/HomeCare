#include "problemdef.hpp"

using namespace std;
using namespace homecare;

/**
 * @brief Constructor for initializing ProblemDef object with data from three different files.
 * 
 * This constructor reads data from three distinct files to initialize the fields of the ProblemDef class.
 * The files include information related to the problem definition, system configuration, and heuristics configuration.
 * 
 * @param t_pathDataFile File path for reading problem definition data.
 * @param t_systemConfPath File path for reading system configuration data.
 * @param t_heuristicsConfPath File path for reading heuristics configuration data.
 * 
 * The method initializes the system configuration and heuristics configuration parameters using the Params class.
 * The Params constructor is responsible for reading data from the system and heuristics configuration files.
 * 
 * @throw std::runtime_error if any errors occur during data reading.
 */
ProblemDef::ProblemDef(string t_pathDataFile, 
                    string t_systemConfPath, string t_heuristicsConfPath) 
        : m_params(t_systemConfPath, t_heuristicsConfPath) {
    m_depotIndex = 0;
    std::ifstream file(t_pathDataFile);
    if (!file.is_open()) {
        throw std::runtime_error("Errore nell'apertura del file dei dati del sistema.");
    }
    int const emptyLines = 4;
    std::string line;
    // Skipping the empty four lines  
    //TODO class file che implementa il salto riga un readAfter(file, righe da saltare, line)
    for (int i = 0; i < emptyLines; ++i) {
        std::getline(file, line);
    }

    // Reading the next line
    std::getline(file, line);

    // Using regex to replace multiple spaces with a single space
    std::regex spaces("\\s{2,}");
    std::regex tab("\\t");
    std::string cleaned_line = std::regex_replace(std::regex_replace(line, tab, " "), spaces, " ");

    // Extract information about camion
    std::istringstream iss(cleaned_line);
    std::string n_camion, camion_cap;
    iss >> n_camion >> camion_cap;
    m_params.setCamionLimits(std::stoi(camion_cap), std::stoi(n_camion));
    // Skipping the empty four lines
    for (int i = 0; i < emptyLines; ++i) {
        std::getline(file, line);
    }

    int i = 0;
    while(std::getline(file, line)){
        // Tokenizza la riga usando spazi come delimitatori
        line = std::regex_replace(line, tab, " ");
        std::istringstream iss(line);
        std::vector<std::string> info_nodo{
            std::istream_iterator<std::string>(iss),
            std::istream_iterator<std::string>()
        };

        Node toInsert(i, stoi(info_nodo[0]), stod(info_nodo[1]), stod(info_nodo[2]),
                    stod(info_nodo[4]), stod(info_nodo[5]), stoi(info_nodo[6]), stoi(info_nodo[3]), i == m_depotIndex);

        m_nodes.push_back(toInsert);
        ++i;  
    }

    file.close(); 

    //for add synchronous nodes in synchronous setting
    if(m_params.getProgramConfig() != 1){
        //add synchronized node
        int nodeSyncNumber = (m_nodes.size() - 1) / 10;
        for(i = 0; i < nodeSyncNumber; ++i) {
            m_nodes[nodeSyncNumber * (i + 1)].setSync();
        }
    }

    //calculation of distances
    //initialise distances matrix to 0
    int nodeNumber = m_nodes.size();
    m_distances = std::vector<std::vector<double>>(nodeNumber, std::vector<double>(nodeNumber, 0.0));
    //set value in matrix
    for(i = 0; i < nodeNumber - 1; ++i){
        for(int j = i + 1; j < nodeNumber; j++){
            double distance = sqrt(pow(m_nodes[i].getXCoord() - m_nodes[j].getXCoord(), 2) + 
                                        pow(m_nodes[i].getYCoord() - m_nodes[j].getYCoord(), 2));
            m_distances[i][j] = distance;
            m_distances[j][i] = distance;
        }
    }

    for(int i = 0; i < m_params.getCamionNumber(); ++i) {               //initialisation of the route
        m_solution.push_back(Route(m_nodes[m_depotIndex], m_params.getCamionCapacity()));
    }
}

ProblemDef::~ProblemDef() {}

void ProblemDef::generateFirstSolution() {
    if(m_nodes.size() < 2) {                                            //check for min number of node to proceed
        throw std::runtime_error("Errore nella definizione del problema");
    }
    //check for min number of routes
    if(m_params.getCamionNumber() < 1) {                                
        throw std::runtime_error("Errore nella definizione del problema");
    }
    //set the group of the node of the path
    vector<Node> nodesToServe(m_nodes.begin() + 1, m_nodes.end());      //depot is the first one
    //ordering nodes to serve by greatest end window
    sort(nodesToServe.begin(), nodesToServe.end(), 
            [] (Node n1, Node n2) { return n1.getWindowEndTime() < n2.getWindowEndTime(); });

    //generate first solution
    while(nodesToServe.size() > 0) {
        try{
            int bestRoute = searchForRoute(nodesToServe[0]);
            double time = calculateArrivalTime(bestRoute, nodesToServe[0]);
            if (nodesToServe[0].isSync()) {
                int bestSync = searchForRoute(nodesToServe[0], bestRoute);
                double syncTime = calculateArrivalTime(bestSync, nodesToServe[0]);
                time = time >= syncTime ? time : syncTime;
                m_solution[bestSync].addNode(nodesToServe[0].getSyncNode(), m_distances, time);
            }
            m_solution[bestRoute].addNode(nodesToServe[0], m_distances, time);
            
            nodesToServe.erase(nodesToServe.begin());
        } catch (const std::exception& e) {
            std::cerr << "Errore: " << e.what() << '\n';
            return;
        }
        
    }
    
    /*for(Route route : m_solution) {
        cout << route.getRouteToString() << '\n';
    }*/
}

int ProblemDef::searchForRoute(Node t_node, int t_syncRoute) noexcept(false) {
    int bestRoute = NO_INDEX;
    double bestCost = MAX_DOUBLE;
    for (int i = 0; i < m_solution.size(); ++i) {
        if (i != t_syncRoute && m_solution[i].isSuitableFor(t_node)) {
            double cost = 3 * m_solution[i].getFreeTime() + m_distances[m_solution[i].getLastClient()][t_node.getID()];
            if(cost < bestCost) {
                bestCost = cost;
                bestRoute = i;
            }
        }
    }
    if (bestRoute == NO_INDEX) {
        throw std::runtime_error("Problema nella composizione della I soluzione");
    }
    return bestRoute;
}

double ProblemDef::calculateArrivalTime(int route, Node node) {
    double serviceTimePrevision = m_solution[route].getFreeTime() + 
                                    m_distances[m_solution[route].getLastClient()][node.getID()];
    return node.getWindowStartTime() > serviceTimePrevision ? node.getWindowStartTime() : serviceTimePrevision;
}