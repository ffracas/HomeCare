#ifndef PARAMS_HPP
#define PARAMS_HPP

#include <iostream>
#include <fstream>
#include <sstream>

namespace homecare
{
    
class Params
{
private:
    //parametri dell'euristica I1
    double m_alfa_1;
    double m_alfa_2;
    double m_mu;
    double m_lambda;
    int m_num_run;
    int m_settaggio_seme;          //sceglie vertice (1)più lontano dal deposito (2)con la finestra temporale più piccola              
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
    
    bool controlParameters();

public:
    Params(std::string, std::string);
    ~Params();
    double getInsertionCost(int , int , int , int );
};
  
} // namespace homecare

#endif