#ifndef EDFSIGNALDATA_H
#define EDFSIGNALDATA_H
/*
 * Copyright (c) Holger Nahrstaedt 
 */

#include <QVector>
#include <cstdlib>
/*!
 * \brief The EDFSignalData class
 */
class EDFSignalData
{
public:
    EDFSignalData();
    // EDFSignalData(double, double, double);
    EDFSignalData(const EDFSignalData&) = default;
    virtual ~EDFSignalData();

    // EDFSignalData& operator=(const EDFSignalData&);

    // void addElement(int16_t);
    // void addElements(vector<int16_t>);
    // void addElements(int16_t*, int);
    void addPhysicalData(double*, int);
    void addPhysicalData(double);
    int size() const { return data.size(); }  // in seconds
    void resize(int n) { data.resize(n); }
    // double getFrequency(); // in hertz
    QVector<double> getData() const { return data; }
    void clear() { data.clear(); }
    // double getMax();
    // double getMin();
    double at(int pos) const { return data.at(pos); }

private:
    double frequency;  // in hertz
    double max;
    double min;
    QVector<double> data;
};

#endif  // EDFSIGNALDATA_H
