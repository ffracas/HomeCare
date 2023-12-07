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
    // Skipping the empty four lines  //TODO class file che implementa il salto riga un readAfter(file, righe da saltare, line)
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
    m_params.setCamionLimits(std::stoi(n_camion), std::stoi(camion_cap));
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
        int nodeNumber = m_nodes.size();
        int nodeSyncNumber = (nodeNumber - 1) / 10;
        for(i = 0; i < nodeSyncNumber; ++i){
            int nodoInSyncIndex = nodeSyncNumber * (i + 1);
            m_nodes[nodoInSyncIndex].setSyncConfig(true, i);
            
            //create synchronized node
            Node duplicate(m_nodes[nodoInSyncIndex]);
            duplicate.setSyncDuplicateNode(nodeNumber + i, nodoInSyncIndex);

            m_nodes.push_back(duplicate);
        }
    }

    //calculation of distances
    //initialise distances matrix to 0
    int nodeNumber = m_nodes.size();
    m_distances = new double*[nodeNumber];
    for(i = 0; i < nodeNumber; ++i){
        m_distances[i] = new double[nodeNumber]();
    }
    //set value in matrix
    for(i = 0; i < nodeNumber - 1; ++i){
        for(int j = i + 1; j < nodeNumber; j++){
            double distance = sqrt(pow(m_nodes[i].getXCoord() - m_nodes[j].getXCoord(), 2) + 
                                        pow(m_nodes[i].getYCoord() - m_nodes[j].getYCoord(), 2));
            m_distances[i][j] = distance;
            m_distances[j][i] = distance;
        }
    }
}

ProblemDef::~ProblemDef(){}

void ProblemDef::generateFirstSolution(){
    if(m_nodes.size() < 2) {                                            //check for min number of node to proceed
        throw std::runtime_error("Errore nella definizione del problema");
    }
    vector<Node> nodesToServe(m_nodes.begin() + 1, m_nodes.end());  //depot is the first one
    while(nodesToServe.size() > 0){
        Route route(m_nodes[m_depotIndex]);
        int nodeIndex = searchForSeed(nodesToServe);
        Node seed(nodesToServe[nodeIndex]);
        route.startRoute(seed, m_distances);  
        nodesToServe.erase(nodesToServe.begin() + nodeIndex);           //delete served node
        //search for new node to insert in the root
        nodeIndex = -1;
        bool routeCompleted = false;
        while(!routeCompleted){
            int nextNodeIndex = route.searchForNextNode(nodesToServe, m_distances, m_params);
            if(nextNodeIndex != NO_INDEX_NODE){
                nodesToServe.erase(nodesToServe.begin() + nextNodeIndex);           //delete served node
            }
            else {
                routeCompleted = true;
            }
        }
        m_solution.push_back(route); //forse sei il problema
    }

    for(Route route : m_solution) {
        cout << route.getRouteToString() << '\n';
    }
}

int ProblemDef::searchForSeed(vector<Node> t_nodes){   //di default Elena usa questa
    if(t_nodes.size() < 1) return NO_INDEX_NODE;
    if(t_nodes.size() == 1) return 0;
    int seedIndex = NO_INDEX_NODE;
    double distance = 0;
    for(int i = 0; i < t_nodes.size(); ++i){
        if(m_distances[0][i] <= t_nodes[i].getWindowEndTime()){
            if(m_distances[0][i] > distance){
                seedIndex = i;
                distance = m_distances[0][i];
            }
        }
    }
    return seedIndex;
}

