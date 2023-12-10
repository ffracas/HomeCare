#include "arc.hpp"

using namespace homecare;

/**
 * @brief Constructor for the Arc class.
 * 
 * Initializes an Arc object with specified arrival and departure nodes and the distance between them.
 * 
 * @param t_departure The departure node of the arc.
 * @param t_arrival The arrival node of the arc.
 * @param t_distance The distance between the arrival and departure nodes.
 * @param t_departure_time The time in which the arc departs from the departure node
 * @param t_arrival_time The time in which the arc arrives to the arrival node
 * 
 * The method sets the arrival and departure nodes along with the distance between them.
 */
Arc::Arc(Node t_departure, Node t_arrival, double t_distance, double t_departure_time, double t_arrival_time)
            : m_departure (t_departure), m_arrival (t_arrival), m_distance(t_distance),
            m_departure_time (t_departure_time) {
    m_arrival_time = t_arrival_time >= t_arrival.getWindowStartTime() ? t_arrival_time : t_arrival.getWindowStartTime();
    m_ready_time = m_arrival_time + m_arrival.getService();
}

Arc::~Arc() {}

Node Arc::getDeparture() { return m_departure; }

Node Arc::getArrival() { return m_arrival; }

double Arc::getDistance() { return m_distance;}

double Arc::getDeparturTime() { return m_departure_time; }

double Arc::getArrvalTime() { return m_arrival_time; }

double Arc::getReadyTime() { return m_ready_time; }