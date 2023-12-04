#ifndef NODE_HPP
#define NODE_HPP

#include <iostream>
#include <sstream>
#include "../utils/utils.hpp"

namespace homecare{

class Node{
    private:
        static const int NO_VALUE = -1;
        bool m_sync = false;                            //dato di configurazione per la sincronia: bool flag servizio sincrono
        int m_indice_gestore_sync = NO_VALUE;           //dato di configurazione per la sincronia: indice del nodo copiato
        int m_indice_nodo_sync = NO_VALUE;              //dato di configurazione per la sincronia: riferimento al nodo copiato
        int m_id;                                       //posizione in lista e nella matrice delle distanze
        int m_id_city;
        double m_x_coord;                               //coordinata x
        double m_y_coord;                               //coordinata y
        double m_start_window_time;                     //inizio della finestra, dovrebbe essere int ed indicare i minuti ma bho
        double m_end_window_time;                       //fine della finestra, dovrebbe essere int ed indicare i minuti ma bho
        int m_service;                                  //durata servizio in min
        int m_demand;
        bool m_depot;                                   //flag del deposito
        double m_arrival_time;                          //tempo di arrivo al nodo
        double m_departure_time;                        //tempo di partenza dal nodo
        static const int NO_TIME = -1;                  //default di non assegnato

    public:
        Node(int, int, double, double, double, double, int, int, bool);
        ~Node();
        int getID();
        int getCityID();
        double getXCoord();
        double getYCoord();
        double getWindowStartTime();
        double getWindowEndTime();
        int getService();
        int getDemand();
        void setSyncConfig(bool, int);
        void setSyncDuplicateNode(int, int);
        void setArrivalTime(double);
        double getDeparturTime();
        double getArrvalTime();
        int getSyncNode();
        bool isSync();
        bool isInTimeWindow(double);
};

}

#endif