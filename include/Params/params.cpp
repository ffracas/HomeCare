#include "params.h"

using namespace std;

/**
 * Constructor for the 'params' class, creating an instance of parameters used by the program. 
 * If it fails return an error and terminates the programm.
 * 
 * @param systemConfPath: A string specifying the path to read constants for the system configuration.
 * @param heuristicsConfPath: A string specifying the path to read constants for the problem configuration.
 * 
*/
params::params(string systemConfPath, string heuristicsConfPath){

    //lettura e salvataggio della configurazione di sistema
    std::ifstream file(systemConfPath);
    if (!file.is_open()) {
        throw std::runtime_error("Errore nell'apertura del file di configurazione del sistema.");
    }

    string line;
    std::getline(file, line);
    num_run = stoi(line);
    std::getline(file, line);
    std::getline(file, line);
    lunghezza_segmento = stoi(line);
    std::getline(file, line);
    std::getline(file, line);
    iterazioni_totali = stoi(line);
    std::getline(file, line);
    std::getline(file, line);
    string p_worst = line;
    std::getline(file, line);
    std::getline(file, line);
    omega_temperatura = stod(line);
    std::getline(file, line);
    std::getline(file, line);
    decremento_temperatura = stod(line);
    std::getline(file, line);
    std::getline(file, line);
    sigma_1 = stod(line);
    std::getline(file, line);
    std::getline(file, line);
    sigma_2 = stod(line);
    std::getline(file, line);
    std::getline(file, line);
    sigma_3 = stod(line);
    std::getline(file, line);
    std::getline(file, line);
    fattore_reazione = stod(line);
    std::getline(file, line);
    std::getline(file, line);
    fattore_noise = stod(line);
    std::getline(file, line);
    std::getline(file, line);
    delta = stod(line);
    std::getline(file, line);
    std::getline(file, line);
    fi = stod(line);
    std::getline(file, line);
    std::getline(file, line);
    p_related = stod(line);
    std::getline(file, line);
    std::getline(file, line);
    iterazioni_tolleranza_miglioramento = stoi(line);
    std::getline(file, line);
    std::getline(file, line);
    num_soluzioni_ottime = stoi(line);
    std::getline(file, line);
    std::getline(file, line);
    i_freq = stoi(line);
    std::getline(file, line);
    std::getline(file, line);
    configurazione_programma = stoi(line);
    
    file.close();
    file.open(heuristicsConfPath);

    if (!file.is_open()) {
        throw std::runtime_error("Errore nell'apertura del file di configurazione dell'euristica.");
    }
    
    std::getline(file, line);
    std::istringstream iss(line);
    

    // Tokenizzazione della riga
    std::string token;
    int i = 0;
    while (iss >> token) {
        if(i == 0)
            alfa_1 = stod(token);
        if(i == 1)
            alfa_2 = std::stod(token);
        if(i == 2)
            mu = std::stod(token);
        if(i == 3)
            lambda =  std::stod(token);
        if(i == 4)
            settaggio_seme = std::stoi(token);
        i++;
    }

}
/**
 * 
*/
params::~params(){}

/** TODO Da completare questa è di prova
 * The getHeuristicCost method takes the problem variables and returns the cost of the solution.
 * 
 * @param ca1 alfa_1 variable
 * @param ca2 alfa_2 variable
 * @param cmu mu variable
 * @param cl  lambda variable
 * @return the cost of the solution [[double]]
*/
double params::getHeuristicsCost(int ca1, int ca2, int cmu, int cl){
    return ca1 * alfa_1 + ca2 * alfa_2 + cmu * mu + cl * lambda;
}

