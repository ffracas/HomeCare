#include "arc.hpp"

using namespace homecare;

/**
 * @brief Constructor for the Arc class.
 * 
 * Initializes an Arc object with specified arrival and departure nodes and the distance between them.
 * 
 * @param t_arrival The arrival node of the arc.
 * @param t_departure The departure node of the arc.
 * @param t_distance The distance between the arrival and departure nodes.
 * 
 * The method sets the arrival and departure nodes along with the distance between them.
 */
Arc::Arc(Node* t_departure, Node* t_arrival, double t_distance)
        : m_departure (t_departure), m_arrival (t_arrival), m_distance(t_distance) {}

Arc::~Arc() {}

Node Arc::getDeparture() { return *m_departure; }

Node Arc::getArrival() { return *m_arrival; }

Node* Arc::getArrivalRef() { return m_arrival; }

Node* Arc::getDepartureRef() { return m_departure; }

double Arc::getDistance() { return m_distance;}

void Arc::setArrivalTime(double t_time) { m_arrival->setArrivalTime(t_time); }