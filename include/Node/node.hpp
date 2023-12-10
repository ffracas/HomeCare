#ifndef NODE_HPP
#define NODE_HPP

#include <iostream>
#include <sstream>
#include "../utils/utils.hpp"

namespace homecare{

class Node{
    private:
        int m_id;                                       //posizione in lista e nella matrice delle distanze
        int m_id_client;
        double m_x_coord;                               //coordinata x
        double m_y_coord;                               //coordinata y
        double m_start_window_time;                     //inizio della finestra, dovrebbe essere int ed indicare i minuti ma bho
        double m_end_window_time;                       //fine della finestra, dovrebbe essere int ed indicare i minuti ma bho
        int m_service;                                  //durata servizio in min
        int m_demand;                                   //richiesta cliente
        bool m_depot;                                   //flag del deposito
        bool m_sync;                                    //dato di configurazione per la sincronia: bool flag servizio sincrono

    public:
        Node(int, int, double, double, double, double, int, int, bool);
        ~Node();
        //setter
        void setSync();
        void resetReq();
        //getter
        int getID();
        int getCityID();
        double getXCoord();
        double getYCoord();
        double getWindowStartTime();
        double getWindowEndTime();
        int getService();
        int getDemand();
        Node getSyncNode();
        //checker
        bool isDepot();
        bool isSync();
        bool isInTimeWindow(double);
};

}

#endif