#include "infonode.hpp"

using namespace homecare;

// Metodi getter
int InfoNode::getRoute() const { return m_route; }

int InfoNode::getPosition() const { return m_position; }

int InfoNode::getTime() const { return m_time; }

int InfoNode::getPatientIndex() const { return m_patientIndex; }

// Metodi setter
void InfoNode::setInRoute(int route, int position) {
    m_route = route;
    m_position = position;
}

void InfoNode::setTime(int time) { m_time = time; }