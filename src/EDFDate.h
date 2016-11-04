#ifndef EDFDATE_H
#define EDFDATE_H
/*
 * Copyright (c) Holger Nahrstaedt (TU Berlin)
 */

#include <QString>
#include <QStringList>
#include <QDate>
#include <ostream>
/*
* Copyright (c) Holger Nahrstaedt
*/

/*!
 * \brief The EDFDate class
 *
 * EDF Date format
 */
class EDFDate
{
public:
    EDFDate();
    EDFDate(QDate);
    EDFDate(int day, int month, int year);
    EDFDate(const EDFDate&) = default;

    virtual ~EDFDate();

    EDFDate& operator=(const EDFDate&) = default;
    bool operator==(const EDFDate& rhs) const;

    friend std::ostream& operator<<(std::ostream&, EDFDate);
    bool parseHeaderString(QString);
    int getDay() const;
    int getMonth() const;
    int getYear() const;

    bool setDay(int);
    bool setMonth(int);
    bool setYear(int);
    QString toString() const;
    QDate toQDate() const;
    bool fromQDate(QDate date);

private:
    int day, month, year;
};

#endif  // EDFDATE_H
