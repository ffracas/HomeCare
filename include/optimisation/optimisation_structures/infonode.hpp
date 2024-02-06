#ifndef INFONODE_HPP
#define INFONODE_HPP

namespace homecare {

class InfoNode {
private:
    int m_route;
    int m_position;
    int m_time;
    int m_patientIndex;

public:
    InfoNode(int t_route, int t_pos, int t_time, int t_index)
        : m_route(t_route), m_position(t_pos), m_time(t_time), m_patientIndex(t_index) {}
    // Metodi getter
    int getRoute() const;
    int getPosition() const;
    int getTime() const;
    int getPatientIndex() const;

    // Metodi setter
    void setInRoute(int, int);
    void setTime(int);
};
    
}

#endif 