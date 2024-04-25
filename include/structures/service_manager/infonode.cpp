#include "infonode.hpp"

using namespace homecare;

const int InfoNode::NOT_ASSIGNED = -1;

/**
 * @brief Constructor for InfoNode object.
 * 
 * @param t_route The route ID.
 * @param t_pos The position in the route.
 * @param t_time The time associated with the node.
 * @param t_index The index of the patient associated with the node.
 */
InfoNode::InfoNode(int t_route, int t_pos, int t_time, int t_index)
    : m_route(t_route), m_position(t_pos), m_time(t_time), m_patientIndex(t_index) {}

/**
 * @brief Constructor for InfoNode object.
 */ 
InfoNode::InfoNode()
    : m_route(NOT_ASSIGNED), m_position(NOT_ASSIGNED), m_time(NOT_ASSIGNED), m_patientIndex(NOT_ASSIGNED) {}

/**
 * @brief Destructor for InfoNode object.
 */
InfoNode::~InfoNode() {}

/**
 * @brief Getter method to retrieve the route ID.
 * 
 * @return The route ID.
 */
int InfoNode::getRoute() const { return m_route; }

/**
 * @brief Getter method to retrieve the position in the route.
 * 
 * @return The position in the route.
 */
int InfoNode::getPositionInRoute() const { return m_position; }

/**
 * @brief Getter method to retrieve the time associated with the node.
 * 
 * @return The time associated with the node.
 */
int InfoNode::getTime() const { return m_time; }

/**
 * @brief Getter method to retrieve the index of the patient associated with the node.
 * 
 * @return The index of the patient associated with the node.
 */
int InfoNode::getPatientIndex() const { return m_patientIndex; }

/**
 * @brief Setter method to set route, position, and time in the route.
 * 
 * @param route The route ID to set.
 * @param position The position in the route to set.
 * @param time The time associated with the node to set.
 */
void InfoNode::setInRoute(int route, int position, int time) {
    if (route < 0 || position < 0 || time < 0) { return; }
    m_route = route;
    m_position = position;
    m_time = time;
}

/**
 * @brief Setter method to set the time associated with the node.
 * 
 * @param time The time associated with the node to set.
 */
void InfoNode::setTime(int time) { m_time = time; }

/**
 * @brief Method to destroy/reset the InfoNode object.
 * 
 * Resets route, position, and time to NOT_ASSIGNED constant.
 */
void InfoNode::destroy() {
    m_route = NOT_ASSIGNED;
    m_position = NOT_ASSIGNED;
    m_time = NOT_ASSIGNED;
}

/**
 * @brief Method that informs whether the node is assigned or not.
 * 
 * @return true if the node is assigned else false
*/
bool InfoNode::isAssigned() { return m_route != NOT_ASSIGNED && m_position != NOT_ASSIGNED && m_time != NOT_ASSIGNED; }