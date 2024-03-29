#ifndef PARAMS_HPP
#define PARAMS_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>

namespace vr
{
    
class Params
{
private:
    //parametri dell'euristica I1
    double m_alfa_1;
    double m_alfa_2;
    double m_mu;
    double m_lambda;
    //num massimo di tentativi
    int m_num_run;
    //sceglie vertice (1) più lontano dal deposito (2) con la finestra temporale più piccola
    int m_settaggio_seme;                     
    //parametri di sistema
    int m_lunghezza_segmento;
    int m_iterazioni_totali;
    double m_omega_temperatura;
    double m_decremento_temperatura;
    double m_sigma_1;
    double m_sigma_2;
    double m_sigma_3;
    double m_fattore_reazione;
    double m_fattore_noise;
    double m_delta;
    int m_num_soluzioni_ottime;
    double m_fi;
    double m_p_related;
    int m_i_freq;
    int m_iterazioni_tolleranza_miglioramento;
    int m_configurazione_programma;
    int m_camionCapacity;
    int m_camionNumber;
    //validity check on parameters
    bool controlParameters();

public:
    Params(std::string, std::string) noexcept (false);
    ~Params();
    double getInsertionCost(int , int , int , int );
    int getProgramConfig();
    double mu();
    double alfa1();
    double alfa2();
    double lambda();
    void setCamionLimits(int, int);
    int getCamionCapacity();
    int getCamionNumber();
};
  
} // namespace vr

#endif