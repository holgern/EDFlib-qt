#ifndef EDFPATIENT_H
#define EDFPATIENT_H
/*
 * Copyright (c) Holger Nahrstaedt
 */

#include <QString>
#include <QStringList>
#include <ostream>
#include "EDFDate.h"
#include <QRegExp>
/*!
 * \brief The EDFPatient class
 * Originally, EDF has two headerfields each with 80 Bytes  reserved for the patientname and the recording info.
 *
 * EDF+ and BDF+ have divided these fields into patientname 	patientcode 	birthdate 	patient_additional
 */
class EDFPatient
{
public:
    enum Gender
    {
        Unknown,
        Male,
        Female
    };

    EDFPatient();
    EDFPatient(QString newId, QString newCode, QString newName, QString newAdditional, EDFPatient::Gender newGender,
               EDFDate newBirthdate);
    EDFPatient(const EDFPatient&) = default;
    virtual ~EDFPatient() = default;

    EDFPatient& operator=(const EDFPatient&) = default;
    friend std::ostream& operator<<(std::ostream&, EDFPatient);

    bool parseHeaderString(const QString&, bool);

    QString getId() const;
    QString getCode() const;
    QString getName() const;
    QString getAdditional() const;
    Gender getGender() const;
    EDFDate getBirthdate() const;

    QString toHeaderInfoString() const;

    void setId(const QString&);
    void setCode(const QString&);
    void setName(const QString&);
    void setAdditional(const QString&);
    void setGender(const EDFPatient::Gender&);
    void setBirthdate(const EDFDate&);

private:
    static QString convertString(const QString& str);
    QString id;
    QString code;
    QString name;
    QString additional;
    QString patient;  // for EDF and BDF format
    Gender gender;
    EDFDate birthdate;
};
#endif  // EDFPATIENT_H
