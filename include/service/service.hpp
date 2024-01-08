#ifndef SERVICE_HPP
#define SERVICE_HPP

#include <string>
#include <json/json.h>

namespace homecare {

/** 
 * Class representing a patient service.
 */
class Service
{
private:
    std::string m_service;
    int m_duration;
public:
    Service(std::string, int);
    Service(Json::Value, bool = false) noexcept (false);
    ~Service();
    /**
     * Service Getter
    */
    std::string getService() const { return m_service; }
    /**
     * Duration Getter
    */
    int getDuration() const { return m_duration; }
    std::string toString() const;
};

}

#endif