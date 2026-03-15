#include "TimeConverter.h"
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

tm TimeConverter::getUTC(){

    time_t now = time(0);

    tm* utc = gmtime(&now);

    return *utc;
}

tm TimeConverter::applyOffset(tm utc, double offsetHours){

    time_t raw;

#ifdef _WIN32
    raw = _mkgmtime(&utc);
#else
    raw = timegm(&utc);
#endif

    raw += static_cast<int>(round(offsetHours * 3600));

    tm* result = gmtime(&raw);

    return *result;
}

void TimeConverter::show(tm t, const TimeZone& z, bool dst){

    cout << "\n====================================\n";

    cout << "City: " << z.city << " (" << z.country << ")\n";

    cout << "Zone: " << z.zoneName << "\n";

    cout << "Date: " << put_time(&t, "%d-%m-%Y") << "\n";

    cout << "Time: " << put_time(&t, "%H:%M:%S") << "\n";

    cout << "DST: " << (dst ? "Applied" : "Not Applied") << "\n";

    cout << "====================================\n";
}