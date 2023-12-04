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
Arc::Arc(Node t_arrival, Node t_departure, double t_distance)
        : m_arrival (&t_arrival), m_departure (&t_departure), m_distance(t_distance) {}

Arc::~Arc() {}

Node Arc::getDeparture() { return *m_departure; }

Node Arc::getArrival() { return *m_arrival; }

double Arc::getDistance() { return m_distance;}