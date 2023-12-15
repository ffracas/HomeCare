#ifndef HCREADER_HPP
#define HCREADER_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <json/json.h> 

#include "../patient/patient.hpp"


using namespace std;
namespace homecare {

class hcreader
{
private:
    /* data */
public:
    hcreader(string);
    ~hcreader();
};

}

#endif