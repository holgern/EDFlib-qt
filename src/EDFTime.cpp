

#include "EDFTime.h"
#include <iomanip>
#include <cstdlib>

EDFTime::EDFTime()
{
    this->subsecond = 0;
    this->second = 0;
    this->minute = 0;
    this->hour = 0;
}

/**
 Constructor to build new time object. Out of
 range values will be adjusted as appropriate.
 @param hour Integer value between 0 and 59.
 @param min Integer value between 0 and 59.
 @param sec Integer value greater than or equal to 0.
*/
EDFTime::EDFTime(int hour, int min, int sec, long long sub)
{
    // validate sec
    if (sub > 10000000)
    {
        sec += sub / 10000000;
        this->subsecond = sub % 10000000;
    }
    else if (sub < 0)
        this->subsecond = 0;
    else
        this->subsecond = sub;

    if (sec > 59)
    {
        min += sec / 60;
        this->second = sec % 60;
    }
    else if (sec < 0)
        this->second = 0;
    else
        this->second = sec;

    // validate min
    if (min > 59)
    {
        hour += min / 60;
        this->minute = min % 60;
    }
    else if (min < 0)
        this->minute = 0;
    else
        this->minute = min;

    // validate hour
    if (hour < 0)
        this->hour = 0;
    else
        this->hour = hour;
}

/**
 Constructor to build new time object from a positive number
 of seconds.
 @param seconds Integer greater than or equal to 0.
*/
EDFTime::EDFTime(int seconds)
{
    this->hour = seconds / 3600;
    seconds %= 3600;
    this->minute = seconds / 60;
    this->second = seconds % 60;
    this->subsecond = 0;
}

/**
 Constructor to parse time from string of required format
 "hh.mm.ss". Any format errors will cause the time to be
 set to 00.00.00.
 @param time String of aforementioned format.
*/
EDFTime::EDFTime(QTime time)
{
    if (!fromQTime(time))
    {
        this->subsecond = 0;
        this->second = 0;
        this->minute = 0;
        this->hour = 0;
    }
}

// EDFTime::EDFTime(const EDFTime& orig) {
//    hour = orig.hour;
//    minute = orig.minute;
//    second = orig.second;
//    subsecond = orig.subsecond;
//}

/**
 Destructor.
*/
EDFTime::~EDFTime() {}

// EDFTime& EDFTime::operator=(const EDFTime& rhs) {
//    if (this != &rhs) {
//        hour = rhs.hour;
//        minute = rhs.minute;
//        second = rhs.second;
//        subsecond = rhs.subsecond;
//    }

//    return *this;
//}

/**
 Output operator to make a readable string of this object.
 @param s An output stream reference to place the data into.
 @param time The time to stringify.
 @return The output stream.
*/
std::ostream& operator<<(std::ostream& s, EDFTime time)
{
    s << std::setw(2) << std::setfill('0') << time.hour << ".";
    s << std::setw(2) << std::setfill('0') << time.minute << ".";
    s << std::setw(2) << std::setfill('0') << time.second << ".";
    s << time.subsecond;
    return s;
}

/**
 Get the integer hour of this object.
 @return Integer in [0, maxInt].
*/
int EDFTime::getHour() const { return hour; }

/**
 Get the integer minute of this object.
 @return Integer in [0, 59].
*/
int EDFTime::getMinute() const { return minute; }

/**
 Get the integer second of this object.
 @return Integer in [0, 59].
*/
int EDFTime::getSecond() const { return second; }

long long EDFTime::getSubSecond() const { return subsecond; }
/**
 Set the time's hour value. Out of range values
 will default to 0.
 @param day Integer value between 0 and maxInt.
*/
void EDFTime::setHour(int hour)
{
    // validate hour
    if (hour < 0)
        this->hour = 0;
    else
        this->hour = hour;
}

/**
 Set the time's minute value. Out of range values
 will default to 0.
 @param day Integer value between 0 and 59.
*/
void EDFTime::setMinute(int min)
{
    // validate min
    if (min < 0)
        this->minute = 0;
    else if (min > 59)
    {
        this->hour += min / 60;
        this->minute = min % 60;
    }
    else
        this->minute = min;
}

/**
 Set the time's second value. Out of range values
 will default to 0.
 @param day Integer value between 0 and 59.
*/
void EDFTime::setSecond(int sec)
{
    // validate sec
    if (sec < 0)
        this->second = 0;
    else if (sec > 59)
    {
        this->minute += sec / 60;
        this->second = sec % 60;
    }
    else
        this->second = sec;

    if (minute > 59)
    {
        this->hour += this->minute / 60;
        this->minute %= 60;
    }
}

void EDFTime::setSubSecond(long long sub)
{
    // validate sec
    if (sub < 0)
        this->subsecond = 0;
    else if (sub > 10000000)
    {
        this->second += sub / 10000000;
        this->subsecond = sub % 10000000;
    }
    else
        this->subsecond = sub;

    if (second > 59)
    {
        this->minute += this->second / 60;

        this->second %= 60;
    }
}

/**
 Get the value of this time object in seconds.
 @return Integer in [0, maxInt]
*/
int EDFTime::asSeconds() const { return hour * 3600 + minute * 60 + second; }

QTime EDFTime::toQTime() const { return QTime(hour, minute, second, subsecond / 10000); }

bool EDFTime::fromQTime(QTime time)
{
    if (time.isValid())
    {
        this->hour = time.hour();
        this->minute = time.minute();
        this->second = time.second();
        this->subsecond = time.msec() * 10000;
        return true;
    }
    else
        return false;
}

bool EDFTime::parseHeaderString(QString str) { return fromQTime(QTime::fromString(str, "hh.mm.ss")); }
