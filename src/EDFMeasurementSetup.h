#ifndef EDFMEASUREMENTSETUP_H
#define EDFMEASUREMENTSETUP_H
/*
 * Copyright (c) Holger Nahrstaedt
 */

#include <QString>
#include "EDFDate.h"
#include <QStringList>
/*!
 * \brief The EDFMeasurementSetup class
 */
class EDFMeasurementSetup
{
public:
    EDFMeasurementSetup();
    EDFMeasurementSetup(EDFDate, QString, QString, QString, QString, QString);
    //    EDFMeasurementSetup(const EDFMeasurementSetup&);
    virtual ~EDFMeasurementSetup();
    QString toHeaderInfoString() const;
    bool parseHeaderString(QString, bool);
    //    EDFMeasurementSetup& operator=(const EDFMeasurementSetup&);
    void setRecording(QString s) { recording = convertString(s); }
    void setRecordingAdditional(QString s) { recordingAdditional = convertString(s); }
    void setAdminCode(QString s) { adminCode = convertString(s); }
    void setTechnician(QString s) { technician = convertString(s); }
    void setDate(EDFDate d) { date = d; }
    void setEquipment(QString s) { equipment = convertString(s); }

    QString getRecording() const { return recording; }
    QString getRecordingAdditional() const { return recordingAdditional; }
    QString getAdminCode() const { return adminCode; }
    QString getTechnician() const { return technician; }
    QString getEquipment() const { return equipment; }
    EDFDate getDate() const { return date; }

private:
    static QString convertString(QString& str);

    EDFDate date;
    QString adminCode;
    QString technician;
    QString equipment;
    QString recordingAdditional;
    QString recording;  // for EDF and BDF format

};

#endif  // EDFMEASUREMENTSETUP_H
