#include "hcreader.hpp"

using namespace homecare; 


hcreader::hcreader(string t_dataPath) {
    vector<vector<int>> t_distances;
    vector<Patient> t_patients;
    std::ifstream file(t_dataPath);
    if (!file.is_open()) {
        throw std::runtime_error("Errore nell'apertura del file di configurazione del sistema.");
        return;
    }
    string content = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();  // Chiudi il file

    // Parsing del JSON
    Json::Value root;
    Json::Reader reader;

    if (!reader.parse(content, root)) {
        throw std::runtime_error("Errore nell'apertura del file di configurazione del sistema.");
        return;
    }
    if (root.isMember("distances") && root["distances"].isArray()) {
        for (const auto& distances : root["distances"]){
            vector<int> row;
            if (!distances.isArray()) {
                throw std::runtime_error("Errore nell'apertura del file di configurazione del sistema.");
                return;
            }
            for (const auto& distance : distances) {
                row.push_back(distance.asInt());
            }
            t_distances.push_back(row);
        }
    } else {
        throw std::runtime_error("Errore nell'apertura del file di configurazione del sistema.");
        return;
    }

    /*for (vector<int> row : t_distances) {
        for (int dis : row) {
            cout << dis << '\t';
        }
        cout<<'\n';
    }*/

    if (root.isMember("patients") && root["patients"].isArray()) {
        for (const auto& patient : root["patients"]) {
            
        }
    }

}

hcreader::~hcreader() {}


