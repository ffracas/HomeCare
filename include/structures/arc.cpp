#include "arc.hpp"

using namespace homecare;

/**
 * @brief Constructor for the Arc class.
 * 
 * Initializes an Arc object with specified arrival and departure nodes and the distance between them.
 * 
 * @param t_departure The departure node of the arc.
 * @param t_arrival The arrival node of the arc.
 * @param t_departureTime The time in which the arc departs from the departure node
 * @param t_arrivalTime The time in which the arc arrives to the arrival node
 * @param t_distance The distance between the arrival and departure nodes.
 * 
 * The method sets the arrival and departure nodes along with the distance between them.
 */
Arc::Arc(Patient t_departure, Patient t_arrival, int t_departureTime, int t_arrivalTime, int t_distance) 
        : m_departure(t_departure), m_arrival(t_arrival), m_distance(t_distance), m_departureTime(t_departureTime) {
    m_arrivalTime = t_arrivalTime >= t_arrival.getWindowStartTime() ? t_arrivalTime : t_arrival.getWindowStartTime();
    m_readyTime = m_arrivalTime + m_arrival.getServiceTime();
}

Arc::~Arc() {}

Patient Arc::getDeparture() const { return m_departure; }

Patient Arc::getArrival() const { return m_arrival; }

int Arc::getDistance() const { return m_distance; }

int Arc::getDeparturTime() const {return m_departureTime; }

int Arc::getArrvalTime() const { return m_arrivalTime; }

int Arc::getReadyTime() const { return m_readyTime; }