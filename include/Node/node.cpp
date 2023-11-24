#include "node.hpp"

using namespace homecare;

int Node::getCityID(){
    return m_id_city;
}
    
double Node::getXCoord(){
    return m_x_coord;
}
    
double Node::getYCoord(){
    return m_y_coord;
}

double Node::getWindowStartTime(){
    return m_window_start_time;
}

double Node::getWindowEndTime(){
    return m_window_end_time;
}

int Node::getService(){
    return m_service;
}