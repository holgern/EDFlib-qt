
#include "EDFDate.h"
#include <iomanip>
#include <cstdlib>



EDFDate::EDFDate()
{
    day = 1;
    month = 1;
    year = 1900;
}

/**
 Constructor to build new date object. Out of
 range values will default to 1.
 @param day Integer value between 1 and 31.
 @param month Integer value between 1 and 12.
 @param year Integer value between 0 and 99.
*/

EDFDate::EDFDate(int day, int month, int year)
{
    if (day < 1 || day > 31)
        this->day = 1;
    else
        this->day = day;

    if (month < 1 || month > 12)
        this->month = 1;
    else
        this->month = month;

    if (year < 0 || year > 3000)
        this->year = 1;
    else
        this->year = year;
}

/**
 Constructor to parse date from string of required
 format "dd.mm.yy". Any format errors will cause the date
 to be set to 01.01.1900.
 @param date String of aforementioned format.
*/
EDFDate::EDFDate(QDate date)
{
    if (!fromQDate(date))
    {
        day = 1;
        month = 1;
        year = 1900;
    }
}

// EDFDate::EDFDate(const EDFDate& orig) {
//    day = orig.day;
//    month = orig.month;
//    year = orig.year;
//}

EDFDate::~EDFDate() {}

// EDFDate& EDFDate::operator=(const EDFDate& rhs) {
//    if (this != &rhs) {
//        day = rhs.day;
//        month = rhs.month;
//        year = rhs.year;
//    }

//    return *this;
//}

bool EDFDate::operator==(const EDFDate& rhs) const
{
    return (this->day == rhs.day && this->month == rhs.month && this->year == rhs.year);
}

/**
 Output operator to make a readable string of this object.
 @param s An output stream reference to place the data into.
 @param date The date to stringify.
 @return The output stream.
*/
std::ostream& operator<<(std::ostream& s, EDFDate date)
{
    s << std::setw(2) << std::setfill('0') << date.day << ".";
    s << std::setw(2) << std::setfill('0') << date.month << ".";
    s << date.year;
    return s;
}


/**
 Get the integer day of month value.
 @return Integer in [1, 31].
*/
int EDFDate::getDay() const
{ return day; }

/**
 Get the integer month of year value.
 @return Integer in [1, 12].
*/
int EDFDate::getMonth() const
{ return month; }

/**
 Get the integer year value.
 @return Integer in [0, 3000].
*/
int EDFDate::getYear() const
{ return year; }

/**
 Set the date's day value. Out of range values
 will default to 1.
 @param day Integer value between 1 and 31.
*/
bool EDFDate::setDay(int day)
{
    if (day < 1 || day > 31)
    {
        this->day = 1;
        return false;
    }
    else
    {
        this->day = day;
        return true;
    }
}

/**
 Set the date's month value. Out of range values
 will default to 1.
 @param month Integer value between 1 and 12.
*/
bool EDFDate::setMonth(int month)
{
    if (month < 1 || month > 12)
    {
        this->month = 1;
        return false;
    }
    else
    {
        this->month = month;
        return true;
    }
}

/**
 Set the date's year value. Out of range values
 will default to 1.
 @param year Integer value between 0 and 3000.
*/
bool EDFDate::setYear(int year)
{
    if (year < 1 || year > 3000)
    {
        this->year = 1;
        return false;
    }
    else
    {
        this->year = year;
        return true;
    }
}

QString EDFDate::toString() const
{
    QDate date = QDate(year, month, day);
    QString dateString = date.toString("dd-MM-yyyy");
    QStringList dateStr = dateString.split("-");

    if (dateStr.size() < 3)
    {
        return "";
    }

    QString out;
    switch (dateStr[1].toInt())
    {
        case 1:
            out = dateStr.at(0) + "-JAN-" + dateStr.at(2);
            break;
        case 2:
            out = dateStr.at(0) + "-FEB-" + dateStr.at(2);
            break;
        case 3:
            out = dateStr.at(0) + "-MAR-" + dateStr.at(2);
            break;
        case 4:
            out = dateStr.at(0) + "-APR-" + dateStr.at(2);
            break;
        case 5:
            out = dateStr.at(0) + "-MAY-" + dateStr.at(2);
            break;
        case 6:
            out = dateStr.at(0) + "-JUN-" + dateStr.at(2);
            break;
        case 7:
            out = dateStr.at(0) + "-JUL-" + dateStr.at(2);
            break;
        case 8:
            out = dateStr.at(0) + "-AUG-" + dateStr.at(2);
            break;
        case 9:
            out = dateStr.at(0) + "-SEP-" + dateStr.at(2);
            break;
        case 10:
            out = dateStr.at(0) + "-OCT-" + dateStr.at(2);
            break;
        case 11:
            out = dateStr.at(0) + "-NOV-" + dateStr.at(2);
            break;
        case 12:
            out = dateStr.at(0) + "-DEC-" + dateStr.at(2);
            break;
    }
    if(out.isEmpty())
    {
        return "";
    }
    return out;
}

QDate EDFDate::toQDate() const
{ return QDate(year, month, day); }

bool EDFDate::fromQDate(QDate date)
{
    if (date.isValid())
    {
        this->day = date.day();
        this->month = date.month();
        this->year = date.year();
        return true;
    }
    else
        return false;
}

bool EDFDate::parseHeaderString(QString str)
{
    QStringList dateStr = str.split("-");
    if (dateStr.size() != 3)
        return false;
    if (!setDay(dateStr.at(0).toInt()))
        return false;
    if (!setYear(dateStr.at(2).toInt()))
        return false;
    if (dateStr.at(1).contains("JAN"))
        setMonth(1);
    else if (dateStr.at(1).contains("FEB"))
        setMonth(2);
    else if (dateStr.at(1).contains("MAR"))
        setMonth(3);
    else if (dateStr.at(1).contains("APR"))
        setMonth(4);
    else if (dateStr.at(1).contains("MAY"))
        setMonth(5);
    else if (dateStr.at(1).contains("JUN"))
        setMonth(6);
    else if (dateStr.at(1).contains("JUL"))
        setMonth(7);
    else if (dateStr.at(1).contains("AUG"))
        setMonth(8);
    else if (dateStr.at(1).contains("SEP"))
        setMonth(9);
    else if (dateStr.at(1).contains("OCT"))
        setMonth(10);
    else if (dateStr.at(1).contains("NOV"))
        setMonth(11);
    else if (dateStr.at(1).contains("DEC"))
        setMonth(12);
    else
        return false;

    return true;
}
