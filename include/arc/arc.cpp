#include "arc.hpp"

using namespace homecare;

/**
 * @brief Constructor for the Arc class.
 * 
 * Initializes an Arc object with the specified attributes, representing a directed edge 
 * between two nodes in a graph.
 *
 * @param t_departure Pointer to the departure node of the arc.
 * @param t_arrival Pointer to the arrival node of the arc.
 * @param t_distance The distance or weight associated with the arc.
 * @param t_time The time or cost associated with traversing the arc.
 */
Arc::Arc(Node* t_departure, Node* t_arrival, double t_distance, double t_time)
        : m_arrival (t_arrival), m_departure (t_departure), m_distance (t_distance), 
            m_time (t_time), m_arrival_time (NO_TIME), m_departure_time (NO_TIME)  {}

Arc::~Arc(){}