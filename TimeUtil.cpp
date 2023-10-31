#include "pch.h"
#include "TimeUtil.h"

int CTimeUtil::GetYear(time_t ts)
{
    // Convert the timestamp to a struct tm representing UTC time
    struct tm timeinfo;
    gmtime_s(&timeinfo, &ts);

    // Extract the year from the struct tm
    int year = timeinfo.tm_year + 1900;
    return year;
}