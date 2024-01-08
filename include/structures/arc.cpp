#include "arc.hpp"

using namespace std;
using namespace homecare;

/**
 * @brief Constructor for the Arc class.
 * 
 * Initializes an Arc object with specified arrival and departure nodes and the distance between them.
 * 
 * @param t_departure The departure node of the arc.
 * @param t_arrival The arrival node of the arc.
 * @param t_distance The distance between the arrival and departure nodes.
 * @param t_departureTime The time in which the arc departs from the departure node
 * @param t_arrivalTime The time in which the arc arrives to the arrival node
 * 
 * The method sets the arrival and departure nodes along with the distance between them.
 */
Arc::Arc(Node t_departure, Node t_arrival, int t_distance, int t_departure_time, int t_arrivalTime)
            : m_departure (t_departure), m_arrival (t_arrival), m_departureTime (t_departure_time), 
            m_distance(t_distance) {
    m_arrivalTime = t_arrivalTime >= t_arrival.getTimeWindowOpen() ? t_arrivalTime : t_arrival.getTimeWindowOpen();
    m_readyTime = m_arrivalTime + m_arrival.getServiceTime();
}

Arc::~Arc() {}

Node Arc::getDeparture() const { return m_departure; }

Node Arc::getArrival() const { return m_arrival; }

int Arc::getDistance() const { return m_distance; }

int Arc::getDeparturTime() const {return m_departureTime; }

int Arc::getArrvalTime() const { return m_arrivalTime; }

int Arc::getReadyTime() const { return m_readyTime; }

string Arc::toString() const { 
    stringstream ss;

    ss << "parteza: " << m_departure.toString() << ' ';
    ss << "at time: " << m_departureTime << ' ';
    ss << "arrivo: " << m_arrival.toString() << ' ';
    ss << "at time: " << m_arrivalTime << ' ';

    return ss.str();
}

Json::Value Arc::getJSONArc() const {
    const string DEPARTURE_FIELD        ("daprture");
    const string DEPARTURE_TIME_FIELD   ("daprture_time");
    const string ARRIVAL_FIELD          ("arrival");
    const string ARRIVAL_TIME_FIELD     ("arrival_time");
    const string ARRIVAL_SERVICE_FIELD  ("service_info");
    const string ARR_SERV_TYPE_FIELD    ("service");
    const string ARR_SERV_TIME_FIELD    ("serve_time");
    const string DISTANCE_FIELD         ("distance_time");
    Json::Value arc;

    arc[DEPARTURE_FIELD      ] = Json::Value(m_departure.getId());
    arc[DEPARTURE_TIME_FIELD ] = Json::Value(m_departureTime);
    arc[ARRIVAL_FIELD        ] = Json::Value(m_arrival.getId());
    arc[ARRIVAL_TIME_FIELD   ] = Json::Value(m_arrivalTime);
    arc[ARRIVAL_SERVICE_FIELD][ARR_SERV_TYPE_FIELD] = Json::Value(m_arrival.getService());
    arc[ARRIVAL_SERVICE_FIELD][ARR_SERV_TIME_FIELD] = Json::Value(m_arrival.getServiceTime());
    arc[DISTANCE_FIELD       ] = Json::Value(m_distance);

    return arc;
}