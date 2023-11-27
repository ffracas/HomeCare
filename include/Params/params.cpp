#include "params.hpp"

using namespace std;
using namespace homecare;

/**
 * @brief Constructor for the 'Params' class
 * 
 * Constructor for the 'params' class, creating an instance of parameters used by the program. 
 * If it fails return an error and terminates the programm.
 * 
 * @param t_systemConfPath: A string specifying the path to read constants for the system configuration.
 * @param t_heuristicsConfPath: A string specifying the path to read constants for the problem configuration.
 * 
*/
Params::Params(string t_systemConfPath, string t_heuristicsConfPath){

    //lettura e salvataggio della configurazione di sistema
    std::ifstream file(t_systemConfPath);
    if (!file.is_open()) {
        throw std::runtime_error("Errore nell'apertura del file di configurazione del sistema.");
    }

    //tra un dato ed un altro nel file originale viene lasciato un rigo vuoto
    string line;
    std::getline(file, line);
    m_num_run = stoi(line);
    std::getline(file, line);
    std::getline(file, line);
    m_lunghezza_segmento = stoi(line);
    std::getline(file, line);
    std::getline(file, line);
    m_iterazioni_totali = stoi(line);
    std::getline(file, line);
    std::getline(file, line);
    string p_worst = line;
    std::getline(file, line);
    std::getline(file, line);
    m_omega_temperatura = stod(line);
    std::getline(file, line);
    std::getline(file, line);
    m_decremento_temperatura = stod(line);
    std::getline(file, line);
    std::getline(file, line);
    m_sigma_1 = stod(line);
    std::getline(file, line);
    std::getline(file, line);
    m_sigma_2 = stod(line);
    std::getline(file, line);
    std::getline(file, line);
    m_sigma_3 = stod(line);
    std::getline(file, line);
    std::getline(file, line);
    m_fattore_reazione = stod(line);
    std::getline(file, line);
    std::getline(file, line);
    m_fattore_noise = stod(line);
    std::getline(file, line);
    std::getline(file, line);
    m_delta = stod(line);
    std::getline(file, line);
    std::getline(file, line);
    m_fi = stod(line);
    std::getline(file, line);
    std::getline(file, line);
    m_p_related = stod(line);
    std::getline(file, line);
    std::getline(file, line);
    m_iterazioni_tolleranza_miglioramento = stoi(line);
    std::getline(file, line);
    std::getline(file, line);
    m_num_soluzioni_ottime = stoi(line);
    std::getline(file, line);
    std::getline(file, line);
    m_i_freq = stoi(line);
    std::getline(file, line);
    std::getline(file, line);
    m_configurazione_programma = stoi(line);
    
    //chiuso il file di sistema si passa a quello per l'euristica
    file.close();
    file.open(t_heuristicsConfPath);

    if (!file.is_open()) {
        throw std::runtime_error("Errore nell'apertura del file di configurazione dell'euristica.");
    }
    
    std::getline(file, line);
    std::istringstream iss(line);
    

    // Tokenizzazione della riga, in questo file i dati sono separati da uno spazio
    std::string token;
    int i = 0;
    while (iss >> token) {
        if(i == 0)
            m_alfa_1 = stod(token);
        if(i == 1)
            m_alfa_2 = std::stod(token);
        if(i == 2)
            m_mu = std::stod(token);
        if(i == 3)
            m_lambda =  std::stod(token);
        if(i == 4)
            m_settaggio_seme = std::stoi(token);
        i++;
    }

    controlParameters();
}

Params::~Params(){}

/** TODO Da completare questa è di prova
 * The getInsertCost method takes the problem variables and returns the cost of the solution. (I1)
 * 
 * @param ca1 alfa_1 variable
 * @param ca2 alfa_2 variable
 * @param cmu mu variable
 * @param cl  lambda variable
 * @return the cost of the solution [[double]]
*/
double Params::getInsertionCost(int distance_cost, int time_difference, int cmu, int cl){
    return  m_alfa_1 +  m_alfa_2 + cmu * m_mu + cl * m_lambda;
}

/**
 * @brief Checks the validity of the parameters.
 * 
 * This method verifies whether the parameters meet specific criteria.
 * 
 * @return True if the parameters are valid, otherwise throws a runtime_error with an error message.
 * 
 * Criteria:
 * - m_alfa_1 and m_alfa_2 must be non-negative.
 * - The sum of m_alfa_1 and m_alfa_2 must equal 1.
 * - m_mu and m_lambda must be non-negative.
 * 
 * @throw std::runtime_error if the parameters do not meet the specified criteria.
 */
bool Params::controlParameters(){
    if(m_alfa_1 < 0 || m_alfa_2 < 0 || m_alfa_1 + m_alfa_2 != 1 || m_mu < 0 || m_lambda < 0)
        throw std::runtime_error("Errore! Parametri non conformi.");
    return true;
}

/**
 * @brief get the value of the field configurazione_programma.
 * 
 * Get method for the field configurazione_programma.
 * 
 * @return the configurazione_programma's integer value.
*/
int Params::getProgramConfig(){
    return m_configurazione_programma;
}
