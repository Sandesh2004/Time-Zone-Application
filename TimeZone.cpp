#include "TimeZone.h"

TimeZone::TimeZone(){}

TimeZone::TimeZone(std::string k, std::string c, std::string z,
                   double o, bool d, std::string co)
    : key(k), city(c), zoneName(z), offset(o), hasDST(d), country(co) {}