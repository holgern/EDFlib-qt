#ifndef EDFANNOTATION_H
#define EDFANNOTATION_H
/*
 * Copyright (c) Holger Nahrstaedt
 */

#include <QString>
#include <QVector>
#include <iostream>
/*!
 * \brief The EDFAnnotation class
 */
class EDFAnnotation
{
public:
    EDFAnnotation();
    EDFAnnotation(long long, long long, QString);
    EDFAnnotation(double, double, QString);
    EDFAnnotation(long long, long long, QVector<QString>);
    EDFAnnotation(double, double, QVector<QString>);

    EDFAnnotation(const EDFAnnotation&) = default;
    virtual ~EDFAnnotation();

    EDFAnnotation& operator=(const EDFAnnotation&) = default;
    friend std::ostream& operator<<(std::ostream&, EDFAnnotation&);

    long long getOnsetInEDFTime() const;
    long long getOnsetIn100USec() const;
    double getOnsetInSec() const;
    long long getDurationInEDFTime() const;
    long long getDurationIn100USec() const;
    double getDurationInSec() const;

    QString getText(int index = 0) const;
    QVector<QString> getList() const;

    void setOnsetInEDFTime(long long);
    void setOnsetInSec(double);
    void setDurationInEDFTime(long long);
    void setDurationInSec(double);
    void setText(const QString& ann);
    void setList(const QVector<QString>& annList);

private:
    long long onset;
    long long duration;
    QVector<QString> annotations;
    double TIME_RESOLUTION = 0.0001;
    long long EDFLIB_TIME_DIMENSION = 10000000LL;
};

#endif  // EDFANNOTATION_H
