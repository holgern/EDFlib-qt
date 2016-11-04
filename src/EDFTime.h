#ifndef EDFTIME_H
#define EDFTIME_H
/*
 * Copyright (c) Holger Nahrstaedt
 */

#include <QString>
#include <QTime>
#include <ostream>
/*!
 * \brief The EDFTime class
 *
 *  EDF Time format
 */
class EDFTime
{
public:
    EDFTime();
    EDFTime(QTime);
    EDFTime(int);
    EDFTime(int, int, int, long long);
    EDFTime(const EDFTime&) = default;
    virtual ~EDFTime();

    EDFTime& operator=(const EDFTime&) = default;
    friend std::ostream& operator<<(std::ostream&, EDFTime);
    bool parseHeaderString(QString);
    int getHour() const;
    int getMinute() const;
    int getSecond() const;
    long long getSubSecond() const;

    void setHour(int);
    void setMinute(int);
    void setSecond(int);
    void setSubSecond(long long);

    int asSeconds() const;
    QTime toQTime() const;
    bool fromQTime(QTime time);

private:
    int hour;
    int minute;
    int second;
    /*  offset expressed in units of 100 nanoSeconds. Is always less than 10000000 (one second). Only used by EDFplus
     * and BDFplus */
    long long subsecond;
};

#endif  // EDFTIME_H
