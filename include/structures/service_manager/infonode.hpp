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
    static const int NOT_ASSIGNED;
    InfoNode(int, int, int, int);
    InfoNode();
    ~InfoNode();
    // Metodi getter
    int getRoute() const;
    int getPositionInRoute() const;
    int getTime() const;
    int getPatientIndex() const;
    // Metodi setter
    void setInRoute(int, int, int);
    void setTime(int);
    void destroy();
    // checker
    bool isAssigned();
};
    
}

#endif 