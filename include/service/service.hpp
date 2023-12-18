#ifndef SERVICE_HPP
#define SERVICE_HPP

#include <string>
#include <json/json.h>

using namespace std;

namespace homecare {

/** 
 * Class representing a patient service.
 */
class Service
{
private:
    string m_service;
    int m_duration;
public:
    Service(string, int);
    Service(Json::Value, bool = false) noexcept (false);
    ~Service();
    /**
     * Service Getter
    */
    string getService() const { return m_service; }
    /**
     * Duration Getter
    */
    int getDuration() const { return m_duration; }
    string toString() const;
};

}

#endif