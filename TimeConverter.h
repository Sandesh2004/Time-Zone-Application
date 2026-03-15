#ifndef TIME_CONVERTER_H
#define TIME_CONVERTER_H

#include <ctime>
#include "TimeZone.h"

class TimeConverter {

public:
    static tm getUTC();
    static tm applyOffset(tm utc, double offsetHours);
    static void show(tm t, const TimeZone& z, bool dst);
};

#endif