#ifndef TIMEZONE_H
#define TIMEZONE_H

#include <string>

class TimeZone {

public:
    std::string key;
    std::string city;
    std::string zoneName;
    double offset;
    bool hasDST;
    std::string country;

    TimeZone();
    TimeZone(std::string k, std::string c, std::string z,
             double o, bool d, std::string co);
};

#endif