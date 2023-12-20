#ifndef HCSOLUTION_HPP
#define HCSOLUTION_HPP

#include <sstream>

#include "../readjson/hcdata.hpp"

using namespace std;

namespace homecare {

class HCSolution
{
private:
    HCData m_data;
    //vettore di routes
public:
    HCSolution(string);
    ~HCSolution();
    void generateISolution();
};

}

#endif
