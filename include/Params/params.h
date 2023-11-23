#include <iostream>
#include <fstream>
#include <sstream>

#ifndef PARAMS_H
#define PARAMS_H

class params
{
private:
    //parametri dell'euristica I1
    double alfa_1;
    double alfa_2;
    double mu;
    double lambda;
    int num_run;
    int settaggio_seme;                          
    //=1 si sceglie il vertice più lontano dal deposito non ancora servito 
    //=2 si sceglie il vertice non ancora servito con la finestra tyemporale più piccola
    int lunghezza_segmento;
    int iterazioni_totali;
    double omega_temperatura;
    double decremento_temperatura;
    double sigma_1;
    double sigma_2;
    double sigma_3;
    double fattore_reazione;
    double fattore_noise;
    double delta;
    int num_soluzioni_ottime;
    double fi;
    double p_related;
    int i_freq;
    int iterazioni_tolleranza_miglioramento;
    int configurazione_programma;

public:
    params(std::string systemConfPath, std::string heuristicsConfPath);
    ~params();
    double getHeuristicsCost(int ca1, int ca2, int cmu, int cl);
};


#endif