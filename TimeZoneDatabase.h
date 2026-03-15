#ifndef TIMEZONE_DATABASE_H
#define TIMEZONE_DATABASE_H

#include <map>
#include <vector>
#include <string>
#include "TimeZone.h"

class TimeZoneDatabase {

private:
    std::map<std::string, TimeZone> zones;
    TimeZoneDatabase();

public:
    static TimeZoneDatabase& getInstance();

    bool hasCity(const std::string& city) const;
    TimeZone getZone(const std::string& city) const;

    void listCities() const;
    std::vector<std::string> search(const std::string& q) const;
};

#endif