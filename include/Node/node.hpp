#ifndef NODE_HPP
#define NODE_HPP

#include <iostream>
#include <sstream>

namespace homecare{

class Node{
    private:
        static const int NO_VALUE = -1;
        bool m_sync = false;                            //dato di configurazione per la sincronia: bool flag servizio sincrono
        int m_indice_gestore_sync = NO_VALUE;           //dato di configurazione per la sincronia: indice del nodo copiato
        int m_indice_nodo_sync = NO_VALUE;              //dato di configurazione per la sincronia: riferimento al nodo copiato
        int m_id;
        int m_id_city;
        double m_x_coord;                               //coordinata x
        double m_y_coord;                               //coordinata y
        double m_window_start_time;                     //inizio della finestra, dovrebbe essere int ed indicare i minuti ma bho
        double m_window_end_time;                       //fine della finestra, dovrebbe essere int ed indicare i minuti ma bho
        int m_service;                                  //durata servizio in min
        int m_demand;
        bool m_depot;

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
};

}

#endif